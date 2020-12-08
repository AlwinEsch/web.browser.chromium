/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "WorkRendererGL.h"
#include "OffscreenBasePosix.h"
#include "../MainCEFProcess.h"
#include "../WebBrowserClient.h"
#include "../../../../lib/kodi-dev-kit/src/sandbox/OpenGLExtensionCheck.h"
#include "../../../../lib/kodi-dev-kit/src/sandbox/shared_data_structure.h"
#include "../../../../lib/kodi-dev-kit/src/sandbox/socket.h"

#include <thread>

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_UNSIGNED_BYTE
#define GL_UNSIGNED_BYTE 0x8367
#endif

namespace chromium
{
namespace app
{
namespace main
{
namespace renderer
{

CWorkRendererGL::CWorkRendererGL(CefRefPtr<CWebBrowserClient> client)
  : IRenderer(client),
    m_offsceenBasePosix(std::dynamic_pointer_cast<COffsceenBasePosix>(client->GetMain().GetOffsceenGUIBase())),
    m_socketServerFile(client->GetClientIdentifier() + "-renderproc"),
    m_socketClientFile(client->GetClientIdentifier() + "-renderview")
{
}

CWorkRendererGL::~CWorkRendererGL()
{
  Deinitialize();
}

bool CWorkRendererGL::Initialize(int width, int height)
{
  m_eglDisplay = m_offsceenBasePosix->GetEGLDisplay();
  m_eglContext = m_offsceenBasePosix->GetEGLContext();

  const EGLint pbufferAttribs[] = {
      EGL_WIDTH,          9,
      EGL_HEIGHT,         9,
      EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
      EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGBA,
      EGL_NONE,
  };

  // create an EGL window surface
  m_eglSurface = eglCreatePbufferSurface(m_eglDisplay, m_offsceenBasePosix->GetEGLConfig(), pbufferAttribs);

  // connect the context to the surface
  eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

  glGenVertexArrays(1, &m_vertexVAO);
  glBindVertexArray(m_vertexVAO);

  m_width = width;
  m_height = height;

  std::string vertShader, fraqShader;
  GetShaderPath(vertShader, fraqShader);
  if (!LoadShaderFiles(vertShader, fraqShader) || !CompileAndLink())
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to create or compile shader");
    return false;
  }

  if (!GLExtCheck::CheckExtensionSupported("EGL_KHR_image_base"))
  {
    kodi::Log(ADDON_LOG_ERROR, "Needed EGL extension \"EGL_KHR_image_base\" not available");
    return false;
  }

  if (!GLExtCheck::CheckExtensionSupported("EGL_MESA_image_dma_buf_export"))
  {
    kodi::Log(ADDON_LOG_ERROR,
              "Needed EGL extension \"EGL_MESA_image_dma_buf_export\" not available");
    return false;
  }

  eglCreateImageKHR = (decltype(eglCreateImageKHR))GLExtCheck::GetEGLFunc("eglCreateImageKHR");
  eglDestroyImageKHR = (decltype(eglDestroyImageKHR))GLExtCheck::GetEGLFunc("eglDestroyImageKHR");
  eglExportDMABUFImageQueryMESA = (decltype(eglExportDMABUFImageQueryMESA))GLExtCheck::GetEGLFunc(
      "eglExportDMABUFImageQueryMESA");
  eglExportDMABUFImageMESA =
      (decltype(eglExportDMABUFImageMESA))GLExtCheck::GetEGLFunc("eglExportDMABUFImageMESA");

  // GL: Create and populate the texture
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, m_width);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  static const EGLint attrib[] = {EGL_IMAGE_PRESERVED_KHR, EGL_FALSE, EGL_GL_TEXTURE_LEVEL_KHR, 0,
                                  EGL_NONE};

  // EGL: Create EGL image from the GL texture
  m_eglImage = eglCreateImageKHR(m_eglDisplay, m_eglContext, EGL_GL_TEXTURE_2D_KHR,
                                 reinterpret_cast<EGLClientBuffer>(m_texture), attrib);
  if (m_eglImage == EGL_NO_IMAGE)
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to get EGL image");
    return false;
  }

  // The next line works around an issue in radeonsi driver (fixed in master at the time of writing). If you are
  // not having problems with texture rendering until the first texture update you can omit this line
  glFinish();

  EGLBoolean queried = eglExportDMABUFImageQueryMESA(
      m_eglDisplay, m_eglImage, &m_textureStorageMetadata.fourcc,
      &m_textureStorageMetadata.num_planes, &m_textureStorageMetadata.modifiers);
  if (!queried)
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to export DMABUF image query");
    return false;
  }

