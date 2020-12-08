/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/ok.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_OK_h : int
{
  kodi_gui_dialogs_OK_ShowAndGetInput,
  kodi_gui_dialogs_OK_ShowAndGetInput2,
  funcParent_gui_dialogs_OK_h_LAST
} funcParent_gui_dialogs_OK_h;

// clang-format off
typedef std::tuple<std::string, std::string>                                      msgParent__IN_kodi_gui_dialogs_OK_ShowAndGetInput;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_OK_ShowAndGetInput;
typedef std::tuple<std::string, std::string, std::string, std::string>            msgParent__IN_kodi_gui_dialogs_OK_ShowAndGetInput2;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_OK_ShowAndGetInput2;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_OK_h
{
  funcChild_gui_dialogs_OK_h_LAST
} funcChild_gui_dialogs_OK_h;

} /* namespace sandbox */
} /* namespace kodi */

