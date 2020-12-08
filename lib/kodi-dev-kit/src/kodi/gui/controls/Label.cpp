/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Label.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Label.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CLabel::CLabel(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Label_h, kodi_gui_controls_CLabel_create));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_controls_CLabel_create(uint64_t(this), m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CLabel_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CLabel::~CLabel()
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Label_h, kodi_gui_controls_CLabel_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CLabel_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CLabel::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Label_h, kodi_gui_controls_CLabel_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CLabel_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CLabel::SetLabel(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Label_h, kodi_gui_controls_CLabel_SetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CLabel_SetLabel(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CLabel::GetLabel() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Label_h, kodi_gui_controls_CLabel_GetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CLabel_GetLabel(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CLabel_GetLabel t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
