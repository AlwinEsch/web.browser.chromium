/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_resource_handler.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace scheme
{

class CSchemeKodiHandler : public CefResourceHandler
{
public:
  CSchemeKodiHandler() = default;
  ~CSchemeKodiHandler() = default;

  bool Open(CefRefPtr<CefRequest> request,
            bool& handle_request,
            CefRefPtr<CefCallback> callback) override;
  void GetResponseHeaders(CefRefPtr<CefResponse> response,
                          int64& response_length,
                          CefString& redirectUrl) override;
  void Cancel() override { CEF_REQUIRE_IO_THREAD(); }
  bool Read(void* m_dataout,
            int bytes_to_read,
            int& bytes_read,
            CefRefPtr<CefResourceReadCallback> callback) override;

private:
  std::string m_data;
  std::string m_mime_type;
  size_t m_offset;

  IMPLEMENT_REFCOUNTING(CSchemeKodiHandler);
  DISALLOW_COPY_AND_ASSIGN(CSchemeKodiHandler);
};

} /* namespace scheme */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
