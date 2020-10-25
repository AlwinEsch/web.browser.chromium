/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_resource_handler.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

// Implementation of the schema handler for client:// requests.
class CSchemeKodiHandler : public CefResourceHandler
{
public:
  CSchemeKodiHandler() : m_offset(0) {}

  bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) override;
  void GetResponseHeaders(CefRefPtr<CefResponse> response,
                          int64& response_length,
                          CefString& redirectUrl) override;
  void Cancel() override { CEF_REQUIRE_IO_THREAD(); }
  bool ReadResponse(void* m_dataout,
                    int bytes_to_read,
                    int& bytes_read,
                    CefRefPtr<CefCallback> callback) override;

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
  CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       const CefString& scheme_name,
                                       CefRefPtr<CefRequest> request) override
  {
    CEF_REQUIRE_IO_THREAD();
    return new CSchemeKodiHandler();
  }

  IMPLEMENT_REFCOUNTING(CSchemeKodiFactory);
};
