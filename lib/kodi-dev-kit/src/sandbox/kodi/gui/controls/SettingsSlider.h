/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/settings_slider.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_SettingsSlider_h : int
{
  kodi_gui_controls_CSettingsSlider_create,
  kodi_gui_controls_CSettingsSlider_destroy,
  kodi_gui_controls_CSettingsSlider_SetVisible,
  kodi_gui_controls_CSettingsSlider_SetEnabled,
  kodi_gui_controls_CSettingsSlider_SetText,
  kodi_gui_controls_CSettingsSlider_Reset,
  kodi_gui_controls_CSettingsSlider_SetIntRange,
  kodi_gui_controls_CSettingsSlider_SetIntValue,
  kodi_gui_controls_CSettingsSlider_GetIntValue,
  kodi_gui_controls_CSettingsSlider_SetIntInterval,
  kodi_gui_controls_CSettingsSlider_SetPercentage,
  kodi_gui_controls_CSettingsSlider_GetPercentage,
  kodi_gui_controls_CSettingsSlider_SetFloatRange,
  kodi_gui_controls_CSettingsSlider_SetFloatValue,
  kodi_gui_controls_CSettingsSlider_GetFloatValue,
  kodi_gui_controls_CSettingsSlider_SetFloatInterval,
  funcParent_gui_controls_SettingsSlider_h_LAST
} funcParent_gui_controls_SettingsSlider_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CSettingsSlider_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CSettingsSlider_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSettingsSlider_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CSettingsSlider_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CSettingsSlider_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetVisible;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CSettingsSlider_SetEnabled;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetEnabled;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CSettingsSlider_SetText;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetText;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSettingsSlider_Reset;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_Reset;
typedef std::tuple<uint64_t, int, int>                                            msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntRange;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetIntRange;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntValue;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetIntValue;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSettingsSlider_GetIntValue;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetIntValue;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntInterval;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetIntInterval;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSettingsSlider_SetPercentage;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetPercentage;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSettingsSlider_GetPercentage;
typedef std::tuple<float>                                                         msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetPercentage;
typedef std::tuple<uint64_t, float, float>                                        msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatRange;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetFloatRange;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatValue;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetFloatValue;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSettingsSlider_GetFloatValue;
typedef std::tuple<float>                                                         msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetFloatValue;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatInterval;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSettingsSlider_SetFloatInterval;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_SettingsSlider_h
{
  funcChild_gui_controls_SettingsSlider_h_LAST
} funcChild_gui_controls_SettingsSlider_h;

} /* namespace sandbox */
} /* namespace kodi */

