/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Select.h"

#include "../../../../sandbox/kodi/gui/dialogs/Select.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/Select.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_Select_h::HandleMessage(int funcGroup,
                                           int func,
                                           const msgpack::unpacked& in,
                                           msgpack::sbuffer& out,
                                           const char* data,
                                           std::size_t size,
                                           std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_Select_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_Select_Show:
    {
      msgParent__IN_kodi_gui_dialogs_Select_Show t = in.get().as<decltype(t)>();

      int ret = kodi::gui::dialogs::Select::Show(std::get<0>(t), std::get<1>(t), std::get<2>(t),
                                                 std::get<3>(t));

      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Select_Show(ret));
      break;
    }
    case kodi_gui_dialogs_Select_Show2:
    {
      msgParent__IN_kodi_gui_dialogs_Select_Show2 t = in.get().as<decltype(t)>();

      std::vector<std::tuple<std::string, std::string, bool>> entries = std::get<1>(t);
      std::vector<gui::dialogs::SSelectionEntry> entriesKodi;
      for (const auto& entry : entries)
      {
        gui::dialogs::SSelectionEntry data;
        data.id = std::get<0>(entry);
        data.name = std::get<1>(entry);
        data.selected = std::get<2>(entry);
        entriesKodi.emplace_back(data);
      }
      int ret = kodi::gui::dialogs::Select::Show(std::get<0>(t), entriesKodi, std::get<2>(t),
                                                 std::get<3>(t));
      for (size_t i = 0; i < entries.size(); ++i)
      {
        std::get<2>(entries[i]) = entriesKodi[i].selected;
      }
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Select_Show2(ret, entries));
      break;
    }
    case kodi_gui_dialogs_Select_ShowMultiSelect:
    {
      msgParent__IN_kodi_gui_dialogs_Select_ShowMultiSelect t = in.get().as<decltype(t)>();

      std::vector<std::tuple<std::string, std::string, bool>> entries = std::get<1>(t);
      std::vector<gui::dialogs::SSelectionEntry> entriesKodi;
      for (const auto& entry : entries)
      {
        gui::dialogs::SSelectionEntry data;
        data.id = std::get<0>(entry);
        data.name = std::get<1>(entry);
        data.selected = std::get<2>(entry);
        entriesKodi.emplace_back(data);
      }
      int ret =
          kodi::gui::dialogs::Select::ShowMultiSelect(std::get<0>(t), entriesKodi, std::get<2>(t));
      for (size_t i = 0; i < entries.size(); ++i)
      {
        std::get<2>(entries[i]) = entriesKodi[i].selected;
      }
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Select_ShowMultiSelect(ret, entries));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_Select_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
