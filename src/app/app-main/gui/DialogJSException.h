/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_process_message.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

namespace JSException
{

void ReportJSException(CefRefPtr<CefProcessMessage> message);

} /* namespace JSException */

} /* gui */
} /* main */
} /* app */
} /* chromium */
