/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../include/kodi/Network.h"

#include "../../include/kodi/Processor.h"
#include "../sandbox/ShareProcessTransmitter.h"
#include "../sandbox/kodi/Network.h"

#include <kodi/tools/StringUtils.h>

using namespace kodi::sandbox;

namespace kodi
{
namespace network
{

bool WakeOnLan(const std::string& mac)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_WakeOnLan));
  msgpack::pack(in, msgParent__IN_kodi_network_WakeOnLan(mac));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_WakeOnLan t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetIPAddress()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_GetIPAddress));
  msgpack::pack(in, msgParent__IN_kodi_network_GetIPAddress(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_GetIPAddress t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetHostname()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_GetHostname));
  msgpack::pack(in, msgParent__IN_kodi_network_GetHostname(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_GetHostname t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string GetUserAgent()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_GetUserAgent));
  msgpack::pack(in, msgParent__IN_kodi_network_GetUserAgent(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_GetUserAgent t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsLocalHost(const std::string& hostname)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_WakeOnLan));
  msgpack::pack(in, msgParent__IN_kodi_network_WakeOnLan(hostname));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_WakeOnLan t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsHostOnLAN(const std::string& hostname, bool offLineCheck)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_IsHostOnLAN));
  msgpack::pack(in, msgParent__IN_kodi_network_IsHostOnLAN(hostname, offLineCheck));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_IsHostOnLAN t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

std::string URLEncode(const std::string& url)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_URLEncode));
  msgpack::pack(in, msgParent__IN_kodi_network_URLEncode(url));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_URLEncode t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool DNSLookup(const std::string& hostName, std::string& ipAddress)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_Network_h, funcParent_Network_h::kodi_network_DNSLookup));
  msgpack::pack(in, msgParent__IN_kodi_network_DNSLookup(hostName, ipAddress));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_network_DNSLookup t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace network */
} /* namespace kodi */
