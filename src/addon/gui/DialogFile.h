/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_dialog_handler.h"

class CBrowserDialogFile : public CefDialogHandler
{
public:
  CBrowserDialogFile() = default;

  bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                    FileDialogMode mode,
                    const CefString& title,
                    const CefString& default_file_path,
                    const std::vector<CefString>& accept_filters,
                    int selected_accept_filter,
                    CefRefPtr<CefFileDialogCallback> callback) override;

private:
  static void Process(FileDialogMode mode,
                      std::string heading,
                      std::string default_file_path,
                      std::string mask,
                      int selected_accept_filter,
                      CefRefPtr<CefFileDialogCallback> callback);

  IMPLEMENT_REFCOUNTING(CBrowserDialogFile);
  DISALLOW_COPY_AND_ASSIGN(CBrowserDialogFile);
};
