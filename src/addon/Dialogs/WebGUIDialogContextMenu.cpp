/*
 *      Copyright (C) 2015 Team KODI
 *      http:/kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "WebGUIDialogContextMenu.h"

CWebGUIDialogContextMenu::CWebGUIDialogContextMenu()
{

}

CWebGUIDialogContextMenu::~CWebGUIDialogContextMenu()
{

}

bool CWebGUIDialogContextMenu::Clear()
{
  return false;
}

int CWebGUIDialogContextMenu::GetCount()
{
  return 0;
}

bool CWebGUIDialogContextMenu::AddSeparator()
{
  return false;
}

bool CWebGUIDialogContextMenu::AddItem(
    int              command_id,
    const CefString& label)
{
  return false;
}

bool CWebGUIDialogContextMenu::AddCheckItem(
    int              command_id,
    const CefString& label)
{
  return false;
}

bool CWebGUIDialogContextMenu::AddRadioItem(
    int              command_id,
    const CefString& label,
    int              group_id)
{
  return false;
}

CefRefPtr<CefMenuModel> CWebGUIDialogContextMenu::AddSubMenu(
    int              command_id,
    const CefString& label)
{
  return nullptr;
}

bool CWebGUIDialogContextMenu::InsertSeparatorAt(
    int              index)
{
  return false;
}

bool CWebGUIDialogContextMenu::InsertItemAt(
    int              index,
    int              command_id,
    const CefString& label)
{
  return false;
}

bool CWebGUIDialogContextMenu::InsertCheckItemAt(
    int              index,
    int              command_id,
    const CefString& label)
{
  return false;
}

bool CWebGUIDialogContextMenu::InsertRadioItemAt(
    int              index,
    int              command_id,
    const CefString& label,
    int              group_id)
{
  return false;
}

CefRefPtr<CefMenuModel> CWebGUIDialogContextMenu::InsertSubMenuAt(
    int              index,
    int              command_id,
    const CefString& label)
{
  return nullptr;
}

bool CWebGUIDialogContextMenu::Remove(
    int              command_id)
{
  return false;
}

bool CWebGUIDialogContextMenu::RemoveAt(
    int              index)
{
  return false;
}

int CWebGUIDialogContextMenu::GetIndexOf(
    int              command_id)
{
  return 0;
}

int CWebGUIDialogContextMenu::GetCommandIdAt(
    int              index)
{
  return 0;
}

bool CWebGUIDialogContextMenu::SetCommandIdAt(
    int              index,
    int              command_id)
{
  return false;
}

CefString CWebGUIDialogContextMenu::GetLabel(
    int              command_id)
{
  return "";
}

CefString CWebGUIDialogContextMenu::GetLabelAt(
    int              index)
{
  return "";
}

bool CWebGUIDialogContextMenu::SetLabel(
    int              command_id,
    const CefString& label)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetLabelAt(
    int              index,
    const CefString& label)
{
  return false;
}

CefMenuModel::MenuItemType CWebGUIDialogContextMenu::GetType(
    int              command_id)
{
  return MENUITEMTYPE_NONE;
}

CefMenuModel::MenuItemType CWebGUIDialogContextMenu::GetTypeAt(
    int              index)
{
  return MENUITEMTYPE_NONE;
}

int CWebGUIDialogContextMenu::GetGroupId(
    int              command_id)
{
  return 0;
}

int CWebGUIDialogContextMenu::GetGroupIdAt(
    int              index)
{
  return 0;
}

bool CWebGUIDialogContextMenu::SetGroupId(
    int              command_id,
    int              group_id)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetGroupIdAt(
    int              index,
    int              group_id)
{
  return false;
}

CefRefPtr<CefMenuModel> CWebGUIDialogContextMenu::GetSubMenu(
    int              command_id)
{
  return nullptr;
}

CefRefPtr<CefMenuModel> CWebGUIDialogContextMenu::GetSubMenuAt(
    int              index)
{
  return nullptr;
}

bool CWebGUIDialogContextMenu::IsVisible(
    int              command_id)
{
  return false;
}

bool CWebGUIDialogContextMenu::IsVisibleAt(
    int              index)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetVisible(
    int              command_id,
    bool             visible)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetVisibleAt(
    int              index,
    bool             visible)
{
  return false;
}

bool CWebGUIDialogContextMenu::IsEnabled(
    int              command_id)
{
  return false;
}

bool CWebGUIDialogContextMenu::IsEnabledAt(
    int              index)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetEnabled(
    int              command_id,
    bool             enabled)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetEnabledAt(
    int              index,
    bool             enabled)
{
  return false;
}

bool CWebGUIDialogContextMenu::IsChecked(
    int              command_id)
{
  return false;
}

bool CWebGUIDialogContextMenu::IsCheckedAt(
    int              index)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetChecked(
    int              command_id,
    bool             checked)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetCheckedAt(
    int              index,
    bool             checked)
{
  return false;
}

bool CWebGUIDialogContextMenu::HasAccelerator(
    int              command_id)
{
  return false;
}

bool CWebGUIDialogContextMenu::HasAcceleratorAt(
    int              index)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetAccelerator(
    int              command_id,
    int              key_code,
    bool             shift_pressed,
    bool             ctrl_pressed,
    bool             alt_pressed)
{
  return false;
}

bool CWebGUIDialogContextMenu::SetAcceleratorAt(
    int              index,
    int              key_code,
    bool             shift_pressed,
    bool             ctrl_pressed,
    bool             alt_pressed)
{
  return false;
}

bool CWebGUIDialogContextMenu::RemoveAccelerator(
    int              command_id)
{
  return false;
}

bool CWebGUIDialogContextMenu::RemoveAcceleratorAt(
    int              index)
{
  return false;
}

bool CWebGUIDialogContextMenu::GetAccelerator(
    int              command_id,
    int&             key_code,
    bool&            shift_pressed,
    bool&            ctrl_pressed,
    bool&            alt_pressed)
{
  return false;
}

bool CWebGUIDialogContextMenu::GetAcceleratorAt(
    int              index,
    int&             key_code,
    bool&            shift_pressed,
    bool&            ctrl_pressed,
    bool&            alt_pressed)
{
  return false;
}
