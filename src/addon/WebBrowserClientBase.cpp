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

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "addon.h"
#include "WebBrowserClientBase.h"
#include "Utils.h"

using namespace std;
using namespace ADDON;

CWebBrowserClientBase::CWebBrowserClientBase(int iUniqueClientId, const WEB_ADDON_GUI_PROPS *props) :
  m_iUniqueClientId(iUniqueClientId),
  m_Browser(NULL),
  m_pDevice(props->pDevice),
  m_iXPos(props->iXPos),
  m_iYPos(props->iYPos),
  m_iWidth(props->iWidth),
  m_iHeight(props->iHeight),
  m_fPixelRatio(props->fPixelRatio),
  m_strIdName(props->strName),
  m_iLeaveOpenTime(props->iLeaveOpenTime),
  m_iGUIItemLeft(props->iGUIItemLeft),
  m_iGUIItemRight(props->iGUIItemRight),
  m_iGUIItemTop(props->iGUIItemTop),
  m_iGUIItemBottom(props->iGUIItemBottom),
  m_iGUIItemBack(props->iGUIItemBack),
  m_bTransparentBackground(props->bUseTransparentBackground)
{
  m_BackgroundColor[3] = float(CefColorGetA(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[2] = float(CefColorGetR(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[1] = float(CefColorGetG(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[0] = float(CefColorGetB(props->iBackgroundColorARGB)) / 255.0f;
}

CWebBrowserClientBase::~CWebBrowserClientBase()
{
}

bool CWebBrowserClientBase::SetInactive()
{
  m_inactivateTime = time(NULL);
  return m_iLeaveOpenTime < 0;
}

bool CWebBrowserClientBase::SetActive()
{
  return true;
}

int CWebBrowserClientBase::CurrentInactiveCountdown()
{
  return time(NULL) - m_inactivateTime + m_iLeaveOpenTime;
}

bool CWebBrowserClientBase::Initialize()
{
  return true;
}

bool CWebBrowserClientBase::OpenWebsite(const char* strURL, bool single, bool allowMenus)
{
  if (!m_Browser.get())
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Called without present browser", __FUNCTION__);
    return false;
  }

  m_Browser->GetMainFrame()->LoadURL(strURL);
  m_Browser->Reload();
  return true;
}

void CWebBrowserClientBase::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  if (!m_Browser)
  {
    m_Browser = browser;
  }
}

void CWebBrowserClientBase::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
}
