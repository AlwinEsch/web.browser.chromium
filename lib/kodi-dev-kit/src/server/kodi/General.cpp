/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "General.h"

#include "../../sandbox/kodi/General.h"
#include "../utils/Log.h"

#include <kodi/General.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool CGeneral_h::HandleMessage(int funcGroup,
                               int func,
                               const msgpack::unpacked& in,
                               msgpack::sbuffer& out,
                               const char* data,
                               std::size_t size,
                               std::size_t offset)
{
  if (funcGroup != funcGroup_General_h)
    return false;

  switch (func)
  {
    case funcParent_General_h::kodi_Log:
    {
      msgParent__IN_kodi_Log t = in.get().as<decltype(t)>();
      kodi::utils::LOG_MESSAGE(AddonLog(std::get<0>(t)), "CHILD: %s", std::get<1>(t).c_str());
      break;
    }
    case funcParent_General_h::kodi_GetAddonPath:
    {
      msgParent__IN_kodi_GetAddonPath t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetAddonPath(kodi::GetAddonPath(std::get<0>(t))));
      break;
    }
    case funcParent_General_h::kodi_GetBaseUserPath:
    {
      msgParent__IN_kodi_GetBaseUserPath t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetBaseUserPath(kodi::GetBaseUserPath(std::get<0>(t))));
      break;
    }
    case funcParent_General_h::kodi_GetSettingBoolean:
    {
      msgParent__IN_kodi_GetSettingBoolean t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetSettingBoolean(
                             kodi::GetSettingBoolean(std::get<0>(t), std::get<1>(t))));
      break;
    }
    case funcParent_General_h::kodi_SetSettingBoolean:
    {
      msgParent__IN_kodi_SetSettingBoolean t = in.get().as<decltype(t)>();
      kodi::SetSettingBoolean(std::get<0>(t), std::get<1>(t));
      break;
    }
    case funcParent_General_h::kodi_GetSettingInt:
    {
      msgParent__IN_kodi_GetSettingInt t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetSettingInt(
                             kodi::GetSettingInt(std::get<0>(t), std::get<1>(t))));
      break;
    }
    case funcParent_General_h::kodi_SetSettingInt:
    {
      msgParent__IN_kodi_SetSettingInt t = in.get().as<decltype(t)>();
      kodi::SetSettingInt(std::get<0>(t), std::get<1>(t));
      break;
    }
    case funcParent_General_h::kodi_GetSettingString:
    {
      msgParent__IN_kodi_GetSettingString t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetSettingString(
                             kodi::GetSettingString(std::get<0>(t), std::get<1>(t))));
      break;
    }
    case funcParent_General_h::kodi_SetSettingString:
    {
      msgParent__IN_kodi_SetSettingString t = in.get().as<decltype(t)>();
      kodi::SetSettingString(std::get<0>(t), std::get<1>(t));
      break;
    }
    case funcParent_General_h::kodi_GetAddonInfo:
    {
      msgParent__IN_kodi_GetAddonInfo t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetAddonInfo(kodi::GetAddonInfo(std::get<0>(t))));
      break;
    }
    case funcParent_General_h::kodi_OpenSettings:
    {
      msgpack::pack(out, msgParent_OUT_kodi_OpenSettings(kodi::OpenSettings()));
      break;
    }
    case funcParent_General_h::kodi_GetLocalizedString:
    {
      msgParent__IN_kodi_GetLocalizedString t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetLocalizedString(
                             kodi::GetLocalizedString(std::get<0>(t), std::get<1>(t))));
      break;
    }
    case funcParent_General_h::kodi_UnknownToUTF8:
    {
      msgParent__IN_kodi_UnknownToUTF8 t = in.get().as<decltype(t)>();
      std::string& stringSrc(std::get<0>(t));
      std::string& utf8StringDst(std::get<1>(t));
      bool ret = kodi::UnknownToUTF8(stringSrc, utf8StringDst, std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_UnknownToUTF8(ret, utf8StringDst));
      break;
    }
    case funcParent_General_h::kodi_GetLanguage:
    {
      msgParent__IN_kodi_GetLanguage t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetLanguage(
                             kodi::GetLanguage(LangFormats(std::get<0>(t)), std::get<1>(t))));
      break;
    }
    case funcParent_General_h::kodi_QueueNotification:
    {
      msgParent__IN_kodi_QueueNotification t = in.get().as<decltype(t)>();
      kodi::QueueNotification(QueueMsg(std::get<0>(t)), std::get<1>(t), std::get<2>(t),
                              std::get<3>(t), std::get<4>(t), std::get<5>(t), std::get<6>(t));
      break;
    }
    case funcParent_General_h::kodi_GetMD5:
    {
      msgParent__IN_kodi_GetMD5 t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_GetMD5(kodi::GetMD5(std::get<0>(t))));
      break;
    }
    case funcParent_General_h::kodi_GetTempAddonPath:
    {
      msgParent__IN_kodi_GetTempAddonPath t = in.get().as<decltype(t)>();
      msgpack::pack(out,
                    msgParent_OUT_kodi_GetTempAddonPath(kodi::GetTempAddonPath(std::get<0>(t))));
      break;
    }
    case funcParent_General_h::kodi_GetRegion:
    {
      msgParent__IN_kodi_GetRegion t = in.get().as<decltype(t)>();
      const std::string& id(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_GetRegion(kodi::GetRegion(id)));
      break;
    }
    case funcParent_General_h::kodi_GetFreeMem:
    {
      msgParent__IN_kodi_GetFreeMem t = in.get().as<decltype(t)>();
      long free(std::get<0>(t));
      long total(std::get<1>(t));
      kodi::GetFreeMem(free, total, std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_GetFreeMem(int64_t(free), int64_t(total)));
      break;
    }
    case funcParent_General_h::kodi_GetGlobalIdleTime:
    {
      msgpack::pack(out, msgParent_OUT_kodi_GetGlobalIdleTime(kodi::GetGlobalIdleTime()));
      break;
    }
    case funcParent_General_h::kodi_GetCurrentSkinId:
    {
      msgpack::pack(out, msgParent_OUT_kodi_GetCurrentSkinId(kodi::GetCurrentSkinId()));
      break;
    }
    case funcParent_General_h::kodi_IsAddonAvailable:
    {
      msgParent__IN_kodi_IsAddonAvailable t = in.get().as<decltype(t)>();
      std::string& version(std::get<1>(t));
      bool& enabled(std::get<2>(t));
      bool ret = kodi::IsAddonAvailable(std::get<0>(t), version, enabled);
      msgpack::pack(out, msgParent_OUT_kodi_IsAddonAvailable(ret, version, enabled));
      break;
    }
    case funcParent_General_h::kodi_KodiVersion:
    {
      kodi_version_t version;
      kodi::KodiVersion(version);
      msgpack::pack(out, msgParent_OUT_kodi_KodiVersion(version.compile_name, version.major,
                                                        version.minor, version.revision,
                                                        version.tag, version.tag_revision));
      break;
    }
    case funcParent_General_h::kodi_GetKeyboardLayout:
    {
      msgParent__IN_kodi_GetKeyboardLayout t = in.get().as<decltype(t)>();
      std::string& layout_name(std::get<1>(t));
      std::vector<std::vector<std::string>>& layout(std::get<2>(t));
      bool ret = kodi::GetKeyboardLayout(std::get<0>(t), layout_name, layout);
      msgpack::pack(out, msgParent_OUT_kodi_GetKeyboardLayout(ret, layout_name, layout));
      break;
    }
    case funcParent_General_h::kodi_ChangeKeyboardLayout:
    {
      msgParent__IN_kodi_ChangeKeyboardLayout t = in.get().as<decltype(t)>();
      std::string& layout_name(std::get<0>(t));
      bool ret = kodi::ChangeKeyboardLayout(layout_name);
      msgpack::pack(out, msgParent_OUT_kodi_ChangeKeyboardLayout(ret, layout_name));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "CGeneral_h::%s: addon called with unknown function id '%i' on group 'kodi'", __func__,
          func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */
