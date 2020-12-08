/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace Numeric
{

bool ShowAndVerifyNewPassword(std::string& newPassword);
int ShowAndVerifyPassword(const std::string& password, const std::string& heading, int retries);
bool ShowAndVerifyInput(std::string& toVerify, const std::string& heading, bool verifyInput);
bool ShowAndGetTime(tm& time, const std::string& heading);
bool ShowAndGetDate(tm& date, const std::string& heading);
bool ShowAndGetIPAddress(std::string& ipAddress, const std::string& heading);
bool ShowAndGetNumber(std::string& input,
                      const std::string& heading,
                      unsigned int autoCloseTimeoutMs = 0);
bool ShowAndGetSeconds(std::string& time, const std::string& heading);

} /* namespace Numeric */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
