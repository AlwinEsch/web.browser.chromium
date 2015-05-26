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

#include "kodi/kodi_web_dll.h"
#include "platform/util/util.h"
#include "platform/util/StdString.h"

#include "addon.h"
#include "Utils.h"

using namespace std;
using namespace ADDON;

/* User adjustable settings are saved here.
 * Default values are defined inside addon.h
 * and exported to the other source files.
 */
std::string               g_strUserPath       = "";
std::string               g_strAddonPath      = "";
CHelper_libXBMC_addon    *KODI                = NULL;
CHelper_libKODI_web      *WEB                 = NULL;
CHelper_libKODI_guilib   *GUI                 = NULL;
ADDON_STATUS              m_CurStatus         = ADDON_STATUS_UNKNOWN;

extern "C" {

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!hdl || !props)
    return ADDON_STATUS_UNKNOWN;

  WEB_ADDON_PROPERTIES* webProps = (WEB_ADDON_PROPERTIES*)props;

  KODI = new CHelper_libXBMC_addon;
  if (!KODI->RegisterMe(hdl))
  {
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  GUI = new CHelper_libKODI_guilib;
  if (!GUI->RegisterMe(hdl))
  {
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  WEB = new CHelper_libKODI_web;
  if (!WEB->RegisterMe(hdl))
  {
    SAFE_DELETE(WEB);
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  LOG_MESSAGE(LOG_DEBUG, "%s - Creating the Google Chromium Internet Browser add-on", __FUNCTION__);

  m_CurStatus     = ADDON_STATUS_UNKNOWN;
  g_strUserPath   = webProps->strUserPath;
  g_strAddonPath  = webProps->strAddonPath;

  m_CurStatus = ADDON_STATUS_OK;

  return m_CurStatus;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
  SAFE_DELETE(WEB);
  SAFE_DELETE(GUI);
  SAFE_DELETE(KODI);

  m_CurStatus = ADDON_STATUS_UNKNOWN;
}

bool ADDON_HasSettings()
{
  return true;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *settingName, const void *settingValue)
{
  return ADDON_STATUS_OK;
}

void ADDON_Stop()
{
}

void ADDON_FreeSettings()
{

}

void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
}

const char* GetWebAddonName(void)
{
  return "KODIChromiumBrowser";
}

const char* GetWebAPIVersion(void)
{
  return KODI_WEB_ADDON_API_VERSION;
}

const char* GetMinimumWebAPIVersion(void)
{
  return KODI_WEB_ADDON_MIN_API_VERSION;
}

const char* GetGUIAPIVersion(void)
{
  return KODI_GUILIB_API_VERSION;
}

const char* GetMinimumGUIAPIVersion(void)
{
  return KODI_GUILIB_MIN_API_VERSION;
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

WEB_ADDON_ERROR CreateControl(const WEB_ADDON_GUI_PROPS &props, unsigned int webType, ADDON_HANDLE handle)
{
  return true;
}

bool DestroyControl(const ADDON_HANDLE handle)
{
  return true;
}

bool SetLanguage(const char *language)
{
  return true;
}

void Render(const ADDON_HANDLE handle)
{
}

bool OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus)
{
  return true;
}

void Stop(const ADDON_HANDLE handle)
{
}

bool Dirty(const ADDON_HANDLE handle)
{
  return false;
}

bool OnInit(const ADDON_HANDLE handle)
{
  return true;
}

bool OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem)
{
  return false;
}

bool OnMouseEvent(const ADDON_HANDLE handle, int id, double x, double y, double offsetX, double offsetY, int state)
{
  return false;
}

} // extern "C"
