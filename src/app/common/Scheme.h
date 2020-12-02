/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_scheme.h"

namespace chromium
{
namespace app
{
namespace common
{

void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar);

} /* namespace common */
} /* namespace app */
} /* namespace chromium */
