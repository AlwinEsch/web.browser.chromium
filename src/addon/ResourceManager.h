/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/wrapper/cef_resource_manager.h"

#include <string>

namespace ResourceManager
{

void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager);
std::string KodiURLFilter(const std::string& url);

} /* namespace ResourceManager */
