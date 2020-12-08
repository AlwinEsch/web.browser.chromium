/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../../sandbox/kodi/gui/dialogs/FileBrowser.h"

#include "../../../utils/Log.h"
#include "FileBrowser.h"

#include <kodi/gui/dialogs/FileBrowser.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_FileBrowser_h::HandleMessage(int funcGroup,
                                                int func,
                                                const msgpack::unpacked& in,
                                                msgpack::sbuffer& out,
                                                const char* data,
                                                std::size_t size,
                                                std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_FileBrowser_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory:
    {
      msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory t = in.get().as<decltype(t)>();
      std::string path = std::get<2>(t);
      bool ret = kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory(
          std::get<0>(t), std::get<1>(t), path, std::get<3>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory(ret, path));
      break;
    }
    case kodi_gui_dialogs_FileBrowser_ShowAndGetFile:
    {
      msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFile t = in.get().as<decltype(t)>();
      std::string path = std::get<3>(t);
      bool ret = kodi::gui::dialogs::FileBrowser::ShowAndGetFile(
          std::get<0>(t), std::get<1>(t), std::get<2>(t), path, std::get<4>(t), std::get<5>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFile(ret, path));
      break;
    }
    case kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir:
    {
      msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir t =
          in.get().as<decltype(t)>();
      std::string path = std::get<3>(t);
      bool ret = kodi::gui::dialogs::FileBrowser::ShowAndGetFileFromDir(
          std::get<0>(t), std::get<1>(t), std::get<2>(t), path, std::get<4>(t), std::get<5>(t),
          std::get<6>(t));
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir(ret, path));
      break;
    }
    case kodi_gui_dialogs_FileBrowser_ShowAndGetFileList:
    {
      msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFileList t = in.get().as<decltype(t)>();
      std::vector<std::string> fileList = std::get<3>(t);
      bool ret = kodi::gui::dialogs::FileBrowser::ShowAndGetFileList(
          std::get<0>(t), std::get<1>(t), std::get<2>(t), fileList, std::get<4>(t), std::get<5>(t));
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFileList(ret, fileList));
      break;
    }
    case kodi_gui_dialogs_FileBrowser_ShowAndGetSource:
    {
      msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetSource t = in.get().as<decltype(t)>();
      std::string path = std::get<0>(t);
      bool ret = kodi::gui::dialogs::FileBrowser::ShowAndGetSource(path, std::get<1>(t),
                                                                   std::get<2>(t), std::get<3>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetSource(ret, path));
      break;
    }
    case kodi_gui_dialogs_FileBrowser_ShowAndGetImage:
    {
      msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetImage t = in.get().as<decltype(t)>();
      std::string path = std::get<2>(t);
      bool ret =
          kodi::gui::dialogs::FileBrowser::ShowAndGetImage(std::get<0>(t), std::get<1>(t), path);
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetImage(ret, path));
      break;
    }
    case kodi_gui_dialogs_FileBrowser_ShowAndGetImageList:
    {
      msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetImageList t = in.get().as<decltype(t)>();
      std::vector<std::string> imageList = std::get<2>(t);
      bool ret = kodi::gui::dialogs::FileBrowser::ShowAndGetImageList(std::get<0>(t),
                                                                      std::get<1>(t), imageList);
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetImageList(ret, imageList));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_FileBrowser_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
