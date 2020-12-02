/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/slider.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Slider_h : int
{
  kodi_gui_controls_CSlider_create,
  kodi_gui_controls_CSlider_destroy,
  kodi_gui_controls_CSlider_SetVisible,
  kodi_gui_controls_CSlider_SetEnabled,
  kodi_gui_controls_CSlider_Reset,
  kodi_gui_controls_CSlider_GetDescription,
  kodi_gui_controls_CSlider_SetIntRange,
  kodi_gui_controls_CSlider_SetIntValue,
  kodi_gui_controls_CSlider_GetIntValue,
  kodi_gui_controls_CSlider_SetIntInterval,
  kodi_gui_controls_CSlider_SetPercentage,
  kodi_gui_controls_CSlider_GetPercentage,
  kodi_gui_controls_CSlider_SetFloatRange,
  kodi_gui_controls_CSlider_SetFloatValue,
  kodi_gui_controls_CSlider_GetFloatValue,
  kodi_gui_controls_CSlider_SetFloatInterval,
  funcParent_gui_controls_Slider_h_LAST
} funcParent_gui_controls_Slider_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CSlider_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CSlider_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSlider_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CSlider_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CSlider_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetVisible;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CSlider_SetEnabled;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetEnabled;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSlider_Reset;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_Reset;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSlider_GetDescription;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CSlider_GetDescription;
typedef std::tuple<uint64_t, int, int>                                            msgParent__IN_kodi_gui_controls_CSlider_SetIntRange;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetIntRange;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_controls_CSlider_SetIntValue;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetIntValue;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSlider_GetIntValue;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_controls_CSlider_GetIntValue;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_controls_CSlider_SetIntInterval;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetIntInterval;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSlider_SetPercentage;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetPercentage;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSlider_GetPercentage;
typedef std::tuple<float>                                                         msgParent_OUT_kodi_gui_controls_CSlider_GetPercentage;
typedef std::tuple<uint64_t, float, float>                                        msgParent__IN_kodi_gui_controls_CSlider_SetFloatRange;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetFloatRange;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSlider_SetFloatValue;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetFloatValue;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSlider_GetFloatValue;
typedef std::tuple<float>                                                         msgParent_OUT_kodi_gui_controls_CSlider_GetFloatValue;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSlider_SetFloatInterval;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSlider_SetFloatInterval;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Slider_h
{
  funcChild_gui_controls_Slider_h_LAST
} funcChild_gui_controls_Slider_h;

} /* namespace sandbox */
} /* namespace kodi */

