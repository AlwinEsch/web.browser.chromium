/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/numeric.h"

#include <msgpack.hpp>
#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_Numeric_h : int
{
  kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword,
  kodi_gui_dialogs_Numeric_ShowAndVerifyPassword,
  kodi_gui_dialogs_Numeric_ShowAndVerifyInput,
  kodi_gui_dialogs_Numeric_ShowAndGetTime,
  kodi_gui_dialogs_Numeric_ShowAndGetDate,
  kodi_gui_dialogs_Numeric_ShowAndGetIPAddress,
  kodi_gui_dialogs_Numeric_ShowAndGetNumber,
  kodi_gui_dialogs_Numeric_ShowAndGetSeconds,
  funcParent_gui_dialogs_Numeric_h_LAST
} funcParent_gui_dialogs_Numeric_h;

struct timeValue : tm
{
  timeValue() = default;
  timeValue(tm& data) : tm(data) {}

  MSGPACK_DEFINE(tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst, tm_gmtoff);
};

// clang-format off
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword;
typedef std::tuple<std::string, std::string, int>                                 msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyPassword;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyPassword;
typedef std::tuple<std::string, std::string, bool>                                msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyInput;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyInput;
typedef std::tuple<timeValue, std::string>                                        msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetTime;
typedef std::tuple<bool, timeValue>                                               msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetTime;
typedef std::tuple<timeValue, std::string>                                        msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetDate;
typedef std::tuple<bool, timeValue>                                               msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetDate;
typedef std::tuple<std::string, std::string>                                      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetIPAddress;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetIPAddress;
typedef std::tuple<std::string, std::string, unsigned int>                        msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetNumber;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetNumber;
typedef std::tuple<std::string, std::string>                                      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetSeconds;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetSeconds;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_Numeric_h
{
  funcChild_gui_dialogs_Numeric_h_LAST
} funcChild_gui_dialogs_Numeric_h;

} /* namespace sandbox */
} /* namespace kodi */

