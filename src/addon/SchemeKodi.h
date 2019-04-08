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

#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_request.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

// Implementation of the schema handler for client:// requests.
class CSchemeKodiHandler : public CefResourceHandler
{
 public:
  CSchemeKodiHandler() : m_offset(0) {}

  bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) override;
  void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) override;
  void Cancel() override { CEF_REQUIRE_IO_THREAD(); }
  bool ReadResponse(void* m_dataout, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) override;

 private:
  std::string m_data;
  std::string m_mime_type;
  size_t m_offset;

  IMPLEMENT_REFCOUNTING(CSchemeKodiHandler);
};

// Implementation of the factory for for creating schema handlers.
class CSchemeKodiFactory : public CefSchemeHandlerFactory
{
 public:
  // Return a new scheme handler instance to handle the request.
  CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                       const CefString& scheme_name, CefRefPtr<CefRequest> request) override
  {
    CEF_REQUIRE_IO_THREAD();
    return new CSchemeKodiHandler();
  }

  IMPLEMENT_REFCOUNTING(CSchemeKodiFactory);
};


