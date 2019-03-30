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

#include "RendererOpenGL.h"

#include "addon.h"
#include "Utils.h"
#include "WebBrowserClient.h"

#include "include/wrapper/cef_helpers.h"

#include <kodi/General.h>
#include <glm/glm.hpp>

#if !defined(GL_UNPACK_ROW_LENGTH)
  #if defined(GL_UNPACK_ROW_LENGTH_EXT)
    #define GL_UNPACK_ROW_LENGTH GL_UNPACK_ROW_LENGTH_EXT
    #define GL_UNPACK_SKIP_ROWS GL_UNPACK_SKIP_ROWS_EXT
    #define GL_UNPACK_SKIP_PIXELS GL_UNPACK_SKIP_PIXELS_EXT
  #else
    #undef HAS_GLES2
  #endif
#endif

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_REV
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#endif

// DCHECK on gl errors.
#ifndef NDEBUG
#define VERIFY_NO_ERROR { \
    int _gl_error = glGetError(); \
    DCHECK(_gl_error == GL_NO_ERROR) << \
    "glGetError returned " << _gl_error; \
  }
#else
#define VERIFY_NO_ERROR
#endif

// #define SHOW_UPDATE_RECT 1

CRendererClientOpenGL::CRendererClientOpenGL(CWebBrowserClient const* client) :
  IRenderer(client),
  m_textureId(0)
{
  m_vertexPos[0] = glm::vec3(-1.0f,  1.0f, 0.0f);
  m_vertexPos[1] = glm::vec3( 1.0f,  1.0f, 0.0f);
  m_vertexPos[2] = glm::vec3( 1.0f, -1.0f, 0.0f);
  m_vertexPos[3] = glm::vec3(-1.0f, -1.0f, 0.0f);

  m_vertexCoord[0] = glm::vec2(0.0f, 0.0f);
  m_vertexCoord[1] = glm::vec2(1.0f, 0.0f);
  m_vertexCoord[2] = glm::vec2(1.0f, 1.0f);
  m_vertexCoord[3] = glm::vec2(0.0f, 1.0f);
}

CRendererClientOpenGL::~CRendererClientOpenGL()
{  
  Deinitialize();
}

bool CRendererClientOpenGL::Initialize()
{
  std::string fraqShader = kodi::GetAddonPath("resources/shaders/" GL_TYPE_STRING "/frag.glsl");
  std::string vertShader = kodi::GetAddonPath("resources/shaders/" GL_TYPE_STRING "/vert.glsl");
  if (!LoadShaderFiles(vertShader, fraqShader) || !CompileAndLink())
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to create or compile shader");
    return false;
  }

  glGenBuffers(2, m_vertexVBO);
  glGenBuffers(1, &m_indexVBO);
  glGenTextures(1, &m_textureId);
  glBindTexture(GL_TEXTURE_2D, m_textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  m_dirty = true;
  return true;
}

void CRendererClientOpenGL::Deinitialize()
{
  glDeleteTextures(1, &m_textureId);
  glDeleteBuffers(2, m_vertexVBO);
  glDeleteBuffers(1, &m_indexVBO);

  m_textureId = 0;
  m_vertexVBO[0] = 0;
  m_vertexVBO[1] = 0;
  m_indexVBO = 0;
}

void CRendererClientOpenGL::OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height)
{
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
      glBindTexture(GL_TEXTURE_2D, m_textureId);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, m_viewWidth);
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
      glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_viewWidth, m_viewHeight, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
    }
    else
    {
      // Update just the dirty rectangles.
      for (const auto& rect : dirtyRects)
      {
        DCHECK(rect.x + rect.width <= m_viewWidth);
        DCHECK(rect.y + rect.height <= m_viewHeight);

        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, m_viewWidth);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect.x);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, rect.y);
        glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width, rect.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
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
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, skip_pixels);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, skip_rows);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
  }

  SetDirty();

  glBindTexture(GL_TEXTURE_2D, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
}