  EGLBoolean exported = eglExportDMABUFImageMESA(m_eglDisplay, m_eglImage, &m_textureDmaBufFd,
                                                 &m_textureStorageMetadata.stride, &m_textureStorageMetadata.offset);
  if (!exported)
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to export DMABUF image");
    return false;
  }

  eglDestroyImageKHR(m_eglDisplay, m_eglImage);

  m_textureStorageMetadata.width = m_width;
  m_textureStorageMetadata.height = m_height;

  glGenBuffers(1, &m_vertexVBO);
  glGenBuffers(1, &m_indexVBO);

  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
  //Attach 2D texture to this FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

  glGenRenderbuffers(1, &m_renderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);

  //Attach depth buffer to FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Unix Domain Socket: Send file descriptor (texture_dmabuf_fd) and texture storage data (texture_storage_metadata)
  m_socket = create_socket(m_socketServerFile.c_str());
  std::thread thread = std::thread([&] {
    while (m_socket >= 0 && connect_socket(m_socket, m_socketClientFile.c_str()) != 0)
      ;
    write_fd(m_socket, m_textureDmaBufFd, &m_textureStorageMetadata,
             sizeof(m_textureStorageMetadata));
    close(m_socket);
    m_socket = -1;
    close(m_textureDmaBufFd);
    m_textureDmaBufFd = -1;
  });

  thread.detach();

  m_dirty = true;
  m_inited = true;
  return true;
}

void CWorkRendererGL::Deinitialize()
{
  if (m_socket >= 0)
  {
    int socket = m_socket;
    m_socket = -1;
    close(socket);
    if (m_textureDmaBufFd >= 0)
    {
      m_textureDmaBufFd = -1;
      close(m_textureDmaBufFd);
    }
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glDeleteRenderbuffers(1, &m_renderbuffer);
  m_renderbuffer = 0;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &m_framebuffer);
  m_framebuffer = 0;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &m_texture);
  m_texture = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &m_vertexVBO);
  m_vertexVBO = 0;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &m_indexVBO);
  m_indexVBO = 0;

  glDeleteVertexArrays(1, &m_vertexVAO);
  m_vertexVAO = 0;
}

void CWorkRendererGL::OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height)
{
  eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

  glBindVertexArray(m_vertexVAO);

  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  if (type == PET_VIEW)
  {
    int old_width = m_viewWidth;
    int old_height = m_viewHeight;

    m_viewWidth = width;
    m_viewHeight = height;

#ifdef SHOW_UPDATE_RECT
    m_updateRect = dirtyRects[0];
#endif // SHOW_UPDATE_RECT

    if (old_width != m_viewWidth || old_height != m_viewHeight ||
        (dirtyRects.size() == 1 &&
        dirtyRects[0] == CefRect(0, 0, m_viewWidth, m_viewHeight)))
    {
      // Update/resize the whole texture.
      glBindTexture(GL_TEXTURE_2D, m_texture);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, m_viewWidth);
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
      glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_viewWidth, m_viewHeight, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
    }
    else
    {
      // Update just the dirty rectangles.
      for (const auto& rect : dirtyRects)
      {
        DCHECK(rect.x + rect.width <= m_viewWidth);
        DCHECK(rect.y + rect.height <= m_viewHeight);

        glBindTexture(GL_TEXTURE_2D, m_texture);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, m_viewWidth);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect.x);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, rect.y);
        glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width, rect.height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
      }
    }
  }
  else if (type == PET_POPUP && m_popupRect.width > 0 && m_popupRect.height > 0)
  {
    int skip_pixels = 0, x = m_popupRect.x;
    int skip_rows = 0, y = m_popupRect.y;
    int w = width;
    int h = height;

    // Adjust the popup to fit inside the view.
    if (x < 0)
    {
      skip_pixels = -x;
      x = 0;
    }
    if (y < 0)
    {
      skip_rows = -y;
      y = 0;
    }
    if (x + w > m_viewWidth)
      w -= x + w - m_viewWidth;
    if (y + h > m_viewHeight)
      h -= y + h - m_viewHeight;

    // Update the popup rectangle.
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, skip_pixels);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, skip_rows);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
  }

  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

  EnableShader();
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
  DisableShader();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFlush();

  SetDirty();
}

void CWorkRendererGL::ScreenSizeChange(float x, float y, float width, float height)
{
}

void CWorkRendererGL::OnCompiledAndLinked()
{
  m_aPosition = glGetAttribLocation(ProgramHandle(), "aPos");
  m_aCoord = glGetAttribLocation(ProgramHandle(), "aTexCoords");
}

bool CWorkRendererGL::OnEnabled()
{
  return true;
}

void CWorkRendererGL::GetShaderPath(std::string& vert, std::string& frag)
{
  std::string path;

#if defined(HAS_GL)
  int glslMajor = 0;
  int glslMinor = 0;

  const char* ver = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
  if (ver)
  {
    sscanf(ver, "%d.%d", &glslMajor, &glslMinor);
  }
  else
  {
    glslMajor = 1;
    glslMinor = 0;
  }

  if (glslMajor >= 2 || (glslMajor == 1 && glslMinor >= 50))
    path = kodi::GetAddonPath("resources/shaders/GL/1.5/");
  else
    path = kodi::GetAddonPath("resources/shaders/GL/1.2/");
#else
  path = kodi::GetAddonPath("resources/shaders/GLES/");
#endif

  kodi::Log(ADDON_LOG_DEBUG, "Used addon shader language path: '%s'", path.c_str());

  vert = path + "/vert.glsl";
  frag = path + "/frag.glsl";
}

} /* renderer */
} /* main */
} /* app */
} /* chromium */
