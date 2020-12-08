/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Progress.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Progress.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CProgress::CProgress(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Progress_h, kodi_gui_controls_CProgress_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CProgress_create(uint64_t(this), m_handleWindow,
                                                                     controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CProgress_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CProgress::~CProgress()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Progress_h, kodi_gui_controls_CProgress_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CProgress_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Progress_h, kodi_gui_controls_CProgress_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CProgress_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::SetPercentage(float percent)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Progress_h,
                                  kodi_gui_controls_CProgress_SetPercentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CProgress_SetPercentage(m_handle, percent));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

float CProgress::GetPercentage() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Progress_h,
                                  kodi_gui_controls_CProgress_GetPercentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CProgress_GetPercentage(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CProgress_GetPercentage t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
