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
