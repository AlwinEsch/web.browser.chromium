/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../include/kodi/c-api/addon_base.h"

#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_AddonBase_h : int
{
  kodi_CAddonBase_constructor,
  kodi_CAddonBase_destructor,
} funcParent_AddonBase_h;

// clang-format off
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_CAddonBase_constructor;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_CAddonBase_constructor;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_CAddonBase_destructor;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_CAddonBase_destructor;
// clang-format on

//==============================================================================

typedef enum funcChild_AddonBase_h : int
{
  funcIdAddon_unused_AddonBase_h
} funcChild_AddonBase_h;

// clang-format off

// clang-format on

} /* namespace sandbox */
} /* namespace kodi */
