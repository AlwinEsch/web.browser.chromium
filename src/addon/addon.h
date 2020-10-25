/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "WebBrowserClient.h"
#include "WidevineControl.h"
#include "audio/AudioHandler.h"
#include "gui/GUIManager.h"
#include "include/base/cef_thread_checker.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#if defined(TARGET_DARWIN)
#include "include/wrapper/cef_library_loader.h"
#endif
#include "include/wrapper/cef_message_router.h"

#include <kodi/addon-instance/Web.h>
#include <mutex>
#include <queue>
#include <unordered_map>

class CWebBrowserClient;

class ATTRIBUTE_HIDDEN CWebBrowser : public kodi::addon::CAddonBase,
                                     public kodi::addon::CInstanceWeb
{
public:
  CWebBrowser() = default;
  ~CWebBrowser() override = default;

  // ---------------------------------------------------------------------------
  // Kodi interface parts

  WEB_ADDON_ERROR StartInstance() override;
  void StopInstance() override;

  bool MainInitialize() override;
  void MainShutdown() override;
  void MainLoop() override;

  void SetMute(bool mute) override;
  bool SetLanguage(const char* language) override;
  kodi::addon::CWebControl* CreateControl(const std::string& sourceName,
                                          const std::string& startURL,
                                          KODI_HANDLE handle) override;
  bool DestroyControl(kodi::addon::CWebControl* control, bool complete) override;

  // ---------------------------------------------------------------------------
  // Internal interface parts

  CBrowserGUIManager& GetGUIManager() { return m_guiManager; }
  CefRefPtr<CefApp> GetApp() { return m_app; }
  CefRefPtr<CAudioHandler> GetAudioHandler() { return m_audioHandler; }

  void InformDestroyed(int uniqueClientId);

private:
  static std::atomic_int m_iUniqueClientId;

  CBrowserGUIManager m_guiManager{this};
  CWidewineControl m_widewineControl{*this};
  CefRefPtr<CefApp> m_app;
  CefRefPtr<CAudioHandler> m_audioHandler;

  std::mutex m_mutex;

  CefSettings* m_cefSettings;
  std::string m_browserSubprocessPath;
  std::string m_frameworkDirPath;
  std::string m_localesPath;
  std::string m_resourcesPath;

#if defined(TARGET_DARWIN)
  CefScopedLibraryLoader m_cefLibraryLoader;
#endif

  std::unordered_map<int, CefRefPtr<CWebBrowserClient>> m_browserClients;
  std::unordered_map<std::string, CefRefPtr<CWebBrowserClient>> m_browserClientsInactive;
  std::set<int> m_browserClientsInDelete;
  std::atomic_bool m_started{false};
};
