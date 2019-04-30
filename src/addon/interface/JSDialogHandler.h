/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_jsdialog_handler.h"

class CWebBrowserClient;

class CJSDialogHandler : public CefJSDialogHandler
{
public:
  CJSDialogHandler(CWebBrowserClient* client) : m_client(client) { }

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

private:
  IMPLEMENT_REFCOUNTING(CJSDialogHandler);

  static void OnJSDialogProcess(std::string origin_url,
                                JSDialogType dialog_type,
                                std::string message_text,
                                std::string default_prompt_text,
                                CefRefPtr<CefJSDialogCallback> callback);

  CWebBrowserClient* m_client;
};
