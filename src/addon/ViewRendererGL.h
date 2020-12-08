/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <kodi/gui/gl/Shader.h>

class ATTRIBUTE_HIDDEN CViewRendererOpenGL : public kodi::gui::gl::CShaderProgram
{
public:
  CViewRendererOpenGL(const std::string& clientIdentifier);
  ~CViewRendererOpenGL();

  bool Init();
  void Deinit();
  void Render();

  void OnCompiledAndLinked() override;
  bool OnEnabled() override;

private:
  void GetShaderPath(std::string& vert, std::string& frag);

  const float m_vertices[20] = {
      -1.0f, 1.0f,  0.0f, 0.0f, 0.0f, // bottom left
      1.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom right
      1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, // top right
      -1.0f, -1.0f, 0.0f, 0.0f, 1.0f // top left
  };
  const unsigned char m_indices[4] = {0, 1, 3, 2};

  const std::string m_socketClientFile;

  EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
  EGLImageKHR m_image = EGL_NO_IMAGE_KHR;

  GLuint m_vertexVBO = 0;
  GLuint m_indexVBO = 0;

  GLint m_aPosition = -1;
  GLint m_aCoord = -1;
  GLuint m_texture = 0;

  PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR = nullptr;
  PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR = nullptr;
  PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES = nullptr;

  bool m_inited = false;
};
