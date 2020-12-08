/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/radio_button.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_RadioButton_h : int
{
  kodi_gui_controls_CRadioButton_create,
  kodi_gui_controls_CRadioButton_destroy,
  kodi_gui_controls_CRadioButton_SetVisible,
  kodi_gui_controls_CRadioButton_SetEnabled,
  kodi_gui_controls_CRadioButton_SetLabel,
  kodi_gui_controls_CRadioButton_GetLabel,
  kodi_gui_controls_CRadioButton_SetSelected,
  kodi_gui_controls_CRadioButton_IsSelected,
  funcParent_gui_controls_RadioButton_h_LAST
} funcParent_gui_controls_RadioButton_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CRadioButton_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CRadioButton_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CRadioButton_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CRadioButton_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CRadioButton_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CRadioButton_SetVisible;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CRadioButton_SetEnabled;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CRadioButton_SetEnabled;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CRadioButton_SetLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CRadioButton_SetLabel;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CRadioButton_GetLabel;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CRadioButton_GetLabel;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CRadioButton_SetSelected;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CRadioButton_SetSelected;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CRadioButton_IsSelected;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_controls_CRadioButton_IsSelected;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_RadioButton_h
{
  funcChild_gui_controls_RadioButton_h_LAST
} funcChild_gui_controls_RadioButton_h;

} /* namespace sandbox */
} /* namespace kodi */

