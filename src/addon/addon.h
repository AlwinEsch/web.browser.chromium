#pragma once
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

#include <kodi/addon-instance/Web.h>

#include "DownloadHandler.h"
#include "GeolocationPermission.h"
#include "UploadHandler.h"

class CWebBrowserManager;

extern std::string                   g_strTempPath;
extern std::string                   g_strLogPath;
extern std::string                   g_strUserPath;
extern std::string                   g_strAddonLibPath;
extern std::string                   g_strAddonSharePath;
extern CWebBrowserManager           *g_pWebManager;


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

  CefRefPtr<CWebBrowserDownloadHandler> GetDownloadHandler() { return m_downloadHandler; }
  CefRefPtr<CWebBrowserUploadHandler> GetUploadHandler() { return m_uploadHandler; }
  CefRefPtr<CWebBrowserGeolocationPermission> GetGeolocationPermission() { return m_geolocationPermission; }

private:
  CefRefPtr<CWebBrowserDownloadHandler> m_downloadHandler;
  CefRefPtr<CWebBrowserUploadHandler> m_uploadHandler;
  CefRefPtr<CWebBrowserGeolocationPermission> m_geolocationPermission;
};
