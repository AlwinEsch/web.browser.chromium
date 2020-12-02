/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <string>
#include <vector>

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace ContextMenu
{

int Show(const std::string& heading, const std::vector<std::string>& entries);
int Show(const std::string& heading,
         const std::vector<std::pair<std::string, std::string>>& entries);
int Show(const std::string& heading, const std::vector<std::pair<int, std::string>>& entries);

} /* namespace ContextMenu */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
