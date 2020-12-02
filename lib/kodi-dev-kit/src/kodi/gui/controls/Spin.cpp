/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Spin.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Spin.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CSpin::CSpin(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_create));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_controls_CSpin_create(uint64_t(this), m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSpin_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CSpin::~CSpin()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::SetEnabled(bool enabled)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetEnabled));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetEnabled(m_handle, enabled));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::SetText(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetText));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetText(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::Reset()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_Reset));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_Reset(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::SetType(AddonGUISpinControlType type)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetType));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetType(m_handle, type));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::AddLabel(const std::string& label, const std::string& value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_AddLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_AddLabel(m_handle, label, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::AddLabel(const std::string& label, int value)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_AddLabel2));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_AddLabel2(m_handle, label, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::SetStringValue(const std::string& value)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetStringValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetStringValue(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CSpin::GetStringValue() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_GetStringValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_GetStringValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSpin_GetStringValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSpin::SetIntRange(int start, int end)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetIntRange));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetIntRange(m_handle, start, end));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::SetIntValue(int value)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetIntValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetIntValue(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int CSpin::GetIntValue() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_GetIntValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_GetIntValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSpin_GetIntValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSpin::SetFloatRange(float start, float end)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetFloatRange));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetFloatRange(m_handle, start, end));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSpin::SetFloatValue(float value)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetFloatValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetFloatValue(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

float CSpin::GetFloatValue() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_GetFloatValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_GetFloatValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSpin_GetFloatValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSpin::SetFloatInterval(float interval)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Spin_h, kodi_gui_controls_CSpin_SetFloatInterval));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSpin_SetFloatInterval(m_handle, interval));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
