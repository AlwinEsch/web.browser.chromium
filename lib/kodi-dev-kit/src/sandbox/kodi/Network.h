/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../include/kodi/c-api/network.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_Network_h : int
{
  kodi_network_WakeOnLan,
  kodi_network_GetIPAddress,
  kodi_network_GetHostname,
  kodi_network_GetUserAgent,
  kodi_network_IsLocalHost,
  kodi_network_IsHostOnLAN,
  kodi_network_URLEncode,
  kodi_network_DNSLookup,
} funcParent_Network_h;

// clang-format off
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_network_WakeOnLan;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_network_WakeOnLan;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_network_GetIPAddress;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_network_GetIPAddress;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_network_GetHostname;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_network_GetHostname;
typedef std::tuple<DummyValue>                                                    msgParent__IN_kodi_network_GetUserAgent;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_network_GetUserAgent;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_network_IsLocalHost;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_network_IsLocalHost;
typedef std::tuple<std::string, bool>                                             msgParent__IN_kodi_network_IsHostOnLAN;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_network_IsHostOnLAN;
typedef std::tuple<std::string>                                                   msgParent__IN_kodi_network_URLEncode;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_network_URLEncode;
typedef std::tuple<std::string, std::string>                                      msgParent__IN_kodi_network_DNSLookup;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_network_DNSLookup;
// clang-format on

//==============================================================================

typedef enum funcChild_Network_h
{
  funcIdAddon_unused_Network_h
} funcChild_Network_h;

} /* namespace sandbox */
} /* namespace kodi */
