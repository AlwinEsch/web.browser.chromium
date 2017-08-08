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

using namespace P8PLATFORM;

#define MAX_MESSAGE_QUEUE_FILL_SIZE 100

CRendererClientOpenGL::CRendererClientOpenGL(CWebBrowserClient* client) :
  m_client(client),
  m_initialized(false),
  m_textureId(0),
  m_viewWidth(0),
  m_viewHeight(0),
  m_fSpinX(0),
  m_fSpinY(0),
  m_dirty(false),
  m_useTransparentBackground(false)
{
}

CRendererClientOpenGL::~CRendererClientOpenGL()
{
  Cleanup();
}

bool CRendererClientOpenGL::Initialize()
{
  if (m_initialized)
    return true;

  uint32_t color = m_client->GetBackgroundColorARGB();
  m_backgroundColor[3] = float(CefColorGetA(color)) / 255.0f;
  m_backgroundColor[2] = float(CefColorGetR(color)) / 255.0f;
  m_backgroundColor[1] = float(CefColorGetG(color)) / 255.0f;
  m_backgroundColor[0] = float(CefColorGetB(color)) / 255.0f;

  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); VERIFY_NO_ERROR;

  m_useTransparentBackground = m_client->UseTransparentBackground();
  if (m_useTransparentBackground)
  {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); VERIFY_NO_ERROR;
  }
  else
  {
    glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]); VERIFY_NO_ERROR;
  }

  // Necessary for non-power-of-2 textures to render correctly.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); VERIFY_NO_ERROR;

  // Create the texture.
  glGenTextures(1, &m_textureId); VERIFY_NO_ERROR;
  DCHECK_NE(m_textureId, 0U); VERIFY_NO_ERROR;

  glBindTexture(GL_TEXTURE_2D, m_textureId); VERIFY_NO_ERROR;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); VERIFY_NO_ERROR;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); VERIFY_NO_ERROR;
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); VERIFY_NO_ERROR;

  m_initialized = true;
  return true;
}

void CRendererClientOpenGL::Cleanup()
{
  if (m_textureId != 0)
    glDeleteTextures(1, &m_textureId);
}

