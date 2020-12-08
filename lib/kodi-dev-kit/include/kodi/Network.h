/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <string>

namespace kodi
{
namespace network
{

bool WakeOnLan(const std::string& mac);
std::string GetIPAddress();
std::string GetHostname();
std::string GetUserAgent();
bool IsLocalHost(const std::string& hostname);
bool IsHostOnLAN(const std::string& hostname, bool offLineCheck = false);
std::string URLEncode(const std::string& url);
bool DNSLookup(const std::string& hostName, std::string& ipAddress);

} /* namespace network */
} /* namespace kodi */
