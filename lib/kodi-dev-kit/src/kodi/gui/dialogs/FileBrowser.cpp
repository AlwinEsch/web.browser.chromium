/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/FileBrowser.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/FileBrowser.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace FileBrowser
{

bool ShowAndGetDirectory(const std::string& shares,
                         const std::string& heading,
                         std::string& path,
                         bool writeOnly)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_FileBrowser_h,
                                  kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory(
                        shares, heading, path, writeOnly));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory t = ident.get().as<decltype(t)>();
  path = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetFile(const std::string& shares,
                    const std::string& mask,
                    const std::string& heading,
                    std::string& path,
                    bool useThumbs,
                    bool useFileDirectories)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_FileBrowser_h,
                                  kodi_gui_dialogs_FileBrowser_ShowAndGetFile));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFile(
                        shares, mask, heading, path, useThumbs, useFileDirectories));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFile t = ident.get().as<decltype(t)>();
  path = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetFileFromDir(const std::string& directory,
                           const std::string& mask,
                           const std::string& heading,
                           std::string& path,
                           bool useThumbs,
                           bool useFileDirectories,
                           bool singleList)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_FileBrowser_h,
                                  kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir(
                        directory, mask, heading, path, useThumbs, useFileDirectories, singleList));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir t =
      ident.get().as<decltype(t)>();
  path = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetFileList(const std::string& shares,
                        const std::string& mask,
                        const std::string& heading,
                        std::vector<std::string>& fileList,
                        bool useThumbs,
                        bool useFileDirectories)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_FileBrowser_h,
                                  kodi_gui_dialogs_FileBrowser_ShowAndGetFileList));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetFileList(
                        shares, mask, heading, fileList, useThumbs, useFileDirectories));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetFileList t = ident.get().as<decltype(t)>();
  fileList = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetSource(std::string& path,
                      bool allowNetworkShares,
                      const std::string& additionalShare,
                      const std::string& type)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_FileBrowser_h,
                                  kodi_gui_dialogs_FileBrowser_ShowAndGetSource));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetSource(
                        path, allowNetworkShares, additionalShare, type));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetSource t = ident.get().as<decltype(t)>();
  path = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetImage(const std::string& shares, const std::string& heading, std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_FileBrowser_h,
                                  kodi_gui_dialogs_FileBrowser_ShowAndGetImage));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetImage(shares, heading, path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetImage t = ident.get().as<decltype(t)>();
  path = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetImageList(const std::string& shares,
                         const std::string& heading,
                         std::vector<std::string>& file_list)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_FileBrowser_h,
                                  kodi_gui_dialogs_FileBrowser_ShowAndGetImageList));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_FileBrowser_ShowAndGetImageList(shares, heading,
                                                                                   file_list));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_FileBrowser_ShowAndGetImageList t = ident.get().as<decltype(t)>();
  file_list = std::get<1>(t);
  return std::get<0>(t);
}

} /* namespace FileBrowser */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
