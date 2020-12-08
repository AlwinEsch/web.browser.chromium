/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Slider.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Slider.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CSlider::CSlider(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_create(uint64_t(this), m_handleWindow,
                                                                   controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSlider_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CSlider::~CSlider()
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSlider::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSlider::SetEnabled(bool enabled)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetEnabled));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetEnabled(m_handle, enabled));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSlider::Reset()
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_Reset));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_Reset(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CSlider::GetDescription() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_GetDescription));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_GetDescription(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSlider_GetDescription t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSlider::SetIntRange(int start, int end)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetIntRange));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetIntRange(m_handle, start, end));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSlider::SetIntValue(int value)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetIntValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetIntValue(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int CSlider::GetIntValue() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_GetIntValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_GetIntValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSlider_GetIntValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSlider::SetIntInterval(float interval)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetIntInterval));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetIntInterval(m_handle, interval));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSlider::SetPercentage(float percent)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetPercentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetPercentage(m_handle, percent));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

float CSlider::GetPercentage() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_GetPercentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_GetFloatValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSlider_GetPercentage t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSlider::SetFloatRange(float start, float end)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetFloatRange));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetFloatRange(m_handle, start, end));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CSlider::SetFloatValue(float value)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_SetFloatValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetFloatValue(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

float CSlider::GetFloatValue() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Slider_h, kodi_gui_controls_CSlider_GetFloatValue));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_GetFloatValue(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CSlider_GetFloatValue t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CSlider::SetFloatInterval(float interval)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Slider_h,
                                  kodi_gui_controls_CSlider_SetFloatInterval));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CSlider_SetFloatInterval(m_handle, interval));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
