/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
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

typedef struct SSelectionEntry
{
  SSelectionEntry() = default;

  std::string id;
  std::string name;
  bool selected = false;
} SSelectionEntry;

namespace Select
{

int Show(const std::string& heading,
         const std::vector<std::string>& entries,
         int selected = -1,
         unsigned int autoclose = 0);
int Show(const std::string& heading,
         std::vector<kodi::gui::dialogs::SSelectionEntry>& entries,
         int selected = -1,
         unsigned int autoclose = 0);
bool ShowMultiSelect(const std::string& heading,
                     std::vector<kodi::gui::dialogs::SSelectionEntry>& entries,
                     int autoclose = 0);

} /* namespace Select */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
