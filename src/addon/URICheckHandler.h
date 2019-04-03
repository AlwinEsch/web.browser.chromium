/*
 *      Copyright (C) 2015-2019 Team KODI
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

#pragma once

#include <string>

#include "include/cef_browser.h"

class CURICheck
{
public:
  static void LoadErrorPage(CefRefPtr<CefFrame> frame,
                            const std::string& failed_url,
                            cef_errorcode_t error_code,
                            const std::string& other_info);
  static std::string GetCertStatusString(cef_cert_status_t status);
  static std::string GetSSLVersionString(cef_ssl_version_t version);
  static std::string GetErrorString(cef_errorcode_t code);                              //!< Returns the string representation of the specified error code.
  static std::string GetDataURI(const std::string& data, const std::string& mime_type); //!< Returns a data: URI with the specified contents.
  static std::string GetCertificateInformation(CefRefPtr<CefX509Certificate> cert, cef_cert_status_t certstatus);
};
