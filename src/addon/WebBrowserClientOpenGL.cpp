/*
 *      Copyright (C) 2015 Team KODI
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

#include "addon.h"
#include "Utils.h"
#include "WebBrowserClientOpenGL.h"

using namespace std;

#if defined(OS_MACOSX)
#include <OpenGL/gl.h>
#elif defined(OS_LINUX)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#error Platform is not supported.
#endif

#include "include/wrapper/cef_helpers.h"

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

CWebBrowserClientOpenGL::CWebBrowserClientOpenGL(int iUniqueClientId, const WEB_ADDON_GUI_PROPS *props, kodi::addon::CInstanceWeb* instance) :
  CWebBrowserClientBase(iUniqueClientId, props, instance),
  m_bInitialized(false),
  m_iTextureId(0),
  m_iViewWidth(0),
  m_iViewHeight(0),
  m_fSpinX(0),
  m_fSpinY(0)
{
}

CWebBrowserClientOpenGL::~CWebBrowserClientOpenGL()
{
  Cleanup();
}

bool CWebBrowserClientOpenGL::Initialize()
{
  if (m_bInitialized)
    return true;

  m_bInitialized = CWebBrowserClientBase::Initialize();
  if (!m_bInitialized)
    return false;

  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); VERIFY_NO_ERROR;

  glClearColor(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], m_BackgroundColor[3]); VERIFY_NO_ERROR;

  // Necessary for non-power-of-2 textures to render correctly.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); VERIFY_NO_ERROR;

  // Create the texture.
  glGenTextures(1, &m_iTextureId); VERIFY_NO_ERROR;
  DCHECK_NE(m_iTextureId, 0U); VERIFY_NO_ERROR;

  glBindTexture(GL_TEXTURE_2D, m_iTextureId); VERIFY_NO_ERROR;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); VERIFY_NO_ERROR;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); VERIFY_NO_ERROR;
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); VERIFY_NO_ERROR;

  return true;
}

bool CWebBrowserClientOpenGL::OpenWebsite(const char* strURL, bool single, bool allowMenus)
{
  Cleanup();
  bool ret = CWebBrowserClientBase::OpenWebsite(strURL, single, allowMenus);
  m_bInitialized = false;

  return ret;
}

void CWebBrowserClientOpenGL::Cleanup()
{
  P8PLATFORM::CLockObject lock(m_Mutex);
  if (m_iTextureId != 0)
    glDeleteTextures(1, &m_iTextureId);
}

void CWebBrowserClientOpenGL::Render()
{
  P8PLATFORM::CLockObject lock(m_Mutex);

  if (m_iViewWidth == 0 || m_iViewHeight == 0)
    return;

  DCHECK(m_bInitialized);

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

  if (!m_bTransparentBackground)
  {
    // Draw the background gradient.
    glPushAttrib(GL_ALL_ATTRIB_BITS); VERIFY_NO_ERROR;
    // Don't check for errors until glEnd().
    glBegin(GL_QUADS);
    glColor4f(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], m_BackgroundColor[3]);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd(); VERIFY_NO_ERROR;
    glPopAttrib(); VERIFY_NO_ERROR;
  }

  // Rotate the view based on the mouse spin.
  if (m_fSpinX != 0)
    glRotatef(-m_fSpinX, 1.0f, 0.0f, 0.0f); VERIFY_NO_ERROR;
  if (m_fSpinY != 0)
    glRotatef(-m_fSpinY, 0.0f, 1.0f, 0.0f); VERIFY_NO_ERROR;

  if (m_bTransparentBackground)
  {
    // Enable alpha blending.
    glEnable(GL_BLEND); VERIFY_NO_ERROR;

    // Alpha blending style. Texture values have premultiplied alpha.
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); VERIFY_NO_ERROR;
    glColor4f(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], m_BackgroundColor[3]);
  }

  //! Enable 2D textures.
  glEnable(GL_TEXTURE_2D); VERIFY_NO_ERROR;

  //! Draw the facets with the texture.
  DCHECK_NE(m_iTextureId, 0U); VERIFY_NO_ERROR;
  glBindTexture(GL_TEXTURE_2D, m_iTextureId); VERIFY_NO_ERROR;
  glInterleavedArrays(GL_T2F_V3F, 0, vertices); VERIFY_NO_ERROR;
  glDrawArrays(GL_QUADS, 0, 4); VERIFY_NO_ERROR;

  // Disable 2D textures.
  glDisable(GL_TEXTURE_2D); VERIFY_NO_ERROR;

  //! Disable alpha blending if needed.
  if (m_bTransparentBackground)
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
    glOrtho(0, m_iViewWidth, m_iViewHeight, 0, 0, 1); VERIFY_NO_ERROR;

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

  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void CWebBrowserClientOpenGL::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  CWebBrowserClientBase::OnAfterCreated(browser);
}

void CWebBrowserClientOpenGL::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  CWebBrowserClientBase::OnBeforeClose(browser);
}

bool CWebBrowserClientOpenGL::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  P8PLATFORM::CLockObject lock(m_Mutex);
  CEF_REQUIRE_UI_THREAD();

  // The simulated screen and view rectangle are the same. This is necessary
  // for popup menus to be located and sized inside the view.
  rect.x = rect.y = 0;
  rect.width = m_iWidth;
  rect.height = m_iHeight;
  return true;
}

bool CWebBrowserClientOpenGL::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  P8PLATFORM::CLockObject lock(m_Mutex);
  CEF_REQUIRE_UI_THREAD();

  screenX = m_iXPos + viewX;
  screenY = m_iYPos + viewY;
  return true;
}

void CWebBrowserClientOpenGL::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                                const RectList& dirtyRects, const void* buffer,
                                int width, int height)
{
  m_onPaintMessage.client.callback = &OnPaint;
  m_onPaintMessage.client.userptr  = this;
  m_onPaintMessage.browser    = browser;
  m_onPaintMessage.type       = type;
  m_onPaintMessage.dirtyRects = &dirtyRects;
  m_onPaintMessage.buffer     = buffer;
  m_onPaintMessage.width      = width;
  m_onPaintMessage.height     = height;

  Message tMsg = {TMSG_HANDLE_ON_PAINT};
  tMsg.lpVoid = &m_onPaintMessage;
  SendMessage(tMsg, true);
}

void CWebBrowserClientOpenGL::OnPaint(void *msg)
{
  OnPaintMessage *data                = static_cast<OnPaintMessage*>(msg);
  CWebBrowserClientOpenGL* thisClass  = static_cast<CWebBrowserClientOpenGL*>(data->client.userptr);

  CefRefPtr<CefBrowser> browser = data->browser;
  PaintElementType type         = data->type;
  const RectList& dirtyRects    = data->dirtyRects[0];
  const void* buffer            = data->buffer;
  int width                     = data->width;
  int height                    = data->height;


  if (!thisClass->m_bInitialized)
    thisClass->Initialize();

  if (thisClass->m_bTransparentBackground) // Enable alpha blending if true.
    glEnable(GL_BLEND); VERIFY_NO_ERROR;

  // Enable 2D textures.
  glEnable(GL_TEXTURE_2D); VERIFY_NO_ERROR;

  DCHECK_NE(thisClass->m_iTextureId, 0U);
  glBindTexture(GL_TEXTURE_2D, thisClass->m_iTextureId); VERIFY_NO_ERROR;

  if (type == PET_VIEW)
  {
    int old_width = thisClass->m_iViewWidth;
    int old_height = thisClass->m_iViewHeight;

    thisClass->m_iViewWidth = width;
    thisClass->m_iViewHeight = height;

#ifdef SHOW_UPDATE_RECT
    thisClass->m_updateRect = dirtyRects[0];
#endif // SHOW_UPDATE_RECT

    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, thisClass->m_iViewWidth); VERIFY_NO_ERROR;

    if (old_width != thisClass->m_iViewWidth || old_height != thisClass->m_iViewHeight ||
        (dirtyRects.size() == 1 &&
         dirtyRects[0] == CefRect(0, 0, thisClass->m_iViewWidth, thisClass->m_iViewHeight)))
    {
      // Update/resize the whole texture.
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0); VERIFY_NO_ERROR;
      glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); VERIFY_NO_ERROR;
      glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGBA, thisClass->m_iViewWidth, thisClass->m_iViewHeight, 0,
          GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer); VERIFY_NO_ERROR;
    }
    else
    {
      // Update just the dirty rectangles.
      CefRenderHandler::RectList::const_iterator i = dirtyRects.begin();
      for (; i != dirtyRects.end(); ++i)
      {
        const CefRect& rect = *i;
        DCHECK(rect.x + rect.width <= thisClass->m_iViewWidth);
        DCHECK(rect.y + rect.height <= thisClass->m_iViewHeight);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, rect.x); VERIFY_NO_ERROR;
        glPixelStorei(GL_UNPACK_SKIP_ROWS, rect.y); VERIFY_NO_ERROR;
        glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width,
                        rect.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                        buffer); VERIFY_NO_ERROR;
      }
    }
    glPopClientAttrib();
  }
  else if (type == PET_POPUP && thisClass->m_popupRect.width > 0 &&
           thisClass->m_popupRect.height > 0)
  {
    int skip_pixels = 0, x = thisClass->m_popupRect.x;
    int skip_rows = 0, y = thisClass->m_popupRect.y;
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
    if (x + w > thisClass->m_iViewWidth)
      w -= x + w - thisClass->m_iViewWidth;
    if (y + h > thisClass->m_iViewHeight)
      h -= y + h - thisClass->m_iViewHeight;

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
  if (thisClass->m_bTransparentBackground)
    glDisable(GL_BLEND); VERIFY_NO_ERROR;
}
