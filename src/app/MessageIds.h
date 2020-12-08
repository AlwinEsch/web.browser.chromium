/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

struct RendererMessage
{
  static const std::string FocusedNodeChanged;
  static const std::string OnUncaughtException;
};

struct BrowserMessage
{
  static const std::string dummy;
};

struct SettingValues
{
  typedef enum securityWebaddonAccess
  {
    webaddonAccess_Off = 0,
    webaddonAccess_LocalOnly = 1,
    webaddonAccess_LocalAndKnown = 2,
    webaddonAccess_Everyone = 3
  } securityWebaddonAccess;

  static const std::string security_webaddon_access;
};
