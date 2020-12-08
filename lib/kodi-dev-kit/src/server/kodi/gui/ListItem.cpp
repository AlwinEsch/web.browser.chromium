/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "ListItem.h"

#include "../../../sandbox/kodi/gui/ListItem.h"
#include "../../utils/Log.h"

#include <kodi/gui/ListItem.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_ListItem_h::HandleMessage(int funcGroup,
                                     int func,
                                     const msgpack::unpacked& in,
                                     msgpack::sbuffer& out,
                                     const char* data,
                                     std::size_t size,
                                     std::size_t offset)
{
  using kodi::addon::CAddonBase;

  if (funcGroup != funcGroup_gui_ListItem_h)
    return false;

  switch (func)
  {
    case kodi_gui_CListItem_create:
    {
      msgParent__IN_kodi_gui_CListItem_create t = in.get().as<decltype(t)>();
      const std::string& label = std::get<1>(t);
      const std::string& label2 = std::get<2>(t);
      const std::string& path = std::get<3>(t);

      KODI_GUI_LISTITEM_HANDLE controlHandle =
          CAddonBase::m_interface->toKodi->kodi_gui->listItem->create(
              CAddonBase::m_interface->toKodi->kodiBase, label.c_str(), label2.c_str(),
              path.c_str());

      msgpack::pack(out, msgParent_OUT_kodi_gui_CListItem_create(uint64_t(controlHandle)));
      break;
    }
    case kodi_gui_CListItem_destroy:
    {
      msgParent__IN_kodi_gui_CListItem_destroy t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      CAddonBase::m_interface->toKodi->kodi_gui->listItem->destroy(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle);
      break;
    }
    case kodi_gui_CListItem_GetLabel:
    {
      msgParent__IN_kodi_gui_CListItem_GetLabel t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      std::string label;
      char* ret = CAddonBase::m_interface->toKodi->kodi_gui->listItem->get_label(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle);
      if (ret != nullptr)
      {
        if (std::strlen(ret))
          label = ret;
        CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                     ret);
      }

      msgpack::pack(out, msgParent_OUT_kodi_gui_CListItem_GetLabel(label));
      break;
    }
    case kodi_gui_CListItem_SetLabel:
    {
      msgParent__IN_kodi_gui_CListItem_SetLabel t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      CAddonBase::m_interface->toKodi->kodi_gui->listItem->set_label(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t).c_str());
      break;
    }
    case kodi_gui_CListItem_GetLabel2:
    {
      msgParent__IN_kodi_gui_CListItem_GetLabel2 t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      std::string label;
      char* ret = CAddonBase::m_interface->toKodi->kodi_gui->listItem->get_label2(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle);
      if (ret != nullptr)
      {
        if (std::strlen(ret))
          label = ret;
        CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                     ret);
      }

      msgpack::pack(out, msgParent_OUT_kodi_gui_CListItem_GetLabel2(label));
      break;
    }
    case kodi_gui_CListItem_SetLabel2:
    {
      msgParent__IN_kodi_gui_CListItem_SetLabel2 t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      CAddonBase::m_interface->toKodi->kodi_gui->listItem->set_label2(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t).c_str());
      break;
    }
    case kodi_gui_CListItem_GetArt:
    {
      msgParent__IN_kodi_gui_CListItem_GetArt t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      std::string strReturn;
      char* ret = CAddonBase::m_interface->toKodi->kodi_gui->listItem->get_art(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t).c_str());
      if (ret != nullptr)
      {
        if (std::strlen(ret))
          strReturn = ret;
        CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                     ret);
      }

      msgpack::pack(out, msgParent_OUT_kodi_gui_CListItem_GetArt(strReturn));
      break;
    }
    case kodi_gui_CListItem_SetArt:
    {
      msgParent__IN_kodi_gui_CListItem_SetArt t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      CAddonBase::m_interface->toKodi->kodi_gui->listItem->set_art(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t).c_str(),
          std::get<2>(t).c_str());

      break;
    }
    case kodi_gui_CListItem_GetPath:
    {
      msgParent__IN_kodi_gui_CListItem_GetPath t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      std::string strReturn;
      char* ret = CAddonBase::m_interface->toKodi->kodi_gui->listItem->get_path(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle);
      if (ret != nullptr)
      {
        if (std::strlen(ret))
          strReturn = ret;
        CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                     ret);
      }

      msgpack::pack(out, msgParent_OUT_kodi_gui_CListItem_GetLabel(strReturn));
      break;
    }
    case kodi_gui_CListItem_SetPath:
    {
      msgParent__IN_kodi_gui_CListItem_SetPath t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      CAddonBase::m_interface->toKodi->kodi_gui->listItem->set_path(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t).c_str());
      break;
    }
    case kodi_gui_CListItem_GetProperty:
    {
      msgParent__IN_kodi_gui_CListItem_GetProperty t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      std::string label;
      char* ret = CAddonBase::m_interface->toKodi->kodi_gui->listItem->get_property(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t).c_str());
      if (ret != nullptr)
      {
        if (std::strlen(ret))
          label = ret;
        CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                     ret);
      }
      msgpack::pack(out, msgParent_OUT_kodi_gui_CListItem_GetProperty(label));
      break;
    }
    case kodi_gui_CListItem_SetProperty:
    {
      msgParent__IN_kodi_gui_CListItem_SetProperty t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      CAddonBase::m_interface->toKodi->kodi_gui->listItem->set_property(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t).c_str(),
          std::get<2>(t).c_str());
      break;
    }
    case kodi_gui_CListItem_Select:
    {
      msgParent__IN_kodi_gui_CListItem_Select t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      CAddonBase::m_interface->toKodi->kodi_gui->listItem->select(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle, std::get<1>(t));
      break;
    }
    case kodi_gui_CListItem_IsSelected:
    {
      msgParent__IN_kodi_gui_CListItem_IsSelected t = in.get().as<decltype(t)>();
      KODI_GUI_LISTITEM_HANDLE controlHandle =
          reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(std::get<0>(t));
      if (!controlHandle)
        break;

      bool selected = CAddonBase::m_interface->toKodi->kodi_gui->listItem->is_selected(
          CAddonBase::m_interface->toKodi->kodiBase, controlHandle);
      msgpack::pack(out, msgParent_OUT_kodi_gui_CListItem_IsSelected(selected));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "C_gui_ListItem_h::%s: addon called with unknown function id '%i' on group 'kodi'",
          __func__, func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */
