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

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_Processor_h : int
{
  kodi_processor_CreateForNewThread,
  kodi_processor_CreateForNewThread2,
} funcParent_Processor_h;

// clang-format off
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_processor_CreateForNewThread;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_processor_CreateForNewThread;
// clang-format on

//==============================================================================

typedef enum funcChild_Processor_h
{
  kodi_processor_CreateForNewChildThread,
  kodi_processor_CreateForNewChildThread2,
  kodi_processor_KillChild,
} funcChild_Processor_h;

// clang-format off
typedef std::tuple<std::string>                                                   msgChild__IN_kodi_processor_KillChild;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_processor_KillChild;
typedef std::tuple<std::string>                                                   msgChild__IN_kodi_processor_CreateForNewThread;
typedef std::tuple<std::string>                                                   msgChild_OUT_kodi_processor_CreateForNewThread;
// clang-format on

} /* namespace sandbox */
} /* namespace kodi */

