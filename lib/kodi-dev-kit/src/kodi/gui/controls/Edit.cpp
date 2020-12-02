/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Edit.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Edit.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CEdit::CEdit(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_create));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_controls_CEdit_create(uint64_t(this), m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CEdit_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CEdit::~CEdit()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CEdit::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CEdit::SetEnabled(bool enabled)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_SetEnabled));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_SetEnabled(m_handle, enabled));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CEdit::SetLabel(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_SetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_SetLabel(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CEdit::GetLabel() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_GetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_GetLabel(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CEdit_GetLabel t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CEdit::SetText(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_SetText));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_SetText(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CEdit::GetText() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_GetText));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_GetText(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CEdit_GetText t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CEdit::SetCursorPosition(unsigned int position)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_SetCursorPosition));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_SetCursorPosition(m_handle, position));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

unsigned int CEdit::GetCursorPosition()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_GetCursorPosition));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_GetCursorPosition(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CEdit_GetCursorPosition t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CEdit::SetInputType(AddonGUIInputType type, const std::string& heading)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Edit_h, kodi_gui_controls_CEdit_SetInputType));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CEdit_SetInputType(m_handle, type, heading));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
