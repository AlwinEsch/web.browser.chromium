/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/SettingsSlider.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/SettingsSlider.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CSettingsSlider::CSettingsSlider(CWindow* window, int controlId)
  : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_create(
                        uint64_t(this), m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSettingsSlider_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CSettingsSlider::~CSettingsSlider()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::SetEnabled(bool enabled)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetEnabled));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_SetEnabled(m_handle, enabled));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::SetText(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetText));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_SetText(m_handle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::Reset()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_Reset));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_Reset(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::SetIntRange(int start, int end)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetIntRange));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntRange(m_handle, start, end));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::SetIntValue(int value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetIntValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntValue(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int CSettingsSlider::GetIntValue() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_GetIntValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_GetIntValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetIntValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSettingsSlider::SetIntInterval(float interval)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetIntInterval));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntInterval(m_handle, interval));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::SetPercentage(float percent)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetPercentage));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_controls_CSettingsSlider_SetPercentage(m_handle, percent));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

float CSettingsSlider::GetPercentage() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_GetPercentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_GetFloatValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetPercentage t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSettingsSlider::SetFloatRange(float start, float end)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetFloatRange));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatRange(m_handle, start, end));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSettingsSlider::SetFloatValue(float value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetFloatValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatValue(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

float CSettingsSlider::GetFloatValue() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_GetFloatValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSettingsSlider_GetFloatValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetFloatValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSettingsSlider::SetFloatInterval(float interval)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_SettingsSlider_h,
                                  kodi_gui_controls_CSettingsSlider_SetFloatInterval));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatInterval(m_handle, interval));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
