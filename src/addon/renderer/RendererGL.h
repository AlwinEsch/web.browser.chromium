/*
 *      Copyright (C) 2015-2019 Team KODI
 *      http:/kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
