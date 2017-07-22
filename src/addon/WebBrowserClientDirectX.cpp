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

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "addon.h"
#include "WebBrowserClientDirectX.h"
#include "Utils.h"

using namespace std;
using namespace ADDON;

CWebBrowserClientDirectX::CWebBrowserClientDirectX(int iUniqueClientId, const WEB_ADDON_GUI_PROPS *props, kodi::addon::CInstanceWeb* instance) :
  CWebBrowserClientBase(iUniqueClientId, props, instance)
{
}

CWebBrowserClientDirectX::~CWebBrowserClientDirectX()
{
  Cleanup();
}

bool CWebBrowserClientDirectX::Initialize()
{
  return false;
}

bool CWebBrowserClientDirectX::OpenWebsite(const char* strURL, bool single, bool allowMenus)
{
  return CWebBrowserClientBase::OpenWebsite(strURL, single, allowMenus);
}

void CWebBrowserClientDirectX::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  CWebBrowserClientBase::OnAfterCreated(browser);
}

void CWebBrowserClientDirectX::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  CWebBrowserClientBase::OnBeforeClose(browser);
}

void CWebBrowserClientDirectX::Cleanup()
{
}

bool CWebBrowserClientDirectX::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
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

bool CWebBrowserClientDirectX::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  P8PLATFORM::CLockObject lock(m_Mutex);
  CEF_REQUIRE_UI_THREAD();

  rect.x = rect.y = 0;
  rect.width = m_iWidth;
  rect.height = m_iHeight;
  return true;
}

bool CWebBrowserClientDirectX::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  P8PLATFORM::CLockObject lock(m_Mutex);
  CEF_REQUIRE_UI_THREAD();

  screenX = m_iXPos + viewX;
  screenY = m_iYPos + viewY;
  return true;
}

bool CWebBrowserClientDirectX::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
{
  CEF_REQUIRE_UI_THREAD();
  return false;
}

void CWebBrowserClientDirectX::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                                const RectList& dirtyRects, const void* buffer,
                                int width, int height)
{
  /*!
   * Do not know if windows DirectX need also rendering performed on same thread,
   * if no this and the function below can be removed,
   */
  sPaintData data;
  data.client = this;
  data.browser = browser;
  data.type = type;
  data.dirtyRects = &dirtyRects;
  data.buffer = buffer;
  data.width = width;
  data.height = height;
  data.function = OnPaint;
  m_processQueueMutex.Lock();
  m_processQueue.push(&data);
  m_processQueueMutex.Unlock();
  if (!data.event.Wait(1000))
    LOG_MESSAGE(LOG_ERROR, "%s - Event signal not processed!", __FUNCTION__);
}

void CWebBrowserClientDirectX::OnPaint(sPaintData *data)
{

  data->event.Signal();
}

void CWebBrowserClientDirectX::Render()
{
  P8PLATFORM::CLockObject lock(m_Mutex);

  if (m_iViewWidth == 0 || m_iViewHeight == 0)
    return;

}

bool CWebBrowserClientDirectX::Dirty()
{
  bool ret = false;

  /*!
   * Handle from chromium given data, must be done on kodi's renderer thread (also on DirectX???),
   * thats why passed here
   */
  P8PLATFORM::CLockObject lock(m_Mutex);

  m_processQueueMutex.Lock();
  while (!m_processQueue.empty())
  {
    sPaintData *queueData = m_processQueue.front();
    m_processQueue.pop();
    queueData->function(queueData);
    queueData->event.Signal();
    ret = true;
  }
  m_processQueueMutex.Unlock();

  return ret;
}