void CRendererClientOpenGL::Render()
{
  if (m_useTransparentBackground)
  {
    // Enable alpha blending.
    glEnable(GL_BLEND);

    // Alpha blending style. Texture values have premultiplied alpha.
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureId);

  EnableShader();

#if defined(HAS_GL)
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexPos), m_vertexPos, GL_STATIC_DRAW);
  glVertexAttribPointer(m_aPosition, 3, GL_FLOAT, 0, sizeof(glm::vec3), 0);
  glEnableVertexAttribArray(m_aPosition);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexCoord), m_vertexCoord, GL_STATIC_DRAW);
  glVertexAttribPointer(m_aCoord, 2, GL_FLOAT, 0, sizeof(glm::vec2), 0);
  glEnableVertexAttribArray(m_aCoord);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*4, m_idx, GL_STATIC_DRAW);

  if (!m_useTransparentBackground)
  {
    glUniform1i(m_uClearColor, true);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);
  }

  glUniform1i(m_uClearColor, false);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableVertexAttribArray(m_aPosition);
  glDisableVertexAttribArray(m_aCoord);

#elif defined(HAS_GLES2)
  glVertexAttribPointer(m_aPosition, 2, GL_FLOAT, 0, 0, m_vertexPos);
  glEnableVertexAttribArray(m_aPosition);

  glVertexAttribPointer(m_aCoord, 2, GL_FLOAT, 0, 0, m_vertexCoord);
  glEnableVertexAttribArray(m_aCoord);

  if (!m_useTransparentBackground)
  {
    glUniform1i(m_uClearColor, true);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, m_idx);
  }

  glUniform1i(m_uClearColor, false);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, m_idx);

  glDisableVertexAttribArray(m_aPosition);
  glDisableVertexAttribArray(m_aCoord);

#endif

  // Disable alpha blending.
  if (m_useTransparentBackground)
    glDisable(GL_BLEND);

#ifdef SHOW_UPDATE_RECT
  // Draw a rectangle around the update region.
  if (!m_updateRect.IsEmpty())
  {
    float left = m_updateRect.x;
    float right = m_updateRect.x + m_updateRect.width;
    float top = m_updateRect.y;
    float bottom = m_updateRect.y + m_updateRect.height;

#if defined(OS_LINUX)
    // Shrink the box so that top & right sides are drawn.
    top += 1;
    right -= 1;
#else
    // Shrink the box so that left & bottom sides are drawn.
    left += 1;
    bottom -= 1;
#endif

    glm::vec2 vertexPos[5];
    vertexPos[0] = glm::vec2(left, top);
    vertexPos[1] = glm::vec2(right, top);
    vertexPos[2] = glm::vec2(right, bottom);
    vertexPos[3] = glm::vec2(left, bottom);
    vertexPos[4] = glm::vec2(left, top);
    m_modelProjMat = glm::ortho(0.0f, float(m_viewWidth), float(m_viewHeight), 0.0f);

    glLineWidth(2);
    glUniform1i(m_uClearColor, true);
    glUniform4f(m_uBackgroundColor, 1.0f, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(m_uModelProjMatrix, 1, GL_FALSE, glm::value_ptr(m_modelProjMat));

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
    glVertexAttribPointer(m_aPosition, 2, GL_FLOAT, 0, sizeof(glm::vec2), 0);
    glEnableVertexAttribArray(m_aPosition);

    glDrawArrays(GL_LINE_STRIP, 0, 5);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(m_aPosition);
    m_modelProjMat = glm::mat4(1.0f);
  }
#endif // SHOW_UPDATE_RECT

  DisableShader();
}

void CRendererClientOpenGL::ScreenSizeChange(float x, float y, float width, float height)
{
}

void CRendererClientOpenGL::OnCompiledAndLinked()
{
  // Variables passed directly to the Vertex shader
  m_uModelProjMatrix = glGetUniformLocation(ProgramHandle(), "u_modelProjMatrix");
  m_uBackgroundColor = glGetUniformLocation(ProgramHandle(), "u_backgroundColor");
  m_uClearColor = glGetUniformLocation(ProgramHandle(), "u_clearColor");
  m_aPosition = glGetAttribLocation(ProgramHandle(), "a_position");
  m_aCoord = glGetAttribLocation(ProgramHandle(), "a_coord");
}

bool CRendererClientOpenGL::OnEnabled()
{
  // This is called after glUseProgram()
  glUniformMatrix4fv(m_uModelProjMatrix, 1, GL_FALSE, glm::value_ptr(m_modelProjMat));
  glUniform4f(m_uBackgroundColor, m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
  return true;
}
