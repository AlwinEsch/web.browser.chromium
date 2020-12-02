/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../include/kodi/gui/Window.h"

#include "../../../include/kodi/Processor.h"
#include "../../../include/kodi/gui/ListItem.h"
#include "../../sandbox/ShareProcessTransmitter.h"
#include "../../sandbox/kodi/gui/Window.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{

CWindow::CWindow(const std::string& xmlFilename,
                 const std::string& defaultSkin,
                 bool asDialog,
                 bool isMedia)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_Window_h, kodi_gui_CWindow_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_create(uint64_t(this), xmlFilename, defaultSkin,
                                                          asDialog, isMedia));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CWindow::~CWindow()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_Window_h, kodi_gui_CWindow_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CWindow::Show()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h, kodi_gui_CWindow_Show));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_Show(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_Show t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::Close()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_Window_h, kodi_gui_CWindow_Close));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_Close(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::DoModal()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_Window_h, kodi_gui_CWindow_DoModal));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_DoModal(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CWindow::SetFocusId(int controlId)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetFocusId));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetFocusId(m_handle, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_SetFocusId t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int CWindow::GetFocusId()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetFocusId));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetFocusId(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetFocusId t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::SetControlLabel(int controlId, const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetControlLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetControlLabel(m_handle, controlId, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::SetControlVisible(int controlId, bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetControlVisible));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_CWindow_SetControlVisible(m_handle, controlId, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::SetControlSelected(int controlId, bool selected)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetControlSelected));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_CWindow_SetControlSelected(m_handle, controlId, selected));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::SetProperty(const std::string& key, const std::string& value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetProperty));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetProperty(m_handle, key, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CWindow::GetProperty(const std::string& key) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetProperty));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetProperty(m_handle, key));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetProperty t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::SetPropertyInt(const std::string& key, int value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetPropertyInt));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetPropertyInt(m_handle, key, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int CWindow::GetPropertyInt(const std::string& key) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetPropertyInt));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetPropertyInt(m_handle, key));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetPropertyInt t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::SetPropertyBool(const std::string& key, bool value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetPropertyBool));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetPropertyBool(m_handle, key, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CWindow::GetPropertyBool(const std::string& key) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetPropertyBool));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetPropertyBool(m_handle, key));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetPropertyBool t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::SetPropertyDouble(const std::string& key, double value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetPropertyDouble));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetPropertyDouble(m_handle, key, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

double CWindow::GetPropertyDouble(const std::string& key) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetPropertyDouble));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetPropertyDouble(m_handle, key));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetPropertyDouble t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::ClearProperties()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_ClearProperties));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_ClearProperties(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::ClearProperty(const std::string& key)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_ClearProperty));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_ClearProperty(m_handle, key));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::ClearList()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_ClearList));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_ClearList(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::AddListItem(std::shared_ptr<CListItem> item, int itemPosition)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_AddListItem));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_AddListItem(m_handle, item->m_controlHandle,
                                                               itemPosition));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::AddListItem(const std::string item, int itemPosition)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_AddListItem2));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_AddListItem2(m_handle, item, itemPosition));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::RemoveListItem(int itemPosition)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_RemoveListItem2));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_RemoveListItem2(m_handle, itemPosition));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::RemoveListItem(std::shared_ptr<CListItem> item)
{
  if (!item)
    return;
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_RemoveListItem));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_CWindow_RemoveListItem(m_handle, item->m_controlHandle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::shared_ptr<CListItem> CWindow::GetListItem(int listPos)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetListItem));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetListItem(m_handle, listPos));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetListItem t = ident.get().as<decltype(t)>();
  uint64_t handle = std::get<0>(t);
  if (!handle)
    return std::shared_ptr<CListItem>();

  return std::make_shared<kodi::gui::CListItem>(handle);
}

void CWindow::SetCurrentListPosition(int listPos)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_Window_h,
                              kodi_gui_CWindow_SetCurrentListPosition));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetCurrentListPosition(m_handle, listPos));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int CWindow::GetCurrentListPosition() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_Window_h,
                              kodi_gui_CWindow_GetCurrentListPosition));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetCurrentListPosition(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetCurrentListPosition t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int CWindow::GetListSize() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetListSize));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetListSize(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetListSize t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::SetContainerProperty(const std::string& key, const std::string& value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetContainerProperty));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetContainerProperty(m_handle, key, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWindow::SetContainerContent(const std::string& value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_SetContainerContent));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_SetContainerContent(m_handle, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int CWindow::GetCurrentContainerId() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_GetCurrentContainerId));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_GetCurrentContainerId(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CWindow_GetCurrentContainerId t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CWindow::MarkDirtyRegion()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_Window_h,
                                  kodi_gui_CWindow_MarkDirtyRegion));
  msgpack::pack(in, msgParent__IN_kodi_gui_CWindow_MarkDirtyRegion(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace gui */
} /* namespace kodi */
