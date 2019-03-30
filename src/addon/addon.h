/*
 *      Copyright (C) 2015-2019 Team KODI
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

#pragma once

#include "WebBrowserClient.h"
#include "gui/GUIManager.h"

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/base/cef_thread_checker.h"
#include "include/wrapper/cef_message_router.h"

#include <kodi/addon-instance/Web.h>
#include <p8-platform/threads/threads.h>
#include <queue>
#include <unordered_map>
#include <mutex>

class CWebBrowserClient;

class ATTRIBUTE_HIDDEN CWebBrowser
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceWeb,
    public P8PLATFORM::CThread
{
public:
  CWebBrowser();
  ~CWebBrowser() = default;

  // ---------------------------------------------------------------------------
  // Kodi interface parts

  WEB_ADDON_ERROR StartInstance() override;
  void StopInstance() override;

  bool MainInitialize();
  void MainShutdown();
  void MainLoop() override;

  bool SetLanguage(const char *language) override;
  kodi::addon::CWebControl* CreateControl(const std::string& sourceName, const std::string& startURL, KODI_HANDLE handle) override;
  bool DestroyControl(kodi::addon::CWebControl* control, bool complete) override;


  // ---------------------------------------------------------------------------
  // Internal interface parts

  bool IsActive() const { return m_isActive; }

  CBrowserGUIManager& GetGUIManager() { return m_guiManager; }

protected:
  virtual void* Process() override;

private:
  static int m_iUniqueClientId;

  CBrowserGUIManager m_guiManager;
  CefRefPtr<CefApp> m_app;

  std::mutex m_mutex;

  bool m_isActive;
  CefSettings m_cefSettings;
  std::string m_strHTMLCachePath;
  std::string m_strCookiePath;
  std::string m_strResourcesPath;
  std::string m_strLocalesPath;
  std::string m_strLibPath;

  std::unordered_map<int, CWebBrowserClient*> m_browserClients;
  std::unordered_map<std::string, CWebBrowserClient*> m_browserClientsInactive;
  std::vector<CWebBrowserClient*> m_browserClientsToDelete;
  bool m_started = false;
};
