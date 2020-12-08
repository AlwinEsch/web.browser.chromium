/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/text_viewer.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_TextViewer_h : int
{
  kodi_gui_dialogs_TextViewer_Show,
  funcParent_gui_dialogs_TextViewer_h_LAST
} funcParent_gui_dialogs_TextViewer_h;

// clang-format off
typedef std::tuple<std::string, std::string>                                      msgParent__IN_kodi_gui_dialogs_TextViewer_Show;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_TextViewer_Show;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_TextViewer_h
{
  funcChild_gui_dialogs_TextViewer_h_LAST
} funcChild_gui_dialogs_TextViewer_h;

} /* namespace sandbox */
} /* namespace kodi */

