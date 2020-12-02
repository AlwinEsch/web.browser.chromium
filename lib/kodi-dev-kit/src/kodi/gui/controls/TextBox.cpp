/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/TextBox.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/TextBox.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CTextBox::CTextBox(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_TextBox_h, kodi_gui_controls_CTextBox_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CTextBox_create(uint64_t(this), m_handleWindow,
                                                                    controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CTextBox_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CTextBox::~CTextBox()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_TextBox_h, kodi_gui_controls_CTextBox_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CTextBox_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CTextBox::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_TextBox_h, kodi_gui_controls_CTextBox_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CTextBox_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CTextBox::Reset()
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_TextBox_h, kodi_gui_controls_CTextBox_Reset));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CTextBox_Reset(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CTextBox::SetText(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_TextBox_h, kodi_gui_controls_CTextBox_SetText));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CTextBox_SetText(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CTextBox::GetText() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_TextBox_h, kodi_gui_controls_CTextBox_GetText));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CTextBox_GetText(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CTextBox_GetText t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CTextBox::Scroll(unsigned int position)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_TextBox_h, kodi_gui_controls_CTextBox_Scroll));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CTextBox_Scroll(m_handle, position));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CTextBox::SetAutoScrolling(int delay, int time, int repeat)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_TextBox_h,
                                  kodi_gui_controls_CTextBox_SetAutoScrolling));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_controls_CTextBox_SetAutoScrolling(m_handle, delay, time, repeat));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
