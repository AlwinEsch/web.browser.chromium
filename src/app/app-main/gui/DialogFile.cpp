/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogFile.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/FileBrowser.h"

// Global
#include <thread>

/*
TODO:
Seems final here.
*/

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

bool CBrowserDialogFile::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                      FileDialogMode mode,
                                      const CefString& title,
                                      const CefString& default_file_path,
                                      const std::vector<CefString>& accept_filters,
                                      int selected_accept_filter,
                                      CefRefPtr<CefFileDialogCallback> callback)
{
  std::string mask;
  for (const auto& filter : accept_filters)
    mask += filter.ToString() + "|";

  std::thread(Process, mode, title.ToString(), default_file_path.ToString(), mask,
              selected_accept_filter, callback)
      .detach();
  return true;
}

void CBrowserDialogFile::Process(FileDialogMode mode,
                                 std::string heading,
                                 std::string default_file_path,
                                 std::string mask,
                                 int selected_accept_filter,
                                 CefRefPtr<CefFileDialogCallback> callback)
{
  bool ret = false;
  std::string shares = "files|local|removable|music|video|pictures";
  std::vector<CefString> file_paths;

  int selectedMode = mode & FILE_DIALOG_TYPE_MASK;
  switch (selectedMode)
  {
    case FILE_DIALOG_OPEN_MULTIPLE:
    {
      if (heading.empty())
        heading = kodi::GetLocalizedString(30020);

      std::vector<std::string> fileList;
      ret = kodi::gui::dialogs::FileBrowser::ShowAndGetFileList(shares, mask, heading, fileList);
      if (ret)
      {
        for (const auto& file : fileList)
          file_paths.push_back(file);
      }
    }
    break;

    case FILE_DIALOG_OPEN_FOLDER:
    {
      if (heading.empty())
        heading = kodi::GetLocalizedString(30021);

      std::string path = default_file_path;
      ret = kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory(shares, heading, path, false);
      if (ret)
        file_paths.push_back(path);
    }
    break;

    case FILE_DIALOG_SAVE:
    {
      if (heading.empty())
        heading = kodi::GetLocalizedString(30022);

      std::string path = default_file_path;
      ret = kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory(shares, heading, path, true);
      if (ret)
        file_paths.push_back(path);
    }
    break;

    default:
    {
      if (heading.empty())
        heading = kodi::GetLocalizedString(30019);

      std::string path = default_file_path;
      ret = kodi::gui::dialogs::FileBrowser::ShowAndGetFile(shares, mask, heading, path);
      if (ret)
        file_paths.push_back(path);
    }
    break;
  }

  if (ret)
    callback->Continue(selected_accept_filter, file_paths);
  else
    callback->Cancel();
}

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
