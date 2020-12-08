/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Window.h"

#include "../../../sandbox/kodi/gui/Window.h"
#include "../../../sandbox/shared_data_structure.h"
#include "../../Launcher.h"
#include "../../utils/Log.h"

#include <kodi/gui/Window.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

class CListItem;

class ATTRIBUTE_HIDDEN CWindowHdl : public kodi::gui::CWindow
{
public:
  CWindowHdl(uint64_t handle,
             CChildLauncher& launcher,
             const std::string& xmlFilename,
             const std::string& defaultSkin,
             bool asDialog,
             bool isMedia = false)
    : kodi::gui::CWindow(xmlFilename, defaultSkin, asDialog, isMedia),
      m_handle(handle),
      m_launcher(launcher)
  {
  }

  virtual ~CWindowHdl() {}

  bool OnInit() override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(
        in, msgIdentifier(funcGroup_gui_Window_h, funcChild_gui_Window_h::kodi_gui_CWindow_OnInit));
    msgpack::pack(in, msgChild__IN_kodi_gui_CWindow_OnInit(m_handle));
    m_launcher.GetCurrentTransmitter()->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_CWindow_OnInit t = ident.get().as<decltype(t)>();
    return std::get<0>(t);
  }

  bool OnFocus(int controlId) override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                    funcChild_gui_Window_h::kodi_gui_CWindow_OnFocus));
    msgpack::pack(in, msgChild__IN_kodi_gui_CWindow_OnFocus(m_handle, controlId));
    m_launcher.GetCurrentTransmitter()->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_CWindow_OnFocus t = ident.get().as<decltype(t)>();
    return std::get<0>(t);
  }

  bool OnClick(int controlId) override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                    funcChild_gui_Window_h::kodi_gui_CWindow_OnClick));
    msgpack::pack(in, msgChild__IN_kodi_gui_CWindow_OnClick(m_handle, controlId));
    m_launcher.GetCurrentTransmitter()->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_CWindow_OnClick t = ident.get().as<decltype(t)>();
    return std::get<0>(t);
  }

  bool OnAction(const kodi::gui::input::CAction& action) override
  {
    gui_input_CAction ifcAction;
    ifcAction.id = action.GetID();
    ifcAction.name = action.GetName();
    ifcAction.amount = action.GetAmountList();
    ifcAction.repeat = action.GetRepeat();
    ifcAction.holdTime = action.GetHoldTime();
    ifcAction.buttonCode = action.GetButtonCode();
    ifcAction.unicode = action.GetUnicode();
    ifcAction.text = action.GetText();
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                    funcChild_gui_Window_h::kodi_gui_CWindow_OnAction));
    msgpack::pack(in, msgChild__IN_kodi_gui_CWindow_OnAction(m_handle, ifcAction));
    m_launcher.GetCurrentTransmitter()->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_CWindow_OnAction t = ident.get().as<decltype(t)>();
    return std::get<0>(t);
  }

  void GetContextButtons(int itemNumber,
                         std::vector<std::pair<unsigned int, std::string>>& buttons) override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                    funcChild_gui_Window_h::kodi_gui_CWindow_GetContextButtons));
    msgpack::pack(in,
                  msgChild__IN_kodi_gui_CWindow_GetContextButtons(m_handle, itemNumber, buttons));

    m_launcher.GetCurrentTransmitter()->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_CWindow_GetContextButtons t = ident.get().as<decltype(t)>();
    buttons = std::get<0>(t);
  }

  bool OnContextButton(int itemNumber, unsigned int button) override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                    funcChild_gui_Window_h::kodi_gui_CWindow_OnContextButton));
    msgpack::pack(in, msgChild__IN_kodi_gui_CWindow_OnContextButton(m_handle, itemNumber, button));
    m_launcher.GetCurrentTransmitter()->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_CWindow_OnContextButton t = ident.get().as<decltype(t)>();
    return std::get<0>(t);
  }

  uint64_t GetControlHandle() const { return m_handle; }

  void AddListItemInternal(uint64_t item, int itemPosition = -1)
  {
    KODI_GUI_LISTITEM_HANDLE controlHandle = reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(item);

    m_interface->kodi_gui->window->add_list_item(m_interface->kodiBase, m_controlHandle,
                                                 controlHandle, itemPosition);
  }

  void RemoveListItemInternal(uint64_t item)
  {
    KODI_GUI_LISTITEM_HANDLE controlHandle = reinterpret_cast<KODI_GUI_LISTITEM_HANDLE>(item);

    m_interface->kodi_gui->window->remove_list_item(m_interface->kodiBase, m_controlHandle,
                                                    controlHandle);
  }

  uint64_t GetListItemInternal(int listPos)
  {
    KODI_GUI_LISTITEM_HANDLE handle = m_interface->kodi_gui->window->get_list_item(
        m_interface->kodiBase, m_controlHandle, listPos);
    return uint64_t(handle);
  }

