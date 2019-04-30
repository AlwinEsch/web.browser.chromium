/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IRenderer.h"
#include "include/cef_render_handler.h"

#include <kodi/gui/gl/GL.h>
#include <kodi/gui/gl/Shader.h>
#include <glm/gtc/type_ptr.hpp>

class CWebBrowserClient;

class ATTRIBUTE_HIDDEN CRendererClientOpenGL
  : public IRenderer,
    public kodi::gui::gl::CShaderProgram
{
public:
  CRendererClientOpenGL(CWebBrowserClient const* client);
  virtual ~CRendererClientOpenGL();

  bool Initialize() override;
  void Deinitialize() override;

  void OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) override;

  void Render() override;
  void ScreenSizeChange(float x, float y, float width, float height) override;

  void OnCompiledAndLinked() override;
  bool OnEnabled() override;

private:
  glm::mat4 m_modelProjMat = glm::mat4(1.0f);
  glm::vec3 m_vertexPos[4];
  glm::vec2 m_vertexCoord[4];
  GLubyte m_idx[4] = {0, 1, 3, 2};

  GLuint m_vertexVBO[2] = { 0 };
  GLuint m_indexVBO = 0;

  GLint m_uModelProjMatrix = -1;
  GLint m_uBackgroundColor = -1;
  GLint m_uClearColor = -1;
  GLint m_aPosition = -1;
  GLint m_aCoord = -1;

  GLuint m_textureId = 0;
};
