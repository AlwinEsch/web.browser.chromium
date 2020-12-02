/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/dialogs/filebrowser.h"

#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_dialogs_FileBrowser_h : int
{
  kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory,
  kodi_gui_dialogs_FileBrowser_ShowAndGetFile,
  kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir,
  kodi_gui_dialogs_FileBrowser_ShowAndGetFileList,
  kodi_gui_dialogs_FileBrowser_ShowAndGetSource,
  kodi_gui_dialogs_FileBrowser_ShowAndGetImage,
  kodi_gui_dialogs_FileBrowser_ShowAndGetImageList,
  funcParent_gui_dialogs_FileBrowser_h_LAST
} funcParent_gui_dialogs_FileBrowser_h;

// clang-format off
typedef std::tuple<std::string, std::string, std::string, bool>                   msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory;
typedef std::tuple<std::string, std::string, std::string, std::string, bool, bool> msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFile;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFile;
typedef std::tuple<std::string, std::string, std::string, std::string, bool, bool, bool> msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir;
typedef std::tuple<std::string, std::string, std::string, std::vector<std::string>, bool, bool> msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFileList;
typedef std::tuple<bool, std::vector<std::string>>                                msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFileList;
typedef std::tuple<std::string, bool, std::string, std::string>                   msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetSource;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetSource;
typedef std::tuple<std::string, std::string, std::string>                         msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetImage;
typedef std::tuple<bool, std::string>                                             msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetImage;
typedef std::tuple<std::string, std::string, std::vector<std::string>>            msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetImageList;
typedef std::tuple<bool, std::vector<std::string>>                                msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetImageList;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_dialogs_FileBrowser_h
{
  funcChild_gui_dialogs_FileBrowser_h_LAST
} funcChild_gui_dialogs_FileBrowser_h;

} /* namespace sandbox */
} /* namespace kodi */
