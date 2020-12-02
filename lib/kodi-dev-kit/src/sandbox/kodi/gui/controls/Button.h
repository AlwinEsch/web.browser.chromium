/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/button.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Button_h : int
{
  kodi_gui_controls_CButton_create,
  kodi_gui_controls_CButton_destroy,
  kodi_gui_controls_CButton_SetVisible,
  kodi_gui_controls_CButton_SetEnabled,
  kodi_gui_controls_CButton_SetLabel,
  kodi_gui_controls_CButton_GetLabel,
  kodi_gui_controls_CButton_SetLabel2,
  kodi_gui_controls_CButton_GetLabel2,
  funcParent_gui_controls_Button_h_LAST
} funcParent_gui_controls_Button_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CButton_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CButton_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CButton_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CButton_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CButton_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CButton_SetVisible;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CButton_SetEnabled;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CButton_SetEnabled;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CButton_SetLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CButton_SetLabel;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CButton_GetLabel;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CButton_GetLabel;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CButton_SetLabel2;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CButton_SetLabel2;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CButton_GetLabel2;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CButton_GetLabel2;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Button_h
{
  funcChild_gui_controls_Button_h_LAST
} funcChild_gui_controls_Button_h;

} /* namespace sandbox */
} /* namespace kodi */

