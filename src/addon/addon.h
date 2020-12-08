/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "ViewRendererGL.h"

// #include <kodi/addon-instance/Web.h>
#include "Web.h"

#include <memory>
#include <mutex>

namespace kodi
{
namespace sandbox
{
namespace parent
{
class CChildLauncher;
class C_addoninstance_Web_h;
} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */

class ATTRIBUTE_HIDDEN CAddon : public kodi::addon::CAddonBase
{
public:
  CAddon();
  ~CAddon() override;

  ADDON_STATUS Create() override;

  ADDON_STATUS CreateInstance(int instanceType,
                              const std::string& instanceID,
                              KODI_HANDLE instance,
                              const std::string& version,
                              KODI_HANDLE& addonInstance) override;
};

class ATTRIBUTE_HIDDEN CWebBrowser : public kodi::addon::CInstanceWebOwn
{
public:
  CWebBrowser(KODI_HANDLE instance, const std::string& kodiVersion);
  ~CWebBrowser() override;

  // ---------------------------------------------------------------------------
  // Kodi interface parts

  WEB_ADDON_ERROR StartInstance() override;
  void StopInstance() override;

  bool MainInitialize() override;
  void MainShutdown() override;
  void MainLoop() override;

  void StartDone(uint64_t childHdl) override;

  void SetMute(bool mute) override;
  bool SetLanguage(const std::string& language) override;

  uint64_t CreateControl(const std::string& sourceName,
                         const std::string& startURL,
                         WEB_CONTROL_HANDLE& handle,
                         int& dataIdentifier) override;
  bool DestroyControl(uint64_t control, bool complete) override;

private:
  uint64_t m_childHdl = 0;
  kodi::sandbox::parent::CChildLauncher* m_launcher;
  kodi::sandbox::parent::C_addoninstance_Web_h* m_hdl;
};

class ATTRIBUTE_HIDDEN CWebBrowserClient : public kodi::addon::CWebControlOwn
{
public:
  CWebBrowserClient(WEB_CONTROL_HANDLE& hdlKodi,
                    uint64_t hdlChild,
                    int dataIdentifier,
                    kodi::sandbox::parent::C_addoninstance_Web_h* hdlParent);
  virtual ~CWebBrowserClient();

  bool RenderInit(int width, int height) override;
  void Render() override;
  bool Dirty() override;
  bool OnInit() override;
  bool OnAction(const kodi::gui::input::CAction& action, int& nextItem) override;
  bool OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state) override;
  bool OpenWebsite(const std::string& url) override;
  bool GetHistory(std::vector<std::string>& historyWebsiteNames, bool behindCurrent) override;
  void SearchText(const std::string& text, bool forward, bool matchCase, bool findNext) override;
  void StopSearch(bool clearSelection) override;
  void Reload() override;
  void StopLoad() override;
  void GoBack() override;
  void GoForward() override;
  void OpenOwnContextMenu() override;
  void ScreenSizeChange(float x, float y, float width, float height, bool fullscreen) override;

private:
  kodi::sandbox::parent::C_addoninstance_Web_h* m_hdl;
  uint64_t m_controlHdl = 0;
  CViewRendererOpenGL* m_rendering = nullptr;
  std::string m_clientIdentifier;
};
