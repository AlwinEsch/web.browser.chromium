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

#include <string>

#include "addon.h"
#include <kodi/addon-instance/Web.h>
#include "p8-platform/util/util.h"
#include "p8-platform/util/StdString.h"

#include "WebBrowserManager.h"
#include "Utils.h"

using namespace std;

/* User adjustable settings are saved here.
 * Default values are defined inside addon.h
 * and exported to the other source files.
 */
std::string               g_strTempPath       = "";
std::string               g_strLogPath        = "";
std::string               g_strUserPath       = "";
std::string               g_strAddonLibPath   = "";
std::string               g_strAddonSharePath = "";
CWebBrowserManager       *g_pWebManager       = NULL;

class CWebBrowser : public kodi::addon::CInstanceWeb
{
public:
  CWebBrowser(KODI_HANDLE instance);
  virtual ~CWebBrowser();

  virtual WEB_ADDON_ERROR GetCapabilities(WEB_ADDON_CAPABILITIES *pCapabilities) override;
  virtual bool SetLanguage(const char *language) override;
  virtual WEB_ADDON_ERROR GetVariousTypes(const WEB_ADDON_VARIOUS_TYPE *prevType, WEB_ADDON_VARIOUS_TYPE *type) override;
  virtual WEB_ADDON_ERROR CreateControl(const WEB_ADDON_GUI_PROPS &props, const char *webType, ADDON_HANDLE handle) override;
  virtual bool DestroyControl(const ADDON_HANDLE handle) override;
  virtual void Render(const ADDON_HANDLE handle) override;
  virtual void Stop(const ADDON_HANDLE handle) override;
  virtual bool Dirty(const ADDON_HANDLE handle) override;
  virtual bool OnInit(const ADDON_HANDLE handle) override;
  virtual bool OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem) override;
  virtual bool OnMouseEvent(const ADDON_HANDLE handle, int id, double x, double y, double offsetX, double offsetY, int state) override;
  virtual bool OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus) override;
  virtual void CallSingleCommand(const ADDON_HANDLE handle, WEB_ADDON_SINGLE_COMMANDS command) override;
};

CWebBrowser::CWebBrowser(KODI_HANDLE instance)
  : CInstanceWeb(instance)
{
  kodi::Log(ADDON_LOG_DEBUG, "%s - Creating the Google Chromium Internet Browser add-on", __FUNCTION__);

  g_strTempPath         = TempPath();
  g_strLogPath          = LogPath();
  g_strUserPath         = UserPath();
  g_strAddonLibPath     = AddonLibPath();
  g_strAddonSharePath   = AddonSharePath();
  g_pWebManager         = new CWebBrowserManager(this);

  if (!g_pWebManager->Create())
  {
    kodi::Log(ADDON_LOG_DEBUG, "%s - Creation of web manager failed", __FUNCTION__);
    return;
  }

  g_pWebManager->LoadUserSettings(); //!< @todo
  fprintf(stderr, "--------------xxxxxxxxxxxxxxxxxxxxxxxxxxxxx------------> %s\n", __func__);
}

CWebBrowser::~CWebBrowser()
{
  fprintf(stderr, "-------------yyyyyyyyyyyyyyy-------------> %s\n", __func__);
  g_pWebManager->SaveUserSettings(); //!< @todo

  g_pWebManager->Destroy();

  SAFE_DELETE(g_pWebManager);
}

WEB_ADDON_ERROR CWebBrowser::GetCapabilities(WEB_ADDON_CAPABILITIES *pCapabilities)
{
  pCapabilities->bSupportsWeb       = true;
  pCapabilities->bSupportsMail      = false;
  pCapabilities->bSupportsMessenger = false;
  pCapabilities->bSupportsVarious   = false;
  return WEB_ADDON_ERROR_NO_ERROR;
}

WEB_ADDON_ERROR CWebBrowser::GetVariousTypes(const WEB_ADDON_VARIOUS_TYPE *prevType, WEB_ADDON_VARIOUS_TYPE *type)
{
  if (prevType == NULL)
  {
    type->strName          = "KODI Chromium Web Browser";
    type->iAddonInternalId = 1;
    type->iType            = WEB_TYPE_ID_BROWSER;
    return WEB_ADDON_ERROR_NO_ERROR;
  }
  return WEB_ADDON_ERROR_REJECTED;
}

/*
//! @todo add to addon interface
WEB_ADDON_ERROR CWebBrowser::LoadUserSettings()
{
  if (g_pWebManager->LoadUserSettings())
    return WEB_ADDON_ERROR_NO_ERROR;
  return WEB_ADDON_ERROR_FAILED;
}

//! @todo add to addon interface
WEB_ADDON_ERROR CWebBrowser::SaveUserSettings()
{
  if (g_pWebManager->SaveUserSettings())
    return WEB_ADDON_ERROR_NO_ERROR;
  return WEB_ADDON_ERROR_FAILED;
}
*/

WEB_ADDON_ERROR CWebBrowser::CreateControl(const WEB_ADDON_GUI_PROPS &props, const char *webType, ADDON_HANDLE handle)
{
  return g_pWebManager->CreateControl(props, webType, handle);
}

bool CWebBrowser::DestroyControl(const ADDON_HANDLE handle)
{
  return g_pWebManager->DestroyControl(handle);
}

bool CWebBrowser::SetLanguage(const char *language)
{
  return g_pWebManager->SetLanguage(language);
}

void CWebBrowser::Render(const ADDON_HANDLE handle)
{
  g_pWebManager->Render(handle);
}

void CWebBrowser::Stop(const ADDON_HANDLE handle)
{
  g_pWebManager->Stop(handle);
}

bool CWebBrowser::Dirty(const ADDON_HANDLE handle)
{
  return g_pWebManager->Dirty(handle);
}

bool CWebBrowser::OnInit(const ADDON_HANDLE handle)
{
  return g_pWebManager->OnInit(handle);
}

bool CWebBrowser::OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem)
{
  return g_pWebManager->OnAction(handle, actionId, nextItem);
}

bool CWebBrowser::OnMouseEvent(const ADDON_HANDLE handle, int id, double x, double y, double offsetX, double offsetY, int state)
{
  return g_pWebManager->OnMouseEvent(handle, id, x, y, offsetX, offsetY, state);
}

bool CWebBrowser::OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus)
{
  return g_pWebManager->OpenWebsite(handle, strURL, single, allowMenus);
}

void CWebBrowser::CallSingleCommand(const ADDON_HANDLE handle, WEB_ADDON_SINGLE_COMMANDS command)
{
  g_pWebManager->CallSingleCommand(handle, command);
}


class CMyAddon : public kodi::addon::CAddonBase
{
public:
  CMyAddon() { }
  virtual ADDON_STATUS CreateInstance(int instanceType, std::string instanceID, KODI_HANDLE instance, KODI_HANDLE& addonInstance) override
  {
    addonInstance = new CWebBrowser(instance);
    return ADDON_STATUS_OK;
  }
};

ADDONCREATOR(CMyAddon)
