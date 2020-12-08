/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

namespace chromium
{
namespace app
{
namespace utils
{

bool UnsuedBlacklistedChromiumString(int stringId);
void PrintCEFInfos(int argc, char* argv[], bool isChild);
bool IsImageURL(std::string url);

} /* namespace utils */
} /* namespace app */
} /* namespace chromium */
