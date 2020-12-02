/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/input/action.h"

#include <msgpack.hpp>

MSGPACK_ADD_ENUM(ADDON_ACTION);

namespace kodi
{
namespace sandbox
{

struct gui_input_CAction
{
  ADDON_ACTION id = ADDON_ACTION_NONE;
  std::string name;

  std::array<float, ADDON_ACTION_AMOUNT_MAX> amount = {};

  float repeat = 0.0f;
  unsigned int holdTime = 0;
  unsigned int buttonCode = 0;
  uint32_t unicode = 0; /* wchar_t */

  std::string text;

  MSGPACK_DEFINE(id, name, amount, repeat, holdTime, buttonCode, unicode, text);
};

typedef enum funcParent_gui_input_Action_h : int
{
  funcParent_gui_input_Action_h_LAST
} funcParent_gui_input_Action_h;

// clang-format off
// clang-format on

//==============================================================================

typedef enum funcChild_gui_input_Action_h
{
  funcChild_gui_input_Action_h_LAST
} funcChild_gui_input_Action_h;

// clang-format off
// clang-format on

} /* namespace sandbox */
} /* namespace kodi */
