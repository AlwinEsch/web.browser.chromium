/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "audio/AudioHandler.h"
#include "gui/GUIManager.h"
#include "renderer/IOffscreenBase.h"

// Dev-kit
#include "../../../lib/kodi-dev-kit/include/kodi/addon-instance/Web.h"

// CEF
#include "include/cef_app.h"
#include "include/cef_command_line.h"

// Global
#include <atomic>
#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>

#undef Success

namespace chromium
{
namespace app
{
namespace main
{

class CWebBrowserClient;

class CMainCEFProcess : public kodi::addon::CInstanceWeb
{
public:
  CMainCEFProcess(uint64_t base_handle, const std::string& identifier);
  ~CMainCEFProcess() override;

  bool Initialize(const CefMainArgs& args,
                  const CefSettings& settings,
                  CefRefPtr<CefApp> app,
                  void* windowsSandboxInfo);
  void Shutdown();
  void SetSettings(CefSettings& settings);
  bool ParseCmdLine(CefRefPtr<CefCommandLine> commandLine);
  const std::string GetIdentifier() const { return m_identifier; }

  // ---------------------------------------------------------------------------
  // Kodi interface parts

  WEB_ADDON_ERROR StartInstance() override;
  void StopInstance() override;

  bool MainInitialize() override;
  void MainShutdown() override;
  void MainLoop() override;

  void SetMute(bool mute) override;
  bool SetLanguage(const std::string& language) override;

  kodi::addon::CWebControl* CreateControl(const WEB_ADDON_GUI_PROPS& properties,
                                          const std::string& sourceName,
                                          const std::string& startURL,
                                          uint64_t handle) override;
  bool DestroyControl(kodi::addon::CWebControl* control, bool complete) override;

  void InformDestroyed(int uniqueClientId);

  const std::string& GetMainShared() const { return m_mainshared; }

  CefRefPtr<audio::CAudioHandler> GetAudioHandler() { return m_audioHandler; }
  std::shared_ptr<gui::CBrowserGUIManager> GetGUIManager() { return m_guiManager; }
  std::shared_ptr<renderer::IOffsceenBase> GetOffsceenGUIBase() { return m_offscreenGUIBase; }

private:
  bool PerformMessageLoopWork();

  static std::atomic_int m_iUniqueClientId;

  const std::string m_identifier;

  std::shared_ptr<gui::CBrowserGUIManager> m_guiManager;
  std::shared_ptr<renderer::IOffsceenBase> m_offscreenGUIBase;
  CefRefPtr<audio::CAudioHandler> m_audioHandler;

  std::unordered_map<int, CefRefPtr<CWebBrowserClient>> m_browserClients;
  std::unordered_map<std::string, CefRefPtr<CWebBrowserClient>> m_browserClientsInactive;
  std::set<int> m_browserClientsInDelete;

  std::atomic_bool m_started{false};
  int m_usedRand{-1};
  uint64_t m_baseInstanceId{0};
  std::string m_browserSubprocessPath;
  std::string m_frameworkDirPath;
  std::string m_localesPath;
  std::string m_resourcesPath;
  std::string m_cdmPath;
  std::string m_mainshared;

  bool m_isActive{false};
  bool m_reentrancyDetected{false};

  EGLDisplay m_eglDisplay;
  EGLContext m_eglContext;
  EGLSurface m_eglSurface;
};

} /* namespace main */
} /* namespace app */
} /* namespace chromium */
