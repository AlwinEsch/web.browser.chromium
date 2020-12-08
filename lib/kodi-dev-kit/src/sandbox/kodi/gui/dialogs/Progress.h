/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/progress.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_Progress_h : int
{
  kodi_gui_dialogs_CProgress_create,
  kodi_gui_dialogs_CProgress_destroy,
  kodi_gui_dialogs_CProgress_Open,
  kodi_gui_dialogs_CProgress_SetHeading,
  kodi_gui_dialogs_CProgress_SetLine,
  kodi_gui_dialogs_CProgress_SetCanCancel,
  kodi_gui_dialogs_CProgress_IsCanceled,
  kodi_gui_dialogs_CProgress_SetPercentage,
  kodi_gui_dialogs_CProgress_GetPercentage,
  kodi_gui_dialogs_CProgress_ShowProgressBar,
  kodi_gui_dialogs_CProgress_SetProgressMax,
  kodi_gui_dialogs_CProgress_SetProgressAdvance,
  kodi_gui_dialogs_CProgress_Abort,
  funcParent_gui_dialogs_Progress_h_LAST
} funcParent_gui_dialogs_Progress_h;

// clang-format off
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CProgress_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_dialogs_CProgress_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CProgress_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_dialogs_CProgress_destroy;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CProgress_Open;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_Open;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_dialogs_CProgress_SetHeading;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_SetHeading;
typedef std::tuple<uint64_t, unsigned int, std::string>                           msgParent__IN_kodi_gui_dialogs_CProgress_SetLine;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_SetLine;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_dialogs_CProgress_SetCanCancel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_SetCanCancel;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CProgress_IsCanceled;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_dialogs_CProgress_IsCanceled;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_dialogs_CProgress_SetPercentage;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_SetPercentage;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CProgress_GetPercentage;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_dialogs_CProgress_GetPercentage;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_dialogs_CProgress_ShowProgressBar;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_ShowProgressBar;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_dialogs_CProgress_SetProgressMax;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_SetProgressMax;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_dialogs_CProgress_SetProgressAdvance;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CProgress_SetProgressAdvance;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CProgress_Abort;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_dialogs_CProgress_Abort;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_Progress_h
{
  funcChild_gui_dialogs_Progress_h_LAST
} funcChild_gui_dialogs_Progress_h;

} /* namespace sandbox */
} /* namespace kodi */

