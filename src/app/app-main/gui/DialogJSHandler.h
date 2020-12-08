/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_jsdialog_handler.h"

namespace chromium
{
namespace app
{
namespace main
{

class CWebBrowserClient;

namespace gui
{

class CJSDialogHandler : public CefJSDialogHandler
{
public:
  CJSDialogHandler(CefRefPtr<CWebBrowserClient> client) : m_client(client) {}

  bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                  const CefString& origin_url,
                  JSDialogType dialog_type,
                  const CefString& message_text,
                  const CefString& default_prompt_text,
                  CefRefPtr<CefJSDialogCallback> callback,
                  bool& suppress_message) override;
  bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
                            const CefString& message_text,
                            bool is_reload,
                            CefRefPtr<CefJSDialogCallback> callback) override;
  void OnResetDialogState(CefRefPtr<CefBrowser> browser) override;

private:
  IMPLEMENT_REFCOUNTING(CJSDialogHandler);
  DISALLOW_COPY_AND_ASSIGN(CJSDialogHandler);

  static void OnJSDialogProcess(std::string origin_url,
                                JSDialogType dialog_type,
                                std::string message_text,
                                std::string default_prompt_text,
                                CefRefPtr<CefJSDialogCallback> callback);

  CefRefPtr<CWebBrowserClient> m_client;
};

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
