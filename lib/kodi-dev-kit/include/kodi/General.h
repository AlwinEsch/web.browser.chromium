/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "c-api/addon_base.h"
#include "c-api/general.h"

#include <string>
#include <vector>

namespace kodi
{

typedef struct kodi_version_t
{
  std::string compile_name;
  int major;
  int minor;
  std::string revision;
  std::string tag;
  std::string tag_revision;
} kodi_version_t;

void Log(const AddonLog loglevel, const char* format, ...);
std::string GetAddonPath(const std::string& append = "");
std::string GetBaseUserPath(const std::string& append = "");
std::string GetTempAddonPath(const std::string& append = "");
std::string GetSettingString(const std::string& settingName, const std::string& defaultValue = "");
void SetSettingString(const std::string& settingName, const std::string& settingValue);
int GetSettingInt(const std::string& settingName, int defaultValue = 0);
void SetSettingInt(const std::string& settingName, int settingValue);
bool GetSettingBoolean(const std::string& settingName, bool defaultValue = false);
void SetSettingBoolean(const std::string& settingName, bool settingValue);
template<typename enumType>
enumType GetSettingEnum(const std::string& settingName,
                        enumType defaultValue = static_cast<enumType>(0));
template<typename enumType>
void SetSettingEnum(const std::string& settingName, enumType settingValue);
std::string GetAddonInfo(const std::string& id);
bool OpenSettings();
std::string GetLocalizedString(uint32_t labelId, const std::string& defaultStr = "");
bool UnknownToUTF8(const std::string& stringSrc,
                   std::string& utf8StringDst,
                   bool failOnBadChar = false);
std::string GetLanguage(LangFormats format = LANG_FMT_ENGLISH_NAME, bool region = false);
void QueueFormattedNotification(QueueMsg type, const char* format, ...);
void QueueNotification(QueueMsg type,
                       const std::string& header,
                       const std::string& message,
                       const std::string& imageFile = "",
                       unsigned int displayTime = 5000,
                       bool withSound = true,
                       unsigned int messageTime = 1000);
std::string GetMD5(const std::string& text);
std::string GetRegion(const std::string& id);
void GetFreeMem(int64_t& free, int64_t& total, bool asBytes = false);
int GetGlobalIdleTime();
std::string GetCurrentSkinId();
bool IsAddonAvailable(const std::string& id, std::string& version, bool& enabled);
void KodiVersion(kodi_version_t& version);
bool GetKeyboardLayout(int modifierKey,
                       std::string& layout_name,
                       std::vector<std::vector<std::string>>& layout);
bool ChangeKeyboardLayout(std::string& layout_name);

} /* namespace kodi */
