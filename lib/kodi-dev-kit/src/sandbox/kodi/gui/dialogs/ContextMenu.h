/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/context_menu.h"

#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_ContextMenu_h : int
{
  kodi_gui_dialogs_ContextMenu_Show,
  kodi_gui_dialogs_ContextMenu_Show2,
  kodi_gui_dialogs_ContextMenu_Show3,
  funcParent_gui_dialogs_ContextMenu_h_LAST
} funcParent_gui_dialogs_ContextMenu_h;

// clang-format off
typedef std::tuple<std::string, std::vector<std::string>>                         msgParent__IN_kodi_gui_dialogs_ContextMenu_Show;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_dialogs_ContextMenu_Show;
typedef std::tuple<std::string, std::vector<std::pair<std::string, std::string>>> msgParent__IN_kodi_gui_dialogs_ContextMenu_Show2;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_dialogs_ContextMenu_Show2;
typedef std::tuple<std::string, std::vector<std::pair<int, std::string>>>         msgParent__IN_kodi_gui_dialogs_ContextMenu_Show3;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_dialogs_ContextMenu_Show3;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_ContextMenu_h
{
  funcChild_gui_dialogs_ContextMenu_h_LAST
} funcChild_gui_dialogs_ContextMenu_h;

} /* namespace sandbox */
} /* namespace kodi */
