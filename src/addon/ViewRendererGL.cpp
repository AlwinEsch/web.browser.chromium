/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "ViewRendererGL.h"
#include "../../lib/kodi-dev-kit/src/sandbox/OpenGLExtensionCheck.h"
#include "../../lib/kodi-dev-kit/src/sandbox/shared_data_structure.h"
#include "../../lib/kodi-dev-kit/src/sandbox/socket.h"

CViewRendererOpenGL::CViewRendererOpenGL(const std::string& clientIdentifier)
  : m_socketClientFile(clientIdentifier + "-renderview")
{
}

CViewRendererOpenGL::~CViewRendererOpenGL()
{
  Deinit();
}

bool CViewRendererOpenGL::Init()
{
  if (m_inited)
    return true;

  std::string vertShader, fraqShader;
  GetShaderPath(vertShader, fraqShader);
  if (!LoadShaderFiles(vertShader, fraqShader) || !CompileAndLink())
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to create or compile shader");
    return false;
  }

  m_eglDisplay = eglGetCurrentDisplay();
  if (m_eglDisplay == EGL_NO_DISPLAY)
  {
    kodi::Log(ADDON_LOG_ERROR, "Not get EGL display");
    return false;
  }

  if (!GLExtCheck::CheckExtensionSupported("EGL_KHR_image_base"))
  {
    kodi::Log(ADDON_LOG_ERROR, "Needed EGL extension \"EGL_KHR_image_base\" not available");
    return false;
  }

  if (!GLExtCheck::CheckExtensionSupported("GL_OES_EGL_image"))
  {
    kodi::Log(ADDON_LOG_ERROR, "Needed EGL extension \"GL_OES_EGL_image\" not available");
    return false;
  }

  eglCreateImageKHR = (decltype(eglCreateImageKHR))GLExtCheck::GetEGLFunc("eglCreateImageKHR");
  eglDestroyImageKHR = (decltype(eglDestroyImageKHR))GLExtCheck::GetEGLFunc("eglDestroyImageKHR");
  glEGLImageTargetTexture2DOES = (decltype(glEGLImageTargetTexture2DOES))GLExtCheck::GetEGLFunc("glEGLImageTargetTexture2DOES");

  glEnable(GL_BLEND);

  // Unix Domain Socket: Receive file descriptor (texture_dmabuf_fd) and texture storage data (dmabuf)
  int texture_dmabuf_fd;
  struct texture_storage_metadata_t dmabuf;

  int sock = create_socket(m_socketClientFile.c_str());
  read_fd(sock, &texture_dmabuf_fd, &dmabuf, sizeof(dmabuf));
  close(sock);

  int i = 0;
  EGLint attribute_list[64];

  attribute_list[i++] = EGL_WIDTH;
  attribute_list[i++] = dmabuf.width;
  attribute_list[i++] = EGL_HEIGHT;
  attribute_list[i++] = dmabuf.height;
  attribute_list[i++] = EGL_LINUX_DRM_FOURCC_EXT;
  attribute_list[i++] = dmabuf.fourcc;

  attribute_list[i++] = EGL_DMA_BUF_PLANE0_FD_EXT;
  attribute_list[i++] = texture_dmabuf_fd;
  attribute_list[i++] = EGL_DMA_BUF_PLANE0_PITCH_EXT;
  attribute_list[i++] = dmabuf.stride;
  attribute_list[i++] = EGL_DMA_BUF_PLANE0_OFFSET_EXT;
  attribute_list[i++] = dmabuf.offset;
#ifdef EGL_DMA_BUF_PLANE0_MODIFIER_LO_EXT
  if (dmabuf.modifiers)
  {
    attribute_list[i++] = EGL_DMA_BUF_PLANE0_MODIFIER_LO_EXT;
    attribute_list[i++] = static_cast<EGLint>(dmabuf.modifiers & 0xFFFFFFFF);
    attribute_list[i++] = EGL_DMA_BUF_PLANE0_MODIFIER_HI_EXT;
    attribute_list[i++] = static_cast<EGLint>(dmabuf.modifiers >> 32);
  }
#endif
  attribute_list[i++] = EGL_NONE;

  m_image = eglCreateImageKHR(m_eglDisplay, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, nullptr,
                              attribute_list);
  if (m_image == EGL_NO_IMAGE)
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to get EGL image");
    return false;
  }

  // GL texture that will be shared
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_image);
  eglDestroyImageKHR(m_eglDisplay, m_image);

  glBindTexture(GL_TEXTURE_2D, 0);

  close(texture_dmabuf_fd);

  glGenBuffers(1, &m_vertexVBO);
  glGenBuffers(1, &m_indexVBO);

  m_inited = true;
  return true;
}

void CViewRendererOpenGL::Deinit()
{
  if (!m_inited)
    return;

  m_inited = false;

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
}

void CViewRendererOpenGL::Render()
{
//   fprintf(stderr, "<<<<<<<<<<<<<<<<<<<<< %p\n", this);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  EnableShader();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);

  DisableShader();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CViewRendererOpenGL::OnCompiledAndLinked()
{
  m_aPosition = glGetAttribLocation(ProgramHandle(), "aPos");
  m_aCoord = glGetAttribLocation(ProgramHandle(), "aTexCoords");
}

bool CViewRendererOpenGL::OnEnabled()
{
  return true;
}

void CViewRendererOpenGL::GetShaderPath(std::string& vert, std::string& frag)
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
