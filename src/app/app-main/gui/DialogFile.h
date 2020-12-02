/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_dialog_handler.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

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

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
