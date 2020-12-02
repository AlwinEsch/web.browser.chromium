/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Network.h"

#include "../../sandbox/kodi/Network.h"
#include "../utils/Log.h"

#include <kodi/Network.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool CNetwork_h::HandleMessage(int funcGroup,
                               int func,
                               const msgpack::unpacked& in,
                               msgpack::sbuffer& out,
                               const char* data,
                               std::size_t size,
                               std::size_t offset)
{
  if (funcGroup != funcGroup_Network_h)
    return false;

  switch (func)
  {
    case funcParent_Network_h::kodi_network_WakeOnLan:
    {
      msgParent__IN_kodi_network_WakeOnLan t = in.get().as<decltype(t)>();
      msgpack::pack(out,
                    msgParent_OUT_kodi_network_WakeOnLan(kodi::network::WakeOnLan(std::get<0>(t))));
      break;
    }
    case funcParent_Network_h::kodi_network_GetIPAddress:
    {
      msgParent__IN_kodi_network_GetIPAddress t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_network_GetIPAddress(kodi::network::GetIPAddress()));
      break;
    }
    case funcParent_Network_h::kodi_network_GetHostname:
    {
      msgParent__IN_kodi_network_GetHostname t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_network_GetHostname(kodi::network::GetHostname()));
      break;
    }
    case funcParent_Network_h::kodi_network_GetUserAgent:
    {
      msgParent__IN_kodi_network_GetUserAgent t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_network_GetUserAgent(kodi::network::GetUserAgent()));
      break;
    }
    case funcParent_Network_h::kodi_network_IsLocalHost:
    {
      msgParent__IN_kodi_network_IsLocalHost t = in.get().as<decltype(t)>();
      msgpack::pack(
          out, msgParent_OUT_kodi_network_IsLocalHost(kodi::network::IsLocalHost(std::get<0>(t))));
      break;
    }
    case funcParent_Network_h::kodi_network_IsHostOnLAN:
    {
      msgParent__IN_kodi_network_IsHostOnLAN t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_network_IsHostOnLAN(
                             kodi::network::IsHostOnLAN(std::get<0>(t), std::get<1>(t))));
      break;
    }
    case funcParent_Network_h::kodi_network_URLEncode:
    {
      msgParent__IN_kodi_network_URLEncode t = in.get().as<decltype(t)>();
      msgpack::pack(out,
                    msgParent_OUT_kodi_network_URLEncode(kodi::network::URLEncode(std::get<0>(t))));
      break;
    }
    case funcParent_Network_h::kodi_network_DNSLookup:
    {
      msgParent__IN_kodi_network_DNSLookup t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_network_DNSLookup(
                             kodi::network::DNSLookup(std::get<0>(t), std::get<1>(t))));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "CNetwork_h::%s: addon called with unknown function id '%i' on group 'kodi'", __func__,
          func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */
