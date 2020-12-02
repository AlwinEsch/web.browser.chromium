/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_browser.h"
#include "include/cef_request_callback.h"
#include "include/cef_ssl_info.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace certificate
{

typedef cef_errorcode_t ErrorCode;

bool OnCertificateError(CefRefPtr<CefBrowser> browser,
                        ErrorCode cert_error,
                        const CefString& request_url,
                        CefRefPtr<CefSSLInfo> ssl_info,
                        CefRefPtr<CefRequestCallback> callback);

} /* namespace certificate */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
