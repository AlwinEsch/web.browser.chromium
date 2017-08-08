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

#include "RendererOpenGLES.h"

#include "addon.h"
#include "Utils.h"
#include "WebBrowserClient.h"

#include "include/wrapper/cef_helpers.h"

#include <p8-platform/threads/threads.h>

using namespace P8PLATFORM;

#define MAX_MESSAGE_QUEUE_FILL_SIZE 100

CRendererClientOpenGLES::CRendererClientOpenGLES(CWebBrowserClient* client) :
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

CRendererClientOpenGLES::~CRendererClientOpenGLES()
{
  Cleanup();
}

bool CRendererClientOpenGLES::Initialize()
{
  if (m_initialized)
    return true;

  uint32_t color = m_client->GetBackgroundColorARGB();
  m_backgroundColor[3] = float(CefColorGetA(color)) / 255.0f;
  m_backgroundColor[2] = float(CefColorGetR(color)) / 255.0f;
  m_backgroundColor[1] = float(CefColorGetG(color)) / 255.0f;
  m_backgroundColor[0] = float(CefColorGetB(color)) / 255.0f;

  m_useTransparentBackground = m_client->UseTransparentBackground();

  m_initialized = true;
  return true;
}

void CRendererClientOpenGLES::Cleanup()
{

}

void CRendererClientOpenGLES::Render()
{
  if (m_viewWidth == 0 || m_viewHeight == 0)
    return;

  DCHECK(m_initialized);

}

bool CRendererClientOpenGLES::Dirty()
{
  if (!m_initialized)
    Initialize();

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


    m_dirty = true;
    delete msg;
  }

  bool ret = m_dirty;
  m_dirty = false;
  return ret;
}

bool CRendererClientOpenGLES::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  CEF_REQUIRE_UI_THREAD();

  // The simulated screen and view rectangle are the same. This is necessary
  // for popup menus to be located and sized inside the view.
  rect.x = rect.y = 0;
  rect.width = m_client->GetWidth();
  rect.height = m_client->GetHeight();
  return true;
}

bool CRendererClientOpenGLES::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  CEF_REQUIRE_UI_THREAD();

  screenX = m_client->GetXPos() + viewX;
  screenY = m_client->GetYPos() + viewY;
  return true;
}

void CRendererClientOpenGLES::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
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
