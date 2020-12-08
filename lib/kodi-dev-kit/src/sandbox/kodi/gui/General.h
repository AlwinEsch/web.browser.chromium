/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../include/kodi/c-api/gui/general.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef int DummyValue;

typedef enum funcParent_gui_General_h : int
{
  kodi_gui_Lock,
  kodi_gui_Unlock,
  kodi_gui_GetScreenHeight,
  kodi_gui_GetScreenWidth,
  kodi_gui_GetVideoResolution,
  kodi_gui_GetCurrentWindowDialogId,
  kodi_gui_GetCurrentWindowId,
  kodi_gui_GetHWContext,

  funcParent_gui_General_h_LAST
} funcParent_gui_General_h;

// clang-format off
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_Lock;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_Lock;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_Unlock;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_Unlock;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_GetScreenHeight;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_GetScreenHeight;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_GetScreenWidth;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_GetScreenWidth;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_GetVideoResolution;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_GetVideoResolution;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_GetCurrentWindowDialogId;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_GetCurrentWindowDialogId;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_GetCurrentWindowId;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_GetCurrentWindowId;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_gui_GetHWContext;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_GetHWContext;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_General_h
{
  funcChild_gui_General_h_LAST
} funcChild_gui_General_h;

} /* namespace sandbox */
} /* namespace kodi */

