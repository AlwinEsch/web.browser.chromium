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
namespace gui
{
namespace dialogs
{
namespace OK
{

void ShowAndGetInput(const std::string& heading, const std::string& text);
void ShowAndGetInput(const std::string& heading,
                     const std::string& line0,
                     const std::string& line1,
                     const std::string& line2);

} /* namespace OK */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