void CRendererClientOpenGL::Render()
{
  if (m_viewWidth == 0 || m_viewHeight == 0)
    return;

  DCHECK(m_initialized);

  struct {
    float tu, tv;
    float x, y, z;
  } static vertices[] = {
    {0.0f, 1.0f, -1.0f, -1.0f, 0.0f},
    {1.0f, 1.0f,  1.0f, -1.0f, 0.0f},
    {1.0f, 0.0f,  1.0f,  1.0f, 0.0f},
    {0.0f, 0.0f, -1.0f,  1.0f, 0.0f}
  };

  glMatrixMode(GL_MODELVIEW); VERIFY_NO_ERROR;
  glPushMatrix ();
  glLoadIdentity(); VERIFY_NO_ERROR;

  glMatrixMode(GL_PROJECTION); VERIFY_NO_ERROR;
  glPushMatrix ();
  glLoadIdentity(); VERIFY_NO_ERROR;

  if (!m_useTransparentBackground)
  {
    // Draw the background gradient.
    glPushAttrib(GL_ALL_ATTRIB_BITS); VERIFY_NO_ERROR;
    // Don't check for errors until glEnd().
    glBegin(GL_QUADS);
    glColor4f(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd(); VERIFY_NO_ERROR;
    glPopAttrib(); VERIFY_NO_ERROR;
  }

  // Rotate the view based on the mouse spin.
  if (m_fSpinX != 0)
  {
    glRotatef(-m_fSpinX, 1.0f, 0.0f, 0.0f); VERIFY_NO_ERROR;
  }
  if (m_fSpinY != 0)
  {
    glRotatef(-m_fSpinY, 0.0f, 1.0f, 0.0f); VERIFY_NO_ERROR;
  }

  if (m_useTransparentBackground)
  {
    // Enable alpha blending.
    glEnable(GL_BLEND); VERIFY_NO_ERROR;

    // Alpha blending style. Texture values have premultiplied alpha.
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); VERIFY_NO_ERROR;
    glColor4f(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
  }

  //! Enable 2D textures.
  glEnable(GL_TEXTURE_2D); VERIFY_NO_ERROR;

  //! Draw the facets with the texture.
  DCHECK_NE(m_textureId, 0U); VERIFY_NO_ERROR;
  glBindTexture(GL_TEXTURE_2D, m_textureId); VERIFY_NO_ERROR;
  glInterleavedArrays(GL_T2F_V3F, 0, vertices); VERIFY_NO_ERROR;
  glDrawArrays(GL_QUADS, 0, 4); VERIFY_NO_ERROR;

  // Disable 2D textures.
  glDisable(GL_TEXTURE_2D); VERIFY_NO_ERROR;

  //! Disable alpha blending if needed.
  if (m_useTransparentBackground)
  {
    glDisable(GL_BLEND);
    VERIFY_NO_ERROR;
  }

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

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    VERIFY_NO_ERROR
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
}

bool CRendererClientOpenGL::Dirty()
{
  if (!m_initialized && !Initialize())
    return false;

  // Do the paint job if data present
  if (!m_paintQueue.empty())
  {
    CPaintMessage* msg = m_paintQueue.front();
    m_paintQueue.pop();

    CefRefPtr<CefBrowser> browser = msg->browser;
    PaintElementType type = msg->type;
    const RectList& dirtyRects = msg->dirtyRects;
    const void* buffer = msg->buffer;
    int width = msg->width;
    int height = msg->height;

    if (m_useTransparentBackground) // Enable alpha blending if true.
    {
      glEnable(GL_BLEND); VERIFY_NO_ERROR;
    }

    // Enable 2D textures.
    glEnable(GL_TEXTURE_2D);
    VERIFY_NO_ERROR;

    DCHECK_NE(m_textureId, 0U);
    glBindTexture(GL_TEXTURE_2D, m_textureId); VERIFY_NO_ERROR;

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
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, m_viewWidth, m_viewHeight, 0,
            GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer); VERIFY_NO_ERROR;
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
          glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width,
                          rect.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                          buffer); VERIFY_NO_ERROR;
        }
      }
      glPopClientAttrib();
    }
    else if (type == PET_POPUP && m_popupRect.width > 0 &&
            m_popupRect.height > 0)
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

    // Disable 2D textures.
    glDisable(GL_TEXTURE_2D); VERIFY_NO_ERROR;

    // Disable alpha blending.
    if (m_useTransparentBackground)
      glDisable(GL_BLEND); VERIFY_NO_ERROR;

    m_dirty = true;
    delete msg;
  }

  bool ret = m_dirty;
  m_dirty = false;
  return ret;
}

bool CRendererClientOpenGL::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  CEF_REQUIRE_UI_THREAD();

  // The simulated screen and view rectangle are the same. This is necessary
  // for popup menus to be located and sized inside the view.
  rect.x = rect.y = 0;
  rect.width = m_client->GetWidth();
  rect.height = m_client->GetHeight();
  return true;
}

bool CRendererClientOpenGL::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  CEF_REQUIRE_UI_THREAD();

  screenX = m_client->GetXPos() + viewX;
  screenY = m_client->GetYPos() + viewY;
  return true;
}

void CRendererClientOpenGL::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                                const RectList& dirtyRects, const void* buffer,
                                int width, int height)
{
  if (width <= 2 && height <= 2)
  {
    // Ignore really small buffer sizes while the widget is starting up.
    return;
  }

  if (m_paintQueue.size() > MAX_MESSAGE_QUEUE_FILL_SIZE)
  {
    kodi::Log(ADDON_LOG_ERROR, "Renderer paint message flow reached max size, further calls becomes ignored!");
    return;
  }

  CPaintMessage* message = new CPaintMessage;
  message->thisClass = this;
  message->browser = browser;
  message->type = type;
  message->dirtyRects = dirtyRects;
  message->buffer = buffer;
  message->width = width;
  message->height = height;
  m_paintQueue.push(message);
}
