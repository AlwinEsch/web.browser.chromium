/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../AddonBase.h"
#include "../gui/input/Action.h"

#include <string>
#include <vector>

#include <kodi/c-api/addon-instance/web.h>

namespace kodi
{
namespace addon
{

class CWebControl
{
public:
  CWebControl(const WEB_ADDON_GUI_PROPS& properties, uint64_t handle, int dataIdentifier);
  virtual ~CWebControl();

  virtual void Render() {}
  virtual bool Dirty() { return false; }
  virtual bool OnInit() { return false; }
  virtual bool OnAction(const kodi::gui::input::CAction& action, int& nextItem) { return false; }
  virtual bool OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state)
  {
    return false;
  }
  virtual bool OpenWebsite(const std::string& url) { return false; }
  virtual bool GetHistory(std::vector<std::string>& historyWebsiteNames, bool behindCurrent)
  {
    return false;
  }
  virtual void SearchText(const std::string& text, bool forward, bool matchCase, bool findNext) {}
  virtual void StopSearch(bool clearSelection) {}
  virtual void Reload() {}
  virtual void StopLoad() {}
  virtual void GoBack() {}
  virtual void GoForward() {}
  virtual void OpenOwnContextMenu() {}
  virtual void ScreenSizeChange(float x, float y, float width, float height, bool fullscreen) {}

  void SetControlReady(bool ready);
  void SetOpenedAddress(const std::string& title);
  void SetOpenedTitle(const std::string& title);
  void SetIconURL(const std::string& icon);
  void SetFullscreen(bool fullscreen);
  void SetLoadingState(bool isLoading, bool canGoBack, bool canGoForward);
  void SetTooltip(const std::string& tooltip);
  void SetStatusMessage(const std::string& status);
  void RequestOpenSiteInNewTab(const std::string& url);

  int GetDataIdentifier() const { return m_dataIdentifier; }
  std::string GetName() const { return m_properties.strName; }
  kodi::HardwareContext GetDevice() const { return m_properties.pDevice; }
  float GetXPos() const { return m_properties.fXPos; }
  float GetYPos() const { return m_properties.fYPos; }
  float GetWidth() const { return m_properties.fWidth; }
  float GetHeight() const { return m_properties.fHeight; }
  float GetPixelRatio() const { return m_properties.fPixelRatio; }
  float GetFPS() const { return m_properties.fFPS; }
  float GetSkinXPos() const { return m_properties.fSkinXPos; }
  float GetSkinYPos() const { return m_properties.fSkinYPos; }
  float GetSkinWidth() const { return m_properties.fSkinWidth; }
  float GetSkinHeight() const { return m_properties.fSkinHeight; }
  bool UseTransparentBackground() const { return m_properties.bUseTransparentBackground; }
  uint32_t GetBackgroundColorARGB() const { return m_properties.iBackgroundColorARGB; }
  int GetGUIItemLeft() const { return m_properties.iGUIItemLeft; }
  int GetGUIItemRight() const { return m_properties.iGUIItemRight; }
  int GetGUIItemTop() const { return m_properties.iGUIItemTop; }
  int GetGUIItemBottom() const { return m_properties.iGUIItemBottom; }
  int GetGUIItemBack() const { return m_properties.iGUIItemBack; }

  uint64_t GetParentHdl_CWebControl() const { return m_handleBase; }

private:
  const uint64_t m_handle; // Kodi base related data handle and used for calls to it
  const int m_dataIdentifier;
  const WEB_ADDON_GUI_PROPS m_properties;
  uint64_t m_handleBase;
};

class CInstanceWeb
{
public:
  CInstanceWeb(uint64_t handle);
  virtual ~CInstanceWeb();

  virtual bool MainInitialize() { return false; }
  virtual void MainLoop() {}
  virtual void MainShutdown() {}
  virtual WEB_ADDON_ERROR StartInstance() { return WEB_ADDON_ERROR_NO_ERROR; }
  virtual void StopInstance() {}
  virtual void SetMute(bool mute) {}
  virtual bool SetLanguage(const std::string& language) { return false; }
  virtual kodi::addon::CWebControl* CreateControl(const WEB_ADDON_GUI_PROPS& properties,
                                                  const std::string& sourceName,
                                                  const std::string& startURL,
                                                  uint64_t handle)
  {
    return nullptr;
  }
  virtual bool DestroyControl(kodi::addon::CWebControl* control, bool complete) { return false; }

  void StartDone();
  void InhibitShutdown(bool inhibit);
  void InhibitScreensaver(bool inhibit);
  std::string UserPath(const std::string& append = "");
  std::string AddonLibPath(const std::string& append = "");
  std::string AddonSharePath(const std::string& append = "");
  bool IsMuted() const;

private:
  const uint64_t m_handle; // Kodi base related data handle and used for calls to it
};

} /* namespace addon */
} /* namespace kodi */
