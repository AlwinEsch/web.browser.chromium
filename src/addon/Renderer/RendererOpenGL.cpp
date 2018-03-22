/*
 *      Copyright (C) 2015-2017 Team KODI
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

#if defined(OS_MACOSX)
#include <OpenGL/gl.h>
#elif defined(OS_LINUX)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#error Platform is not supported.
#endif

#include <p8-platform/threads/mutex.h>

using namespace P8PLATFORM;

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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//#define SHOW_UPDATE_RECT 1

CRendererClientOpenGL::CRendererClientOpenGL(CWebBrowserClient const* client) :
  IRenderer(client),
  m_textureId(1),
  m_viewWidth(0),
  m_viewHeight(0)
{
}

CRendererClientOpenGL::~CRendererClientOpenGL()
{  
  delete m_shader;
  m_shader = nullptr;

  glDeleteTextures(1, &m_textureId);
}

bool CRendererClientOpenGL::Initialize()
{
  m_shader = new CGUIShader("vert.glsl", "frag.glsl");

  if (!m_shader->CompileAndLink())
  {
    delete m_shader;
    m_shader = nullptr;
    return false;
  }
  
  if (m_useTransparentBackground)
  {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); VERIFY_NO_ERROR;
  }
  else
  {
    glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]); VERIFY_NO_ERROR;
  }

  // Create the texture.
  glGenTextures(1, &m_textureId); VERIFY_NO_ERROR;
  DCHECK_NE(m_textureId, 0U); VERIFY_NO_ERROR;

  glBindTexture(GL_TEXTURE_2D, m_textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  return true;
}

void CRendererClientOpenGL::Render()
{
  m_shader->MatrixMode(MM_MODELVIEW);
  m_shader->LoadIdentity();
  m_shader->MatrixMode(MM_PROJECTION);
  m_shader->LoadIdentity();

  CPaintMessage* msg = GetPaintMessage();
  if (msg)
  {
    CefBrowserHost::PaintElementType type = msg->type;
    const CefRenderHandler::RectList& dirtyRects = msg->dirtyRects;
    const void* buffer = msg->buffer;
    int width = msg->width;
    int height = msg->height;
    
    glBindTexture(GL_TEXTURE_2D, m_textureId);
  
    if (type == PET_VIEW)
    {
      int old_width = m_viewWidth;
      int old_height = m_viewHeight;
    
      m_viewWidth = width;
      m_viewHeight = height;
    
#ifdef SHOW_UPDATE_RECT
      m_updateRect = dirtyRects[0];
#endif // SHOW_UPDATE_RECT
    
      glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, m_viewWidth); VERIFY_NO_ERROR;

      if (old_width != m_viewWidth || old_height != m_viewHeight ||
          (dirtyRects.size() == 1 &&
          dirtyRects[0] == CefRect(0, 0, m_viewWidth, m_viewHeight)))
      {
        // Update/resize the whole texture.
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0); VERIFY_NO_ERROR;
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); VERIFY_NO_ERROR;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_viewWidth, m_viewHeight, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer); VERIFY_NO_ERROR;
      }
      else
      {
        // Update just the dirty rectangles.
        CefRenderHandler::RectList::const_iterator i = dirtyRects.begin();
        for (; i != dirtyRects.end(); ++i)
        {
          const CefRect& rect = *i;
          DCHECK(rect.x + rect.width <= m_viewWidth);
          DCHECK(rect.y + rect.height <= m_viewHeight);
          glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect.x); VERIFY_NO_ERROR;
          glPixelStorei(GL_UNPACK_SKIP_ROWS, rect.y); VERIFY_NO_ERROR;
          glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width, rect.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer); VERIFY_NO_ERROR;
        }
      }
      glPopClientAttrib();
    }
    else if (type == PET_POPUP && m_popupRect.width > 0 && m_popupRect.height > 0)
    {
      int skip_pixels = 0, x = m_popupRect.x;
      int skip_rows = 0, y = m_popupRect.y;
      int w = width;
      int h = height;

      glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

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
      glPixelStorei(GL_UNPACK_ROW_LENGTH, width); VERIFY_NO_ERROR;
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, skip_pixels); VERIFY_NO_ERROR;
      glPixelStorei(GL_UNPACK_SKIP_ROWS, skip_rows); VERIFY_NO_ERROR;
      glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA,
                      GL_UNSIGNED_INT_8_8_8_8_REV, buffer); VERIFY_NO_ERROR;
      glPopClientAttrib();
    }
    delete msg;
  }

  if (!m_useTransparentBackground)
  {
    // Draw the background gradient.
    glPushAttrib(GL_ALL_ATTRIB_BITS); VERIFY_NO_ERROR;
    // Don't check for errors until glEnd().
    glBegin(GL_QUADS);
    glColor4f(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glColor4f(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd(); VERIFY_NO_ERROR;
    glPopAttrib(); VERIFY_NO_ERROR;
  }
  else
  {
    // Enable alpha blending.
    glEnable(GL_BLEND); VERIFY_NO_ERROR;

    // Alpha blending style. Texture values have premultiplied alpha.
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); VERIFY_NO_ERROR;
    glColor4f(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
  }
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_textureId);

#if defined(HAS_GL)

  GLubyte idx[4] = {0, 1, 3, 2};
  GLuint vertexVBO;
  GLuint indexVBO;
  struct PackedVertex
  {
    float x, y, z;
    float u1, v1;
  }vertex[4];

  m_shader->Enable();

  GLint posLoc = m_shader->GetPosLoc();
  GLint texLoc = m_shader->GetCordLoc();

  vertex[0].x = -1.0f;
  vertex[0].y = 1.0f;
  vertex[0].z = 0.0f;
  vertex[0].u1 = 0.0f;
  vertex[0].v1 = 0.0f;

  vertex[1].x = 1.0f;
  vertex[1].y = 1.0f;
  vertex[1].z = 0.0f;
  vertex[1].u1 = 1.0f;
  vertex[1].v1 = 0.0f;

  vertex[2].x = 1.0f;
  vertex[2].y = -1.0f;
  vertex[2].z = 0.0f;
  vertex[2].u1 = 1.0f;
  vertex[2].v1 = 1.0f;

  vertex[3].x = -1.0f;
  vertex[3].y = -1.0f;
  vertex[3].z = 0.0f;
  vertex[3].u1 = 0.0f;
  vertex[3].v1 = 1.0f;
  
  glGenBuffers(1, &vertexVBO);
  glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(PackedVertex)*4, &vertex[0], GL_STATIC_DRAW);

  glVertexAttribPointer(posLoc,  3, GL_FLOAT, 0, sizeof(PackedVertex), BUFFER_OFFSET(offsetof(PackedVertex, x)));
  glEnableVertexAttribArray(posLoc);

  glVertexAttribPointer(texLoc, 2, GL_FLOAT, 0, sizeof(PackedVertex), BUFFER_OFFSET(offsetof(PackedVertex, u1)));
  glEnableVertexAttribArray(texLoc);

  glGenBuffers(1, &indexVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*4, idx, GL_STATIC_DRAW);

  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, 0);

  glDisableVertexAttribArray(posLoc);
  glDisableVertexAttribArray(texLoc);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vertexVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &indexVBO);

#elif defined(HAS_GLES2)

  GLubyte idx[4] = {0, 1, 3, 2};        //determines order of triangle strip
  GLfloat ver[4][4];
  GLfloat tex[4][2];

  m_shader->Enable();
  GLint posLoc = m_shader->GetPosLoc();
  GLint texLoc = m_shader->GetCordLoc();

  glVertexAttribPointer(posLoc, 4, GL_FLOAT, 0, 0, ver);
  glVertexAttribPointer(texLoc, 2, GL_FLOAT, 0, 0, tex);

  glEnableVertexAttribArray(posLoc);
  glEnableVertexAttribArray(texLoc);

  // Set vertex coordinates
  for (int i = 0; i < 4; i++)
  {
    ver[i][2] = 0.0f;// set z to 0
    ver[i][3] = 1.0f;
  }
  ver[0][0] = ver[3][0] = -1.0f;
  ver[0][1] = ver[1][1] = -1.0f;
  ver[1][0] = ver[2][0] = 1.0f;
  ver[2][1] = ver[3][1] = 1.0f;

  // Set texture coordinates
  tex[0][0] = tex[3][0] = 0;
  tex[0][1] = tex[1][1] = 0;
  tex[1][0] = tex[2][0] = 1;
  tex[2][1] = tex[3][1] = 1;

  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, idx);

  glDisableVertexAttribArray(posLoc);
  glDisableVertexAttribArray(texLoc);

#endif
  
  m_shader->Disable();

  // Disable 2D textures.
  glDisable(GL_TEXTURE_2D); VERIFY_NO_ERROR;

  // Disable alpha blending.
  if (m_useTransparentBackground)
    glDisable(GL_BLEND); VERIFY_NO_ERROR;
    
#ifdef SHOW_UPDATE_RECT
  // Draw a rectangle around the update region.
  if (!m_updateRect.IsEmpty())
  {
    int left = m_updateRect.x;
    int right = m_updateRect.x + m_updateRect.width;
    int top = m_updateRect.y;
    int bottom = m_updateRect.y + m_updateRect.height;

#if defined(OS_LINUX)
    // Shrink the box so that top & right sides are drawn.
    top += 1;
    right -= 1;
#else
    // Shrink the box so that left & bottom sides are drawn.
    left += 1;
    bottom -= 1;
#endif

    glPushAttrib(GL_ALL_ATTRIB_BITS); VERIFY_NO_ERROR
    glMatrixMode(GL_PROJECTION); VERIFY_NO_ERROR;
    glPushMatrix(); VERIFY_NO_ERROR;
    glLoadIdentity(); VERIFY_NO_ERROR;
    glOrtho(0, m_viewWidth, m_viewHeight, 0, 0, 1); VERIFY_NO_ERROR;

    glLineWidth(1); VERIFY_NO_ERROR;
    glColor3f(1.0f, 0.0f, 0.0f); VERIFY_NO_ERROR;
    // Don't check for errors until glEnd().
    glBegin(GL_LINE_STRIP);
    glVertex2i(left, top);
    glVertex2i(right, top);
    glVertex2i(right, bottom);
    glVertex2i(left, bottom);
    glVertex2i(left, top);
    glEnd(); VERIFY_NO_ERROR;

    glPopMatrix(); VERIFY_NO_ERROR;
    glPopAttrib(); VERIFY_NO_ERROR;
  }
#endif // SHOW_UPDATE_RECT

  glBindTexture(GL_TEXTURE_2D, 0);    
}

void CRendererClientOpenGL::ScreenSizeChange(float x, float y, float width, float height)
{
}
