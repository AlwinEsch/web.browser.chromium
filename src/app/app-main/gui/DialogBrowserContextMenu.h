/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_context_menu_handler.h"

// Global
#include <string>
#include <vector>

namespace chromium
{
namespace app
{
namespace main
{

class CWebBrowserClient;

namespace gui
{

class CBrowerDialogContextMenu : public CefContextMenuHandler
{
public:
  CBrowerDialogContextMenu(CefRefPtr<CWebBrowserClient> client);
  ~CBrowerDialogContextMenu();

  void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefContextMenuParams> params,
                           CefRefPtr<CefMenuModel> model) override;

  bool RunContextMenu(CefRefPtr<CefBrowser> browser,
                      CefRefPtr<CefFrame> frame,
                      CefRefPtr<CefContextMenuParams> params,
                      CefRefPtr<CefMenuModel> model,
                      CefRefPtr<CefRunContextMenuCallback> callback) override;

  bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefContextMenuParams> params,
                            int command_id,
                            EventFlags event_flags) override;

  void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame) override;

private:
  IMPLEMENT_REFCOUNTING(CBrowerDialogContextMenu);
  DISALLOW_COPY_AND_ASSIGN(CBrowerDialogContextMenu);

  bool HasSSLInformation(CefRefPtr<CefBrowser> browser);

  CefRefPtr<CWebBrowserClient> m_client;
};

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
