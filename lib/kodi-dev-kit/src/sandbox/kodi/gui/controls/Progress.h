/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/progress.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Progress_h : int
{
  kodi_gui_controls_CProgress_create,
  kodi_gui_controls_CProgress_destroy,
  kodi_gui_controls_CProgress_SetVisible,
  kodi_gui_controls_CProgress_SetPercentage,
  kodi_gui_controls_CProgress_GetPercentage,
  funcParent_gui_controls_Progress_h_LAST
} funcParent_gui_controls_Progress_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CProgress_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CProgress_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CProgress_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CProgress_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CProgress_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CProgress_SetVisible;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_controls_CProgress_SetPercentage;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CProgress_SetPercentage;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CProgress_GetPercentage;
typedef std::tuple<float>                                                         msgParent_OUT_kodi_gui_controls_CProgress_GetPercentage;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Progress_h
{
  funcChild_gui_controls_Progress_h_LAST
} funcChild_gui_controls_Progress_h;

} /* namespace sandbox */
} /* namespace kodi */

