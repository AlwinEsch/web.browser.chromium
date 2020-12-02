/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "include/wrapper/cef_library_loader.h"

#include <string>

class CCEFLibraryLoader
{
public:
  CCEFLibraryLoader() = default;
  ~CCEFLibraryLoader();

  ///
  // Load the CEF framework in the main process from the expected app
  // bundle location relative to the executable. Returns true if the
  // load succeeds.
  ///
  bool LoadInMain(const std::string& mainPath = "") { return Load(false, mainPath); }

  ///
  // Load the CEF framework in the helper process from the expected app
  // bundle location relative to the executable. Returns true if the
  // load succeeds.
  ///
  bool LoadInHelper(const std::string& mainPath = "") { return Load(true, mainPath); }

private:
  bool Load(bool helper, const std::string& mainPath);

  bool m_loaded{false};
  DISALLOW_COPY_AND_ASSIGN(CCEFLibraryLoader);
};
