/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/input/action.h"

#include <msgpack.hpp>

MSGPACK_ADD_ENUM(ADDON_ACTION);

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_input_ActionIDs_h : int
{
  funcParent_gui_input_ActionIDs_h_LAST
} funcParent_gui_input_ActionIDs_h;

// clang-format off
// clang-format on

//==============================================================================

typedef enum funcChild_gui_input_ActionIDs_h
{
  funcChild_gui_input_ActionIDs_h_LAST
} funcChild_gui_input_ActionIDs_h;

// clang-format off
// clang-format on

} /* namespace sandbox */
} /* namespace kodi */

