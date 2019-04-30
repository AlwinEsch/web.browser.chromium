/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_context_menu_handler.h"

#include <string>
#include <vector>

class CWebBrowserClient;

class CBrowerDialogContextMenu : public CefContextMenuHandler
{
public:
  CBrowerDialogContextMenu(CWebBrowserClient* client);

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


private:
  static void RunContextMenuProcess(CWebBrowserClient* client,
                                    std::vector<std::pair<int, std::string>> entries,
                                    CefRefPtr<CefRunContextMenuCallback> callback);

  IMPLEMENT_REFCOUNTING(CBrowerDialogContextMenu);
  DISALLOW_COPY_AND_ASSIGN(CBrowerDialogContextMenu);

  CWebBrowserClient* m_client;
};
