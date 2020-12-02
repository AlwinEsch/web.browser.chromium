/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/ContextMenu.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/ContextMenu.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace ContextMenu
{

int Show(const std::string& heading, const std::vector<std::string>& entries)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_ContextMenu_h, kodi_gui_dialogs_ContextMenu_Show));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_ContextMenu_Show(heading, entries));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_ContextMenu_Show t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int Show(const std::string& heading,
         const std::vector<std::pair<std::string, std::string>>& entries)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_ContextMenu_h, kodi_gui_dialogs_ContextMenu_Show2));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_ContextMenu_Show2(heading, entries));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_ContextMenu_Show2 t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int Show(const std::string& heading, const std::vector<std::pair<int, std::string>>& entries)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_ContextMenu_h, kodi_gui_dialogs_ContextMenu_Show3));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_ContextMenu_Show3(heading, entries));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_ContextMenu_Show3 t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace ContextMenu */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
