/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/extended_progress.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_ExtendedProgress_h : int
{
  kodi_gui_dialogs_CExtendedProgress_create,
  kodi_gui_dialogs_CExtendedProgress_destroy,
  kodi_gui_dialogs_CExtendedProgress_Title,
  kodi_gui_dialogs_CExtendedProgress_SetTitle,
  kodi_gui_dialogs_CExtendedProgress_Text,
  kodi_gui_dialogs_CExtendedProgress_SetText,
  kodi_gui_dialogs_CExtendedProgress_IsFinished,
  kodi_gui_dialogs_CExtendedProgress_MarkFinished,
  kodi_gui_dialogs_CExtendedProgress_Percentage,
  kodi_gui_dialogs_CExtendedProgress_SetPercentage,
  kodi_gui_dialogs_CExtendedProgress_SetProgress,
  funcParent_gui_dialogs_ExtendedProgress_h_LAST
} funcParent_gui_dialogs_ExtendedProgress_h;

// clang-format off
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_dialogs_CExtendedProgress_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_destroy;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Title;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Title;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetTitle;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_SetTitle;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Text;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Text;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetText;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_SetText;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_IsFinished;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_IsFinished;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_MarkFinished;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_MarkFinished;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Percentage;
typedef std::tuple<float>                                                         msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Percentage;
typedef std::tuple<uint64_t, float>                                               msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetPercentage;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_SetPercentage;
typedef std::tuple<uint64_t, int, int>                                            msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetProgress;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_SetProgress;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_ExtendedProgress_h
{
  funcChild_gui_dialogs_ExtendedProgress_h_LAST
} funcChild_gui_dialogs_ExtendedProgress_h;

} /* namespace sandbox */
} /* namespace kodi */

