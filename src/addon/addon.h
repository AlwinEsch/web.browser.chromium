/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "WebBrowserClient.h"
#include "gui/GUIManager.h"

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/base/cef_thread_checker.h"
#include "include/wrapper/cef_message_router.h"

#include <kodi/addon-instance/Web.h>
#include <queue>
#include <unordered_map>
#include <mutex>

class CWebBrowserClient;

class ATTRIBUTE_HIDDEN CWebBrowser
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceWeb
{
public:
  CWebBrowser();
  ~CWebBrowser() = default;

  // ---------------------------------------------------------------------------
  // Kodi interface parts

  WEB_ADDON_ERROR StartInstance() override;
  void StopInstance() override;

  bool MainInitialize() override;
  void MainShutdown() override;
  void MainLoop() override;

  bool SetLanguage(const char *language) override;
  kodi::addon::CWebControl* CreateControl(const std::string& sourceName, const std::string& startURL, KODI_HANDLE handle) override;
  bool DestroyControl(kodi::addon::CWebControl* control, bool complete) override;

  // ---------------------------------------------------------------------------
  // Internal interface parts

  CBrowserGUIManager& GetGUIManager() { return m_guiManager; }

private:
  void ClearClosedBrowsers();

  static int m_iUniqueClientId;

  CBrowserGUIManager m_guiManager;
  CefRefPtr<CefApp> m_app;

  std::mutex m_mutex;

  CefSettings* m_cefSettings;
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
