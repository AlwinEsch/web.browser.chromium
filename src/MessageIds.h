/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

class RendererMessage
{
public:
  static const std::string FocusedNodeChanged;
  static const std::string V8AddonCall;
};

class BrowserMessage
{
public:
  static const std::string dummy;
};
