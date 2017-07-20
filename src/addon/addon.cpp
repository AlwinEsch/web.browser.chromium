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
#include "kodi_web_dll.h"
#include "p8-platform/util/util.h"
#include "p8-platform/util/StdString.h"

#include "WebBrowserManager.h"
#include "Utils.h"

using namespace std;
using namespace ADDON;

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
CHelper_libXBMC_addon    *KODI                = NULL;
CHelper_libKODI_web      *WEB                 = NULL;
CHelper_libKODI_guilib   *GUI                 = NULL;
ADDON_STATUS              m_CurStatus         = ADDON_STATUS_UNKNOWN;

extern "C" {

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!hdl || !props)
    return ADDON_STATUS_UNKNOWN;

  AddonProps_WebAddon* webProps = (AddonProps_WebAddon*)props;
  fprintf(stderr, "-m_struct.propsllllllllllllll----------------.strAddonSharePath %s\n", webProps->strAddonSharePath);
  KODI = new CHelper_libXBMC_addon;
  if (!KODI->RegisterMe(hdl))
  {
    SAFE_DELETE(KODI);
    KODI->Log(LOG_DEBUG, "%s - Failed to register libKODI_addon", __FUNCTION__);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  GUI = new CHelper_libKODI_guilib;
  if (!GUI->RegisterMe(hdl))
  {
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    KODI->Log(LOG_DEBUG, "%s - Failed to register libKODI_guilib", __FUNCTION__);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  WEB = new CHelper_libKODI_web;
  if (!WEB->RegisterMe(hdl))
  {
    SAFE_DELETE(WEB);
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    KODI->Log(LOG_DEBUG, "%s - Failed to register libKODI_web", __FUNCTION__);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  KODI->Log(LOG_DEBUG, "%s - Creating the Google Chromium Internet Browser add-on", __FUNCTION__);

  m_CurStatus           = ADDON_STATUS_UNKNOWN;
  g_strTempPath         = webProps->strTempPath;
  g_strLogPath          = webProps->strLogPath;
  g_strUserPath         = webProps->strUserPath;
  g_strAddonLibPath     = webProps->strAddonLibPath;
  g_strAddonSharePath   = webProps->strAddonSharePath;
  g_pWebManager         = new CWebBrowserManager;

  if (!g_pWebManager->Create())
  {
    KODI->Log(LOG_DEBUG, "%s - Creation of web manager failed", __FUNCTION__);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  g_pWebManager->LoadUserSettings(); //!< @todo

  m_CurStatus = ADDON_STATUS_OK;

  return m_CurStatus;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
  g_pWebManager->SaveUserSettings(); //!< @todo

  g_pWebManager->Destroy();

  SAFE_DELETE(g_pWebManager);
  SAFE_DELETE(WEB);
  SAFE_DELETE(GUI);
  SAFE_DELETE(KODI);

  m_CurStatus = ADDON_STATUS_UNKNOWN;
}

ADDON_STATUS ADDON_SetSetting(const char *settingName, const void *settingValue)
{
  return ADDON_STATUS_OK;
}

const char* GetWebAddonName(void)
{
  return "KODIChromiumBrowser";
}

WEB_ADDON_ERROR GetAddonCapabilities(WEB_ADDON_CAPABILITIES *pCapabilities)
{
  pCapabilities->bSupportsWeb       = true;
  pCapabilities->bSupportsMail      = false;
  pCapabilities->bSupportsMessenger = false;
  pCapabilities->bSupportsVarious   = false;
  return WEB_ADDON_ERROR_NO_ERROR;
}

WEB_ADDON_ERROR GetVariousTypes(const WEB_ADDON_VARIOUS_TYPE *prevType, WEB_ADDON_VARIOUS_TYPE *type)
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

//! @todo add to addon interface
WEB_ADDON_ERROR LoadUserSettings()
{
  if (g_pWebManager->LoadUserSettings())
    return WEB_ADDON_ERROR_NO_ERROR;
  return WEB_ADDON_ERROR_FAILED;
}

//! @todo add to addon interface
WEB_ADDON_ERROR SaveUserSettings()
{
  if (g_pWebManager->SaveUserSettings())
    return WEB_ADDON_ERROR_NO_ERROR;
  return WEB_ADDON_ERROR_FAILED;
}

WEB_ADDON_ERROR CreateControl(const WEB_ADDON_GUI_PROPS &props, const char *webType, ADDON_HANDLE handle)
{
  return g_pWebManager->CreateControl(props, webType, handle);
}

bool DestroyControl(const ADDON_HANDLE handle)
{
  return g_pWebManager->DestroyControl(handle);
}

bool SetLanguage(const char *language)
{
  return g_pWebManager->SetLanguage(language);
}

void Render(const ADDON_HANDLE handle)
{
  g_pWebManager->Render(handle);
}

void Stop(const ADDON_HANDLE handle)
{
  g_pWebManager->Stop(handle);
}

bool Dirty(const ADDON_HANDLE handle)
{
  return g_pWebManager->Dirty(handle);
}

bool OnInit(const ADDON_HANDLE handle)
{
  return g_pWebManager->OnInit(handle);
}

bool OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem)
{
  return g_pWebManager->OnAction(handle, actionId, nextItem);
}

bool OnMouseEvent(const ADDON_HANDLE handle, int id, double x, double y, double offsetX, double offsetY, int state)
{
  return g_pWebManager->OnMouseEvent(handle, id, x, y, offsetX, offsetY, state);
}

bool OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus)
{
  return g_pWebManager->OpenWebsite(handle, strURL, single, allowMenus);
}

void CallSingleCommand(const ADDON_HANDLE handle, WEB_ADDON_SINGLE_COMMANDS command)
{
  g_pWebManager->CallSingleCommand(handle, command);
}

} // extern "C"
