/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/text_box.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_CTextBox_h : int
{
  kodi_gui_controls_CTextBox_create,
  kodi_gui_controls_CTextBox_destroy,
  kodi_gui_controls_CTextBox_SetVisible,
  kodi_gui_controls_CTextBox_Reset,
  kodi_gui_controls_CTextBox_SetText,
  kodi_gui_controls_CTextBox_GetText,
  kodi_gui_controls_CTextBox_Scroll,
  kodi_gui_controls_CTextBox_SetAutoScrolling,
  funcParent_gui_controls_TextBox_h_LAST
} funcParent_gui_controls_TextBox_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CTextBox_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CTextBox_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CTextBox_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CTextBox_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CTextBox_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CTextBox_SetVisible;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CTextBox_Reset;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CTextBox_Reset;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CTextBox_SetText;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CTextBox_SetText;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CTextBox_GetText;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CTextBox_GetText;
typedef std::tuple<uint64_t, unsigned int>                                        msgParent__IN_kodi_gui_controls_CTextBox_Scroll;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CTextBox_Scroll;
typedef std::tuple<uint64_t, int, int, int>                                       msgParent__IN_kodi_gui_controls_CTextBox_SetAutoScrolling;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CTextBox_SetAutoScrolling;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_CTextBox_h
{
  funcChild_gui_controls_TextBox_h_LAST
} funcChild_gui_controls_TextBox_h;

} /* namespace sandbox */
} /* namespace kodi */

