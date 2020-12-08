/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IRenderer.h"

#include "../../../../lib/kodi-dev-kit/src/sandbox/shared_data_structure.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/gl/Shader.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>

#undef Success

struct gbm_surface;
struct gbm_device;

namespace chromium
{
namespace app
{
namespace main
{

class CWebBrowserClient;

namespace renderer
{

class COffsceenBasePosix;

class ATTRIBUTE_HIDDEN CWorkRendererGL
 : public IRenderer,
   public kodi::gui::gl::CShaderProgram
{
public:
  CWorkRendererGL(CefRefPtr<CWebBrowserClient> client);
  ~CWorkRendererGL();

  bool Initialize(int width, int height) override;
  void Deinitialize() override;

  void OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) override;

  void ScreenSizeChange(float x, float y, float width, float height) override;

  void OnCompiledAndLinked() override;
  bool OnEnabled() override;

private:
  void GetShaderPath(std::string& vert, std::string& frag);

  std::shared_ptr<COffsceenBasePosix> m_offsceenBasePosix;

  const float m_vertices[20] = {
      -1.0f, 1.0f,  0.0f, 0.0f, 0.0f, // top right
      1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, // bottom left
      1.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom right
      1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, // bottom left
  };
  const unsigned char m_indices[4] = {0, 1, 3, 2};

  PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR = nullptr;
  PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR = nullptr;
  PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC eglExportDMABUFImageQueryMESA = nullptr;
  PFNEGLEXPORTDMABUFIMAGEMESAPROC eglExportDMABUFImageMESA = nullptr;

  const std::string m_socketServerFile;
  const std::string m_socketClientFile;

  EGLSurface m_eglSurface = EGL_NO_SURFACE;
  EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
  EGLContext m_eglContext = EGL_NO_CONTEXT;
  EGLImageKHR m_eglImage = EGL_NO_IMAGE_KHR;
  EGLConfig m_eglConfig = EGL_NO_CONFIG_KHR;

  int m_width;
  int m_height;

  // EGL (extension: EGL_MESA_image_dma_buf_export): Get file descriptor (texture_dmabuf_fd) for the EGL image and get its
  // storage data (texture_storage_metadata - fourcc, stride, offset)
  int m_socket = -1;
  int m_textureDmaBufFd = -1;
  struct texture_storage_metadata_t m_textureStorageMetadata;

  GLuint m_vertexVAO = 0;
  GLuint m_vertexVBO = 0;
  GLuint m_indexVBO = 0;

  GLint m_aPosition = -1;
  GLint m_aCoord = -1;

  GLuint m_texture = 0;
  GLuint m_framebuffer = 0;
  GLuint m_renderbuffer = 0;

  uint8_t* buffer;

  bool m_inited = false;
};

} /* renderer */
} /* main */
} /* app */
} /* chromium */
