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

#include <string>

#include "include/cef_browser.h"
#include "include/cef_resource_handler.h"
#include "include/cef_request.h"
#include "include/wrapper/cef_message_router.h"

class CURICheck
{
public:
  static std::string GetErrorString(cef_errorcode_t code);                              //!< Returns the string representation of the specified error code.
  static std::string GetDataURI(const std::string& data, const std::string& mime_type); //!< Returns a data: URI with the specified contents.
  static CefRefPtr<CefResourceHandler> GetResourceHandler(                              //!< Get test resources.
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request);
  static bool ParseTestUrl(                                                             //!< Retrieve the file name and mime type based on the specified url.
      const std::string& url,
      std::string* file_name,
      std::string* mime_type);
  static std::string DumpRequestContents(CefRefPtr<CefRequest> request);
  static CefRefPtr<CefStreamReader> GetBinaryResourceReader(const char* resource_name); //!< Retrieve a resource as a steam reader.
};