protected:
  uint64_t m_handle;
  CChildLauncher& m_launcher;
};


bool C_gui_Window_h::HandleMessage(int funcGroup,
                                   int func,
                                   const msgpack::unpacked& in,
                                   msgpack::sbuffer& out,
                                   const char* data,
                                   std::size_t size,
                                   std::size_t offset)
{
  if (funcGroup != funcGroup_gui_Window_h)
    return false;

  switch (func)
  {
    case kodi_gui_CWindow_create:
    {
      msgParent__IN_kodi_gui_CWindow_create t = in.get().as<decltype(t)>();
      CWindowHdl* window = new CWindowHdl(std::get<0>(t), m_childLauncher, std::get<1>(t),
                                          std::get<2>(t), std::get<3>(t), std::get<4>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_create(uint64_t(window)));
      break;
    }
    case kodi_gui_CWindow_destroy:
    {
      msgParent__IN_kodi_gui_CWindow_destroy t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      delete window;
      break;
    }
    case kodi_gui_CWindow_Show:
    {
      msgParent__IN_kodi_gui_CWindow_Show t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_Show(window->Show()));
      break;
    }
    case kodi_gui_CWindow_Close:
    {
      msgParent__IN_kodi_gui_CWindow_Close t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->Close();
      break;
    }
    case kodi_gui_CWindow_DoModal:
    {
      msgParent__IN_kodi_gui_CWindow_DoModal t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->DoModal();
      break;
    }
    case kodi_gui_CWindow_SetFocusId:
    {
      msgParent__IN_kodi_gui_CWindow_SetFocusId t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_SetFocusId(window->SetFocusId(std::get<1>(t))));
      break;
    }
    case kodi_gui_CWindow_GetFocusId:
    {
      msgParent__IN_kodi_gui_CWindow_GetFocusId t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetFocusId(window->GetFocusId()));
      break;
    }
    case kodi_gui_CWindow_SetControlLabel:
    {
      msgParent__IN_kodi_gui_CWindow_SetControlLabel t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetControlLabel(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_SetControlVisible:
    {
      msgParent__IN_kodi_gui_CWindow_SetControlVisible t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetControlVisible(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_SetControlSelected:
    {
      msgParent__IN_kodi_gui_CWindow_SetControlSelected t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetControlSelected(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_SetProperty:
    {
      msgParent__IN_kodi_gui_CWindow_SetProperty t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetProperty(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_GetProperty:
    {
      msgParent__IN_kodi_gui_CWindow_GetProperty t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(
          out, msgParent_OUT_kodi_gui_CWindow_GetProperty(window->GetProperty(std::get<1>(t))));
      break;
    }
    case kodi_gui_CWindow_SetPropertyInt:
    {
      msgParent__IN_kodi_gui_CWindow_SetPropertyInt t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetPropertyInt(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_GetPropertyInt:
    {
      msgParent__IN_kodi_gui_CWindow_GetPropertyInt t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetPropertyInt(
                             window->GetPropertyInt(std::get<1>(t))));
      break;
    }
    case kodi_gui_CWindow_SetPropertyBool:
    {
      msgParent__IN_kodi_gui_CWindow_SetPropertyBool t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetPropertyBool(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_GetPropertyBool:
    {
      msgParent__IN_kodi_gui_CWindow_GetPropertyBool t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetPropertyBool(
                             window->GetPropertyBool(std::get<1>(t))));
      break;
    }
    case kodi_gui_CWindow_SetPropertyDouble:
    {
      msgParent__IN_kodi_gui_CWindow_SetPropertyDouble t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetPropertyDouble(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_GetPropertyDouble:
    {
      msgParent__IN_kodi_gui_CWindow_GetPropertyDouble t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetPropertyDouble(
                             window->GetPropertyDouble(std::get<1>(t))));
      break;
    }
    case kodi_gui_CWindow_ClearProperties:
    {
      msgParent__IN_kodi_gui_CWindow_ClearProperties t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->ClearProperties();
      break;
    }
    case kodi_gui_CWindow_ClearProperty:
    {
      msgParent__IN_kodi_gui_CWindow_ClearProperty t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->ClearProperty(std::get<1>(t));
      break;
    }
    case kodi_gui_CWindow_ClearList:
    {
      msgParent__IN_kodi_gui_CWindow_ClearList t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->ClearList();
      break;
    }
    case kodi_gui_CWindow_AddListItem:
    {
      msgParent__IN_kodi_gui_CWindow_AddListItem t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->AddListItemInternal(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_AddListItem2:
    {
      msgParent__IN_kodi_gui_CWindow_AddListItem2 t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->AddListItem(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_RemoveListItem:
    {
      msgParent__IN_kodi_gui_CWindow_RemoveListItem t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->RemoveListItemInternal(std::get<1>(t));
      break;
    }
    case kodi_gui_CWindow_RemoveListItem2:
    {
      msgParent__IN_kodi_gui_CWindow_RemoveListItem2 t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->RemoveListItem(std::get<1>(t));
      break;
    }
    case kodi_gui_CWindow_GetListItem:
    {
      msgParent__IN_kodi_gui_CWindow_GetListItem t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetListItem(
                             window->GetListItemInternal(std::get<1>(t))));
      break;
    }
    case kodi_gui_CWindow_SetCurrentListPosition:
    {
      msgParent__IN_kodi_gui_CWindow_SetCurrentListPosition t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetCurrentListPosition(std::get<1>(t));
      break;
    }
    case kodi_gui_CWindow_GetCurrentListPosition:
    {
      msgParent__IN_kodi_gui_CWindow_GetCurrentListPosition t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetCurrentListPosition(
                             window->GetCurrentListPosition()));
      break;
    }
    case kodi_gui_CWindow_GetListSize:
    {
      msgParent__IN_kodi_gui_CWindow_GetListSize t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetListSize(window->GetListSize()));
      break;
    }
    case kodi_gui_CWindow_SetContainerProperty:
    {
      msgParent__IN_kodi_gui_CWindow_SetContainerProperty t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetContainerProperty(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_CWindow_SetContainerContent:
    {
      msgParent__IN_kodi_gui_CWindow_SetContainerContent t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->SetContainerContent(std::get<1>(t));
      break;
    }
    case kodi_gui_CWindow_GetCurrentContainerId:
    {
      msgParent__IN_kodi_gui_CWindow_GetCurrentContainerId t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_CWindow_GetCurrentContainerId(
                             window->GetCurrentContainerId()));
      break;
    }
    case kodi_gui_CWindow_MarkDirtyRegion:
    {
      msgParent__IN_kodi_gui_CWindow_MarkDirtyRegion t = in.get().as<decltype(t)>();
      CWindowHdl* window = reinterpret_cast<CWindowHdl*>(std::get<0>(t));
      if (!window)
        break;

      window->MarkDirtyRegion();
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "C_gui_Window_h::%s: addon called with unknown function id '%i' on group 'kodi'",
          __func__, func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */
