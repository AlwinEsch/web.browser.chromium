/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_browser.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

void ShowSSLInformation(CefRefPtr<CefBrowser> browser);

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
