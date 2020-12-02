/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/label.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Label_h : int
{
  kodi_gui_controls_CLabel_create,
  kodi_gui_controls_CLabel_destroy,
  kodi_gui_controls_CLabel_SetVisible,
  kodi_gui_controls_CLabel_SetLabel,
  kodi_gui_controls_CLabel_GetLabel,
  funcParent_gui_controls_Label_h_LAST
} funcParent_gui_controls_Label_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CLabel_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CLabel_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CLabel_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CLabel_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CLabel_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CLabel_SetVisible;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CLabel_SetLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CLabel_SetLabel;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CLabel_GetLabel;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CLabel_GetLabel;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Label_h
{
  funcChild_gui_controls_Label_h_LAST
} funcChild_gui_controls_Label_h;

} /* namespace sandbox */
} /* namespace kodi */

