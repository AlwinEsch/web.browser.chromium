/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../IMsgHdl.h"
#include "../../Launcher.h"
#include "../../../../../../src/addon/Web.h"

namespace kodi
{

namespace sandbox
{
namespace parent
{

class C_addoninstance_Web_h : public IMsgHdl
{
public:
  C_addoninstance_Web_h(CChildLauncher& childLauncher)
    : m_childLauncher(childLauncher)
  {
  }

  const std::string GetIdentifier() const { return m_childLauncher.GetIdentifier(); }

  bool HandleMessage(int funcGroup,
                     int func,
                     const msgpack::unpacked& in,
                     msgpack::sbuffer& out,
                     const char* data,
                     std::size_t size,
                     std::size_t offset) override;

  void MainLoop(uint64_t hdl);
  void MainShutdown(uint64_t hdl);
  void SetMute(uint64_t hdl, bool mute);
  bool SetLanguage(uint64_t hdl, const std::string& language);
  uint64_t CreateControl(uint64_t hdl,
                         const std::string& sourceName,
                         const std::string& startURL,
                         WEB_CONTROL_HANDLE& handle,
                         int& dataIdentifier);
  bool DestroyControl(uint64_t hdl, uint64_t control, bool complete);

  void ControlRender(uint64_t controlHdl);
  bool ControlDirty(uint64_t controlHdl);
  bool ControlOnInit(uint64_t controlHdl);
  bool ControlOnAction(uint64_t controlHdl, const kodi::gui::input::CAction& action, int& nextItem);
  bool ControlOnMouseEvent(uint64_t controlHdl, int id, double x, double y, double offsetX, double offsetY, int state);
  bool ControlOpenWebsite(uint64_t controlHdl, const std::string& url);
  bool ControlGetHistory(uint64_t controlHdl, std::vector<std::string>& historyWebsiteNames, bool behindCurrent);
  void ControlSearchText(uint64_t controlHdl, const std::string& text, bool forward, bool matchCase, bool findNext);
  void ControlStopSearch(uint64_t controlHdl, bool clearSelection);
  void ControlReload(uint64_t controlHdl);
  void ControlStopLoad(uint64_t controlHdl);
  void ControlGoBack(uint64_t controlHdl);
  void ControlGoForward(uint64_t controlHdl);
  void ControlOpenOwnContextMenu(uint64_t controlHdl);
  void ControlScreenSizeChange(uint64_t controlHdl, float x, float y, float width, float height, bool fullscreen);

private:
  CChildLauncher& m_childLauncher;
};

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */

