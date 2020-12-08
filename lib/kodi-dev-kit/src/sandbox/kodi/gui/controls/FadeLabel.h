/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/fade_label.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_FadeLabel_h : int
{
  kodi_gui_controls_CFadeLabel_create,
  kodi_gui_controls_CFadeLabel_destroy,
  kodi_gui_controls_CFadeLabel_SetVisible,
  kodi_gui_controls_CFadeLabel_AddLabel,
  kodi_gui_controls_CFadeLabel_GetLabel,
  kodi_gui_controls_CFadeLabel_SetScrolling,
  kodi_gui_controls_CFadeLabel_Reset,
  funcParent_gui_controls_FadeLabel_h_LAST
} funcParent_gui_controls_FadeLabel_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CFadeLabel_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CFadeLabel_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CFadeLabel_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CFadeLabel_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CFadeLabel_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CFadeLabel_SetVisible;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CFadeLabel_AddLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CFadeLabel_AddLabel;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CFadeLabel_GetLabel;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CFadeLabel_GetLabel;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CFadeLabel_SetScrolling;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CFadeLabel_SetScrolling;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CFadeLabel_Reset;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CFadeLabel_Reset;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_FadeLabel_h
{
  funcChild_gui_controls_FadeLabel_h_LAST
} funcChild_gui_controls_FadeLabel_h;

} /* namespace sandbox */
} /* namespace kodi */

