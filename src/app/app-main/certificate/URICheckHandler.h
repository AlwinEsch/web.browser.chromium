/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_browser.h"

// Global
#include <string>

namespace chromium
{
namespace app
{
namespace main
{
namespace certificate
{

class CURICheck
{
public:
  static void LoadErrorPage(CefRefPtr<CefFrame> frame,
                            const std::string& failed_url,
                            cef_errorcode_t error_code,
                            const std::string& other_info);
  static std::string GetContentStatusString(cef_ssl_content_status_t status);
  static std::string GetCertStatusString(cef_cert_status_t status);
  static std::string GetSSLVersionString(cef_ssl_version_t version);
  static std::string GetErrorString(
      cef_errorcode_t code); //!< Returns the string representation of the specified error code.
  static std::string GetDataURI(
      const std::string& data,
      const std::string& mime_type); //!< Returns a data: URI with the specified contents.
  static std::string GetCertificateInformation(CefRefPtr<CefX509Certificate> cert,
                                               cef_cert_status_t certstatus);
  static std::string GetTimeString(const CefTime& value);
  static std::string GetBinaryString(CefRefPtr<CefBinaryValue> value);
};

} /* namespace certificate */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */

