/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/keyboard.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_Keyboard_h : int
{
  kodi_gui_dialogs_Keyboard_ShowAndGetInput,
  kodi_gui_dialogs_Keyboard_ShowAndGetInput2,
  kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword,
  kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2,
  kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword,
  kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2,
  kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword,
  kodi_gui_dialogs_Keyboard_ShowAndGetFilter,
  kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard,
  kodi_gui_dialogs_Keyboard_IsKeyboardActivated,
  funcParent_gui_dialogs_Keyboard_h_LAST
} funcParent_gui_dialogs_Keyboard_h;

// clang-format off
typedef std::tuple<std::string, std::string, bool, bool, unsigned int>            msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetInput;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetInput;
typedef std::tuple<std::string, bool, unsigned int>                               msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetInput2;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetInput2;
typedef std::tuple<std::string, std::string, bool, unsigned int>                  msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword;
typedef std::tuple<std::string, unsigned int>                                     msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2;
typedef std::tuple<std::string, std::string, bool, unsigned int>                  msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword;
typedef std::tuple<std::string, unsigned int>                                     msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2;
typedef std::tuple<std::string, std::string, int, unsigned int>                   msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword;
typedef std::tuple<int, std::string>                                              msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword;
typedef std::tuple<std::string, bool, unsigned int>                               msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetFilter;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetFilter;
typedef std::tuple<std::string, bool>                                             msgParent__IN_kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_dialogs_Keyboard_IsKeyboardActivated;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_dialogs_Keyboard_IsKeyboardActivated;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_Keyboard_h
{
  funcChild_gui_dialogs_Keyboard_h_LAST
} funcChild_gui_dialogs_Keyboard_h;

} /* namespace sandbox */
} /* namespace kodi */

