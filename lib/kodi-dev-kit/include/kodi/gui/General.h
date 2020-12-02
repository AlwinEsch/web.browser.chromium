/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "../c-api/addon_base.h"

namespace kodi
{

using HardwareContext = ADDON_HARDWARE_CONTEXT;

namespace gui
{

void Lock();
void Unlock();
int GetScreenHeight();
int GetScreenWidth();
int GetVideoResolution();
int GetCurrentWindowDialogId();
int GetCurrentWindowId();
kodi::HardwareContext GetHWContext();

} /* namespace gui */
} /* namespace kodi */
