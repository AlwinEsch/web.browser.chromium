/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../sandbox/kodi/General.h"

#include "../../include/kodi/General.h"
#include "../../include/kodi/Processor.h"
#include "../sandbox/ShareProcessTransmitter.h"

#include <kodi/tools/StringUtils.h>

using namespace kodi::sandbox;

namespace kodi
{

void Log(const AddonLog loglevel, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  const std::string str = ::kodi::tools::StringUtils::FormatV(format, args);
  va_end(args);

  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_Log));
  msgpack::pack(in, msgParent__IN_kodi_Log(loglevel, str));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string GetAddonPath(const std::string& append)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetAddonPath));
  msgpack::pack(in, msgParent__IN_kodi_GetAddonPath(append));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetAddonPath t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetBaseUserPath(const std::string& append)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetBaseUserPath));
  msgpack::pack(in, msgParent__IN_kodi_GetBaseUserPath(append));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetBaseUserPath t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetTempAddonPath(const std::string& append)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetTempAddonPath));
  msgpack::pack(in, msgParent__IN_kodi_GetTempAddonPath(append));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetTempAddonPath t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetSettingString(const std::string& settingName, const std::string& defaultValue)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetSettingString));
  msgpack::pack(in, msgParent__IN_kodi_GetSettingString(settingName, defaultValue));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetSettingString t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void SetSettingString(const std::string& settingName, const std::string& settingValue)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_SetSettingString));
  msgpack::pack(in, msgParent__IN_kodi_SetSettingString(settingName, settingValue));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int GetSettingInt(const std::string& settingName, int defaultValue)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetSettingInt));
  msgpack::pack(in, msgParent__IN_kodi_GetSettingInt(settingName, defaultValue));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetSettingInt t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void SetSettingInt(const std::string& settingName, int settingValue)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_SetSettingInt));
  msgpack::pack(in, msgParent__IN_kodi_SetSettingInt(settingName, settingValue));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool GetSettingBoolean(const std::string& settingName, bool defaultValue)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetSettingBoolean));
  msgpack::pack(in, msgParent__IN_kodi_GetSettingBoolean(settingName, defaultValue));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetSettingBoolean t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void SetSettingBoolean(const std::string& settingName, bool settingValue)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_SetSettingBoolean));
  msgpack::pack(in, msgParent__IN_kodi_SetSettingBoolean(settingName, settingValue));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

template<typename enumType>
enumType GetSettingEnum(const std::string& settingName, enumType defaultValue)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetSettingInt));
  msgpack::pack(in, msgParent__IN_kodi_GetSettingInt(settingName, static_cast<int>(defaultValue)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetSettingInt t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

template<typename enumType>
void SetSettingEnum(const std::string& settingName, enumType settingValue)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_SetSettingInt));
  msgpack::pack(in, msgParent__IN_kodi_SetSettingInt(settingName, static_cast<int>(settingValue)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string GetAddonInfo(const std::string& id)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetAddonInfo));
  msgpack::pack(in, msgParent__IN_kodi_GetAddonInfo(id));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetAddonInfo t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool OpenSettings()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_OpenSettings));
  msgpack::pack(in, msgParent__IN_kodi_OpenSettings(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_OpenSettings t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetLocalizedString(uint32_t labelId, const std::string& defaultStr)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetLocalizedString));
  msgpack::pack(in, msgParent__IN_kodi_GetLocalizedString(labelId, defaultStr));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetLocalizedString t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool UnknownToUTF8(const std::string& stringSrc, std::string& utf8StringDst, bool failOnBadChar)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_UnknownToUTF8));
  msgpack::pack(in, msgParent__IN_kodi_UnknownToUTF8(stringSrc, utf8StringDst, failOnBadChar));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_UnknownToUTF8 t = ident.get().as<decltype(t)>();
  utf8StringDst = std::get<1>(t);
  return std::get<0>(t);
}

std::string GetLanguage(LangFormats format, bool region)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetLanguage));
  msgpack::pack(in, msgParent__IN_kodi_GetLanguage(uint32_t(format), region));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetLanguage t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void QueueFormattedNotification(QueueMsg type, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  const std::string str = ::kodi::tools::StringUtils::FormatV(format, args);
  va_end(args);
  QueueNotification(type, "", str, "", 5000, false, 1000);
}

void QueueNotification(QueueMsg type,
                       const std::string& header,
                       const std::string& message,
                       const std::string& imageFile,
                       unsigned int displayTime,
                       bool withSound,
                       unsigned int messageTime)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_QueueNotification));
  msgpack::pack(in, msgParent__IN_kodi_QueueNotification(uint32_t(type), header, message, imageFile,
                                                         displayTime, withSound, messageTime));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string GetMD5(const std::string& text)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetMD5));
  msgpack::pack(in, msgParent__IN_kodi_GetMD5(text));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetMD5 t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetRegion(const std::string& id)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetRegion));
  msgpack::pack(in, msgParent__IN_kodi_GetRegion(id));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetRegion t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void GetFreeMem(int64_t& free, int64_t& total, bool asBytes)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetFreeMem));
  msgpack::pack(in, msgParent__IN_kodi_GetFreeMem(free, total, asBytes));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetFreeMem t = ident.get().as<decltype(t)>();
  free = std::get<0>(t);
  total = std::get<1>(t);
}

int GetGlobalIdleTime()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetGlobalIdleTime));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetGlobalIdleTime t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetCurrentSkinId()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetCurrentSkinId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetCurrentSkinId t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsAddonAvailable(const std::string& id, std::string& version, bool& enabled)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_IsAddonAvailable));
  msgpack::pack(in, msgParent__IN_kodi_IsAddonAvailable(id, version, enabled));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_IsAddonAvailable t = ident.get().as<decltype(t)>();
  version = std::get<1>(t);
  enabled = std::get<2>(t);
  return std::get<0>(t);
}

void KodiVersion(kodi_version_t& version)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_KodiVersion));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_KodiVersion t = ident.get().as<decltype(t)>();
  version.compile_name = std::get<0>(t);
  version.major = std::get<1>(t);
  version.major = std::get<2>(t);
  version.revision = std::get<3>(t);
  version.tag = std::get<4>(t);
  version.tag_revision = std::get<5>(t);
}

bool GetKeyboardLayout(int modifierKey,
                       std::string& layout_name,
                       std::vector<std::vector<std::string>>& layout)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_GetKeyboardLayout));
  msgpack::pack(in, msgParent__IN_kodi_GetKeyboardLayout(modifierKey, layout_name, layout));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_GetKeyboardLayout t = ident.get().as<decltype(t)>();
  layout_name = std::get<1>(t);
  layout = std::get<2>(t);
  return std::get<0>(t);
}

bool ChangeKeyboardLayout(std::string& layout_name)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_General_h, funcParent_General_h::kodi_ChangeKeyboardLayout));
  msgpack::pack(in, msgParent__IN_kodi_ChangeKeyboardLayout(layout_name));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_ChangeKeyboardLayout t = ident.get().as<decltype(t)>();
  layout_name = std::get<1>(t);
  return std::get<0>(t);
}

} /* namespace kodi */
