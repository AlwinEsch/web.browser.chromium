#pragma once
/*
 *      Copyright (C) 2015 Team KODI
 *      http:/kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kodi/libXBMC_addon.h"
#include "kodi/libKODI_web.h"
#include "kodi/guilib-v2/GUILib.h"

class CWebBrowserManager;

extern std::string                   g_strTempPath;
extern std::string                   g_strLogPath;
extern std::string                   g_strUserPath;
extern std::string                   g_strAddonLibPath;
extern std::string                   g_strAddonSharePath;
extern CWebBrowserManager           *g_pWebManager;
extern ADDON::CHelper_libXBMC_addon *KODI;
extern CHelper_libKODI_guilib       *GUI;
extern CHelper_libKODI_web          *WEB;
