/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../include/kodi/gui/ListItem.h"

#include "../../../include/kodi/Processor.h"
#include "../../sandbox/ShareProcessTransmitter.h"
#include "../../sandbox/kodi/gui/ListItem.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{

CListItem::CListItem(const std::string& label, const std::string& label2, const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_create(uint64_t(this), label, label2, path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CListItem_create t = ident.get().as<decltype(t)>();
  m_controlHandle = std::get<0>(t);
}

CListItem::~CListItem()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_destroy(m_controlHandle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CListItem::GetLabel() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_GetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_GetLabel(m_controlHandle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CListItem_GetLabel t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CListItem::SetLabel(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_SetLabel));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_SetLabel(m_controlHandle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CListItem::GetLabel2() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_GetLabel2));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_GetLabel2(m_controlHandle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CListItem_GetLabel2 t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CListItem::SetLabel2(const std::string& label)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_SetLabel2));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_SetLabel2(m_controlHandle, label));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CListItem::GetArt(const std::string& type) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_GetArt));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_GetArt(m_controlHandle, type));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CListItem_GetArt t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CListItem::SetArt(const std::string& type, const std::string& url)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_SetArt));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_SetArt(m_controlHandle, type, url));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CListItem::GetPath() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_GetPath));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_GetPath(m_controlHandle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CListItem_GetPath t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CListItem::SetPath(const std::string& path)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_SetPath));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_SetPath(m_controlHandle, path));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CListItem::GetProperty(const std::string& key) const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_GetProperty));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_GetProperty(m_controlHandle, key));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CListItem_GetProperty t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CListItem::SetProperty(const std::string& key, const std::string& value)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_SetProperty));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_SetProperty(m_controlHandle, key, value));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CListItem::Select(bool selected)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_Select));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_Select(m_controlHandle, selected));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CListItem::IsSelected() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_ListItem_h, kodi_gui_CListItem_IsSelected));
  msgpack::pack(in, msgParent__IN_kodi_gui_CListItem_IsSelected(m_controlHandle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_CListItem_IsSelected t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace gui */
} /* namespace kodi */
