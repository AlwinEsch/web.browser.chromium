/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Button.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Button.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CButton::CButton(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_create(uint64_t(this), m_handleWindow,
                                                                   controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CButton_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CButton::~CButton()
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CButton::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CButton::SetEnabled(bool enabled)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_SetEnabled));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_SetEnabled(m_handle, enabled));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CButton::SetLabel(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_SetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_SetLabel(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CButton::GetLabel() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_GetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_GetLabel(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CButton_GetLabel t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CButton::SetLabel2(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_SetLabel2));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_SetLabel2(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CButton::GetLabel2() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Button_h, kodi_gui_controls_CButton_GetLabel2));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CButton_GetLabel2(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CButton_GetLabel2 t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
