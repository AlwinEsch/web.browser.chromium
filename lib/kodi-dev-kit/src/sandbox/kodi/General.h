/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../include/kodi/c-api/general.h"

#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

typedef int DummyValue;

typedef enum funcParent_General_h : int
{
  kodi_Log = 0,
  kodi_GetAddonPath,
  kodi_GetBaseUserPath,
  kodi_GetTempAddonPath,
  kodi_OpenSettings,
  kodi_GetLocalizedString,
  kodi_GetSettingBoolean,
  kodi_SetSettingBoolean,
  kodi_GetSettingInt,
  kodi_SetSettingInt,
  kodi_GetSettingString,
  kodi_SetSettingString,
  kodi_GetLanguage,
  kodi_QueueNotification,
  kodi_UnknownToUTF8,
  kodi_GetAddonInfo,
  kodi_GetMD5,
  kodi_GetRegion,
  kodi_GetFreeMem,
  kodi_GetGlobalIdleTime,
  kodi_GetCurrentSkinId,
  kodi_IsAddonAvailable,
  kodi_KodiVersion,
  kodi_GetKeyboardLayout,
  kodi_ChangeKeyboardLayout,
} funcParent_General_h;

// clang-format off
typedef std::tuple<int, std::string>                                              msgParent__IN_kodi_Log;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_Log;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_GetAddonPath;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetAddonPath;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_GetBaseUserPath;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetBaseUserPath;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_GetTempAddonPath;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetTempAddonPath;
typedef std::tuple<std::string, bool>                                             msgParent__IN_kodi_GetSettingBoolean;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_GetSettingBoolean;
typedef std::tuple<std::string, bool>                                             msgParent__IN_kodi_SetSettingBoolean;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_SetSettingBoolean;
typedef std::tuple<std::string, int>                                              msgParent__IN_kodi_GetSettingInt;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_GetSettingInt;
typedef std::tuple<std::string, int>                                              msgParent__IN_kodi_SetSettingInt;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_SetSettingInt;
typedef std::tuple<std::string, std::string>                                      msgParent__IN_kodi_GetSettingString;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetSettingString;
typedef std::tuple<std::string, std::string>                                      msgParent__IN_kodi_SetSettingString;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_SetSettingString;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_GetAddonInfo;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetAddonInfo;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_OpenSettings;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_OpenSettings;
typedef std::tuple<uint32_t, std::string>                                         msgParent__IN_kodi_GetLocalizedString;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetLocalizedString;
typedef std::tuple<std::string, std::string, bool>                                msgParent__IN_kodi_UnknownToUTF8;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_UnknownToUTF8;
typedef std::tuple<uint32_t, bool>                                                msgParent__IN_kodi_GetLanguage;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetLanguage;
typedef std::tuple<int32_t, std::string, std::string, std::string, unsigned int, bool, unsigned int> msgParent__IN_kodi_QueueNotification;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_QueueNotification;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_GetMD5;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetMD5;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_GetRegion;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetRegion;
typedef std::tuple<int64_t, int64_t, bool>                                        msgParent__IN_kodi_GetFreeMem;
typedef std::tuple<int64_t, int64_t>                                              msgParent_OUT_kodi_GetFreeMem;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_GetGlobalIdleTime;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_GetGlobalIdleTime;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_GetCurrentSkinId;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_GetCurrentSkinId;
typedef std::tuple<std::string, std::string, bool>                                msgParent__IN_kodi_IsAddonAvailable;
typedef std::tuple<bool, std::string, bool>                                       msgParent_OUT_kodi_IsAddonAvailable;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_KodiVersion;
typedef std::tuple<std::string, int, int, std::string, std::string, std::string>  msgParent_OUT_kodi_KodiVersion;
typedef std::tuple<int, std::string, std::vector<std::vector<std::string>>>       msgParent__IN_kodi_GetKeyboardLayout;
typedef std::tuple<bool, std::string, std::vector<std::vector<std::string>>>      msgParent_OUT_kodi_GetKeyboardLayout;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_ChangeKeyboardLayout;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_ChangeKeyboardLayout;
// clang-format on

//==============================================================================

typedef enum funcChild_General_h
{
  funcIdAddon_unused
} funcChild_General_h;

} /* namespace sandbox */
} /* namespace kodi */
