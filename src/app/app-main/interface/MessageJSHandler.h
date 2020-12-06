/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/wrapper/cef_message_router.h"

namespace chromium
{
namespace app
{
namespace main
{

class CWebBrowserClient;

namespace interface
{

class CMessageJSHandler : public CefMessageRouterBrowserSide::Handler
{
public:
  CMessageJSHandler(CefRefPtr<CWebBrowserClient> client);
  virtual ~CMessageJSHandler();

  bool OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id, const CefString& request,
               bool persistent, CefRefPtr<Callback> callback) override;
  void OnQueryCanceled(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id) override;

private:
  CefRefPtr<CWebBrowserClient> m_client;
};

} /* namespace interface */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
