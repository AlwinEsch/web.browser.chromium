/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/RadioButton.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/RadioButton.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CRadioButton::CRadioButton(CWindow* window, int controlId)
  : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_create(uint64_t(this),
                                                                        m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CRadioButton_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CRadioButton::~CRadioButton()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CRadioButton::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CRadioButton::SetEnabled(bool enabled)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_SetEnabled));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_SetEnabled(m_handle, enabled));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CRadioButton::SetLabel(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_SetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_SetLabel(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CRadioButton::GetLabel() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_GetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_GetLabel(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CRadioButton_GetLabel t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CRadioButton::SetSelected(bool selected)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_SetSelected));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_SetSelected(m_handle, selected));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CRadioButton::IsSelected() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_RadioButton_h,
                                  kodi_gui_controls_CRadioButton_IsSelected));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRadioButton_IsSelected(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CRadioButton_IsSelected t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
