/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/spin.h"

#include <msgpack.hpp>
#include <string>
#include <tuple>

MSGPACK_ADD_ENUM(AddonGUISpinControlType);

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Spin_h : int
{
  kodi_gui_controls_CSpin_create,
  kodi_gui_controls_CSpin_destroy,
  kodi_gui_controls_CSpin_SetVisible,
  kodi_gui_controls_CSpin_SetEnabled,
  kodi_gui_controls_CSpin_SetText,
  kodi_gui_controls_CSpin_Reset,
  kodi_gui_controls_CSpin_SetType,
  kodi_gui_controls_CSpin_AddLabel,
  kodi_gui_controls_CSpin_AddLabel2,
  kodi_gui_controls_CSpin_SetStringValue,
  kodi_gui_controls_CSpin_GetStringValue,
  kodi_gui_controls_CSpin_SetIntRange,
  kodi_gui_controls_CSpin_SetIntValue,
  kodi_gui_controls_CSpin_GetIntValue,
  kodi_gui_controls_CSpin_SetFloatRange,
  kodi_gui_controls_CSpin_SetFloatValue,
  kodi_gui_controls_CSpin_GetFloatValue,
  kodi_gui_controls_CSpin_SetFloatInterval,
  funcParent_gui_controls_Spin_h_LAST
} funcParent_gui_controls_Spin_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CSpin_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CSpin_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSpin_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CSpin_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CSpin_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetVisible;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CSpin_SetEnabled;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetEnabled;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CSpin_SetText;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetText;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSpin_Reset;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_Reset;
typedef std::tuple<uint64_t, AddonGUISpinControlType>                             msgParent__IN_kodi_gui_controls_CSpin_SetType;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetType;
typedef std::tuple<uint64_t, std::string, std::string>                            msgParent__IN_kodi_gui_controls_CSpin_AddLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_AddLabel;
typedef std::tuple<uint64_t, std::string, int>                                    msgParent__IN_kodi_gui_controls_CSpin_AddLabel2;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_AddLabel2;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CSpin_SetStringValue;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetStringValue;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSpin_GetStringValue;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CSpin_GetStringValue;
typedef std::tuple<uint64_t, int, int>                                            msgParent__IN_kodi_gui_controls_CSpin_SetIntRange;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetIntRange;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_controls_CSpin_SetIntValue;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetIntValue;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSpin_GetIntValue;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_controls_CSpin_GetIntValue;
typedef std::tuple<uint64_t, float, float>                                        msgParent__IN_kodi_gui_controls_CSpin_SetFloatRange;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetFloatRange;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSpin_SetFloatValue;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetFloatValue;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CSpin_GetFloatValue;
typedef std::tuple<float>                                                         msgParent_OUT_kodi_gui_controls_CSpin_GetFloatValue;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CSpin_SetFloatInterval;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CSpin_SetFloatInterval;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Spin_h
{
  funcChild_gui_controls_Spin_h_LAST
} funcChild_gui_controls_Spin_h;

} /* namespace sandbox */
} /* namespace kodi */

