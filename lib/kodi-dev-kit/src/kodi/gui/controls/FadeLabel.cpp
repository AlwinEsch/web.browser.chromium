/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/FadeLabel.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/FadeLabel.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CFadeLabel::CFadeLabel(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_FadeLabel_h, kodi_gui_controls_CFadeLabel_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CFadeLabel_create(uint64_t(this),
                                                                      m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CFadeLabel_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CFadeLabel::~CFadeLabel()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_FadeLabel_h, kodi_gui_controls_CFadeLabel_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CFadeLabel_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CFadeLabel::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_FadeLabel_h,
                                  kodi_gui_controls_CFadeLabel_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CFadeLabel_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CFadeLabel::AddLabel(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_FadeLabel_h, kodi_gui_controls_CFadeLabel_AddLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CFadeLabel_AddLabel(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CFadeLabel::GetLabel() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_FadeLabel_h, kodi_gui_controls_CFadeLabel_GetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CFadeLabel_GetLabel(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CFadeLabel_GetLabel t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CFadeLabel::SetScrolling(bool scroll)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_FadeLabel_h,
                                  kodi_gui_controls_CFadeLabel_SetScrolling));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CFadeLabel_SetScrolling(m_handle, scroll));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CFadeLabel::Reset()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_FadeLabel_h, kodi_gui_controls_CFadeLabel_Reset));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CFadeLabel_Reset(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
