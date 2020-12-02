/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/yes_no.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_YesNo_h : int
{
  kodi_gui_dialogs_YesNo_ShowAndGetInput,
  kodi_gui_dialogs_YesNo_ShowAndGetInput2,
  kodi_gui_dialogs_YesNo_ShowAndGetInput3,
  funcParent_gui_dialogs_YesNo_h_LAST
} funcParent_gui_dialogs_YesNo_h;

// clang-format off
typedef std::tuple<std::string, std::string, bool, std::string, std::string>      msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput;
typedef std::tuple<bool, bool>                                                    msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput;
typedef std::tuple<std::string, std::string, std::string, std::string, std::string, std::string> msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput2;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput2;
typedef std::tuple<std::string, std::string, std::string, std::string, bool, std::string, std::string> msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput3;
typedef std::tuple<bool, bool>                                                    msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput3;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_YesNo_h
{
  funcChild_gui_dialogs_YesNo_h_LAST
} funcChild_gui_dialogs_YesNo_h;

} /* namespace sandbox */
} /* namespace kodi */

