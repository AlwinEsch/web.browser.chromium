/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/edit.h"

#include <msgpack.hpp>
#include <string>
#include <tuple>

MSGPACK_ADD_ENUM(AddonGUIInputType);

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Edit_h : int
{
  kodi_gui_controls_CEdit_create,
  kodi_gui_controls_CEdit_destroy,
  kodi_gui_controls_CEdit_SetVisible,
  kodi_gui_controls_CEdit_SetEnabled,
  kodi_gui_controls_CEdit_SetLabel,
  kodi_gui_controls_CEdit_GetLabel,
  kodi_gui_controls_CEdit_SetText,
  kodi_gui_controls_CEdit_GetText,
  kodi_gui_controls_CEdit_SetCursorPosition,
  kodi_gui_controls_CEdit_GetCursorPosition,
  kodi_gui_controls_CEdit_SetInputType,
  funcParent_gui_controls_Edit_h_LAST
} funcParent_gui_controls_Edit_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CEdit_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CEdit_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CEdit_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CEdit_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CEdit_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CEdit_SetVisible;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CEdit_SetEnabled;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CEdit_SetEnabled;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CEdit_SetLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CEdit_SetLabel;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CEdit_GetLabel;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CEdit_GetLabel;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_controls_CEdit_SetText;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CEdit_SetText;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CEdit_GetText;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_controls_CEdit_GetText;
typedef std::tuple<uint64_t, unsigned int>                                        msgParent__IN_kodi_gui_controls_CEdit_SetCursorPosition;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CEdit_SetCursorPosition;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CEdit_GetCursorPosition;
typedef std::tuple<unsigned int>                                                  msgParent_OUT_kodi_gui_controls_CEdit_GetCursorPosition;
typedef std::tuple<uint64_t, AddonGUIInputType, std::string>                      msgParent__IN_kodi_gui_controls_CEdit_SetInputType;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CEdit_SetInputType;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Edit_h
{
  funcChild_gui_controls_Edit_h_LAST
} funcChild_gui_controls_Edit_h;

} /* namespace sandbox */
} /* namespace kodi */

