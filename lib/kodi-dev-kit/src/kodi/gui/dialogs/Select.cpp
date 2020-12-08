/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/Select.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/Select.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace Select
{

int Show(const std::string& heading,
         const std::vector<std::string>& entries,
         int selected,
         unsigned int autoclose)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Select_h, kodi_gui_dialogs_Select_Show));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_dialogs_Select_Show(heading, entries, selected, autoclose));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Select_Show t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int Show(const std::string& heading,
         std::vector<kodi::gui::dialogs::SSelectionEntry>& entries,
         int selected,
         unsigned int autoclose)
{
  std::vector<std::tuple<std::string, std::string, bool>> data;
  data.reserve(entries.size());
  for (const auto& entry : entries)
    data.emplace_back(entry.id, entry.name, entry.selected);

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Select_h, kodi_gui_dialogs_Select_Show2));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_dialogs_Select_Show2(heading, data, selected, autoclose));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Select_Show2 t = ident.get().as<decltype(t)>();

  const std::vector<std::tuple<std::string, std::string, bool>>& retEntries = std::get<1>(t);
  for (size_t i = 0; i < retEntries.size(); ++i)
    entries[i].selected = std::get<2>(retEntries[i]);

  return std::get<0>(t);
}

bool ShowMultiSelect(const std::string& heading,
                     std::vector<kodi::gui::dialogs::SSelectionEntry>& entries,
                     int autoclose)
{
  std::vector<std::tuple<std::string, std::string, bool>> data;
  data.reserve(entries.size());
  for (const auto& entry : entries)
    data.emplace_back(entry.id, entry.name, entry.selected);

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Select_h, kodi_gui_dialogs_Select_ShowMultiSelect));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_dialogs_Select_ShowMultiSelect(heading, data, autoclose));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Select_ShowMultiSelect t = ident.get().as<decltype(t)>();

  const std::vector<std::tuple<std::string, std::string, bool>>& retEntries = std::get<1>(t);
  for (size_t i = 0; i < retEntries.size(); ++i)
    entries[i].selected = std::get<2>(retEntries[i]);

  return std::get<0>(t);
}

} /* namespace Select */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
