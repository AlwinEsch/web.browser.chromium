/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/wrapper/cef_resource_manager.h"

#include <string>

namespace chromium
{
namespace app
{
namespace main
{

namespace ResourceManager
{

void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager);
std::string KodiURLFilter(const std::string& url);

} /* namespace ResourceManager */

} /* namespace main */
} /* namespace app */
} /* namespace chromium */
