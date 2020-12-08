/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/select.h"

#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_Select_h : int
{
  kodi_gui_dialogs_Select_Show,
  kodi_gui_dialogs_Select_Show2,
  kodi_gui_dialogs_Select_ShowMultiSelect,
  funcParent_gui_dialogs_Select_h_LAST
} funcParent_gui_dialogs_Select_h;

// clang-format off
typedef std::tuple<std::string, std::vector<std::string>, int, unsigned int>      msgParent__IN_kodi_gui_dialogs_Select_Show;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_dialogs_Select_Show;
typedef std::tuple<std::string, std::vector<std::tuple<std::string, std::string, bool>>, int, unsigned int> msgParent__IN_kodi_gui_dialogs_Select_Show2;
typedef std::tuple<int, std::vector<std::tuple<std::string, std::string, bool>>>  msgParent_OUT_kodi_gui_dialogs_Select_Show2;
typedef std::tuple<std::string, std::vector<std::tuple<std::string, std::string, bool>>, unsigned int> msgParent__IN_kodi_gui_dialogs_Select_ShowMultiSelect;
typedef std::tuple<bool, std::vector<std::tuple<std::string, std::string, bool>>> msgParent_OUT_kodi_gui_dialogs_Select_ShowMultiSelect;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_Select_h
{
  funcChild_gui_dialogs_Select_h_LAST
} funcChild_gui_dialogs_Select_h;

} /* namespace sandbox */
} /* namespace kodi */

