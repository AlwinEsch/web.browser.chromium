#pragma once
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

#include "include/cef_menu_model.h"
#include "include/cef_context_menu_handler.h"
#include "addon.h"

class CWebGUIDialogContextMenu
{
public:
  CWebGUIDialogContextMenu();
  virtual ~CWebGUIDialogContextMenu();

  bool Open(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefContextMenuParams>       params,
    CefRefPtr<CefMenuModel>               model);

  bool OnClick(int controlId);
  bool OnFocus(int controlId);
  bool OnInit();
  bool OnAction(int actionId);

  static bool OnClickCB(GUIHANDLE cbhdl, int controlId);
  static bool OnFocusCB(GUIHANDLE cbhdl, int controlId);
  static bool OnInitCB(GUIHANDLE cbhdl);
  static bool OnActionCB(GUIHANDLE cbhdl, int actionId);

  /**
  //{//@{
  virtual bool Clear()                                                ///!< Clears the menu. Returns true on success.
                        OVERRIDE;
  virtual int GetCount()                                              ///!< Returns the number of items in this menu.
                        OVERRIDE;
  virtual bool AddSeparator()                                         ///!< Add a separator to the menu. Returns true on success.
                        OVERRIDE;
  virtual bool AddItem(                                               ///!< Add an item to the menu. Returns true on success.
      int                                   command_id,               ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual bool AddCheckItem(                                          ///!< Add a check item to the menu. Returns true on success.
      int                                   command_id,               ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual bool AddRadioItem(                                          ///!< Add a radio item to the menu. Only a single item with the specified
      int                                   command_id,               ///   |group_id| can be checked at a time. Returns true on success.
      const CefString&                      label,                    ///
      int                                   group_id)                 ///
                        OVERRIDE;
  virtual CefRefPtr<CefMenuModel> AddSubMenu(                         ///!< Add a sub-menu to the menu. The new sub-menu is returned.
      int                                   command_id,               ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual bool InsertSeparatorAt(                                     ///!< Insert a separator in the menu at the specified |index|. Returns true on success.
      int                                   index)                    ///
                        OVERRIDE;
  virtual bool InsertItemAt(                                          ///!< Insert an item in the menu at the specified |index|. Returns true on success
      int                                   index,                    ///
      int                                   command_id,               ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual bool InsertCheckItemAt(                                     ///!< Insert a check item in the menu at the specified |index|. Returns true on success.
      int                                   index,                    ///
      int                                   command_id,               ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual bool InsertRadioItemAt(                                     ///!< Insert a radio item in the menu at the specified |index|. Only a single item
      int                                   index,                    ///   with the specified |group_id| can be checked at a time. Returns true on success.
      int                                   command_id,               ///
      const CefString&                      label,                    ///
      int                                   group_id)                 ///
                        OVERRIDE;
  virtual CefRefPtr<CefMenuModel> InsertSubMenuAt(                    ///!< Insert a sub-menu in the menu at the specified |index|. The new sub-menu
      int                                   index,                    ///   is returned.
      int                                   command_id,               ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual bool Remove(                                                ///!< Removes the item with the specified |command_id|. Returns true on success.
      int                                   command_id)               ///
                        OVERRIDE;
  virtual bool RemoveAt(                                              ///!< Removes the item at the specified |index|. Returns true on success.
      int                                   index)                    ///
                        OVERRIDE;
  virtual int GetIndexOf(                                             ///!< Returns the index associated with the specified |command_id| or -1 if not found
      int                                   command_id)               ///   due to the command id not existing in the menu.
                        OVERRIDE;
  virtual int GetCommandIdAt(                                         ///!< Returns the command id at the specified |index| or -1 if not found due to
      int                                   index)                    ///   invalid range or the index being a separator.
                        OVERRIDE;
  virtual bool SetCommandIdAt(                                        ///!< Sets the command id at the specified |index|. Returns true on success.
      int                                   index,                    ///
      int                                   command_id)               ///
                        OVERRIDE;
  virtual CefString GetLabel(                                         ///!< Returns the label for the specified |command_id| or empty if not found.
      int                                   command_id)               ///
                        OVERRIDE;
  virtual CefString GetLabelAt(                                       ///!< Returns the label at the specified |index| or empty if not found due to
      int                                   index)                    ///   invalid range or the index being a separator.
                        OVERRIDE;
  virtual bool SetLabel(                                              ///!< Sets the label for the specified |command_id|. Returns true on success.
      int                                   command_id,               ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual bool SetLabelAt(                                            ///!< Set the label at the specified |index|. Returns true on success.
      int                                   index,                    ///
      const CefString&                      label)                    ///
                        OVERRIDE;
  virtual MenuItemType GetType(                                       ///!< Returns the item type for the specified |command_id|. default_retval=MENUITEMTYPE_NONE
      int                                   command_id)               ///
                        OVERRIDE;
  virtual MenuItemType GetTypeAt(                                     ///!< Returns the item type for the specified |index|. default_retval=MENUITEMTYPE_NONE
      int                                   index)                    ///
                        OVERRIDE;
  virtual int GetGroupId(                                             ///!< Returns the group id for the specified |command_id| or -1 if invalid.
      int                                   command_id)               ///
                        OVERRIDE;
  virtual int GetGroupIdAt(                                           ///!< Returns the group id at the specified |index| or -1 if invalid.
      int                                   index)                    ///
                        OVERRIDE;
  virtual bool SetGroupId(                                            ///!< Sets the group id for the specified |command_id|. Returns true on success.
      int                                   command_id,               ///
      int                                   group_id)                 ///
                        OVERRIDE;
  virtual bool SetGroupIdAt(                                          ///!< Sets the group id at the specified |index|. Returns true on success.
      int                                   index,                    ///
      int                                   group_id)                 ///
                        OVERRIDE;
  virtual CefRefPtr<CefMenuModel> GetSubMenu(                         ///!< Returns the submenu for the specified |command_id| or empty if invalid.
      int                                   command_id)               ///
                        OVERRIDE;
  virtual CefRefPtr<CefMenuModel> GetSubMenuAt(                       ///!< Returns the submenu at the specified |index| or empty if invalid.
      int                                   index)                    ///
                        OVERRIDE;
  virtual bool IsVisible(                                             ///!< Returns true if the specified |command_id| is visible.
      int                                   command_id)               ///
                        OVERRIDE;
  virtual bool IsVisibleAt(                                           ///!< Returns true if the specified |index| is visible.
      int                                   index)                    ///
                        OVERRIDE;
  virtual bool SetVisible(                                            ///!< Change the visibility of the specified |command_id|. Returns true on success.
      int                                   command_id,               ///
      bool                                  visible)                  ///
                        OVERRIDE;
  virtual bool SetVisibleAt(                                          ///!< Change the visibility at the specified |index|. Returns true on success.
      int                                   index,                    ///
      bool                                  visible)                  ///
                        OVERRIDE;
  virtual bool IsEnabled(                                             ///!< Returns true if the specified |command_id| is enabled.
      int                                   command_id)               ///
                        OVERRIDE;
  virtual bool IsEnabledAt(                                           ///!< Returns true if the specified |index| is enabled.
      int                                   index)                    ///
                        OVERRIDE;
  virtual bool SetEnabled(                                            ///!< Change the enabled status of the specified |command_id|. Returns true on success.
      int                                   command_id,               ///
      bool                                  enabled)                  ///
                        OVERRIDE;
  virtual bool SetEnabledAt(                                          ///!< Change the enabled status at the specified |index|. Returns true on success.
      int                                   index,                    ///
      bool                                  enabled)                  ///
                        OVERRIDE;
  virtual bool IsChecked(                                             ///!< Returns true if the specified |command_id| is checked. Only applies to
      int                                   command_id)               ///   check and radio items.
                        OVERRIDE;
  virtual bool IsCheckedAt(                                           ///!< Returns true if the specified |index| is checked. Only applies to check
      int                                   index)                    ///   and radio items.
                        OVERRIDE;
  virtual bool SetChecked(                                            ///!< Check the specified |command_id|. Only applies to check and radio items.
      int                                   command_id,               ///   Returns true on success.
      bool                                  checked)                  ///
                        OVERRIDE;
  virtual bool SetCheckedAt(                                          ///!< Check the specified |index|. Only applies to check and radio items.
      int                                   index,                    ///   Returns true on success.
      bool                                  checked)                  ///
                        OVERRIDE;
  virtual bool HasAccelerator(                                        ///!< Returns true if the specified |command_id| has a keyboard accelerator assigned.
      int                                   command_id)               ///
                        OVERRIDE;
  virtual bool HasAcceleratorAt(                                      ///!< Returns true if the specified |index| has a keyboard accelerator assigned.
      int                                   index)                    ///
                        OVERRIDE;
  virtual bool SetAccelerator(                                        ///!< Set the keyboard accelerator for the specified |command_id|. |key_code| can
      int                                   command_id,               ///   be any virtual key or character value. Returns true on success.
      int                                   key_code,                 ///
      bool                                  shift_pressed,            ///
      bool                                  ctrl_pressed,             ///
      bool                                  alt_pressed)              ///
                        OVERRIDE;
  virtual bool SetAcceleratorAt(                                      ///!< Set the keyboard accelerator at the specified |index|. |key_code| can be
      int                                   index,                    ///   any virtual key or character value. Returns true on success.
      int                                   key_code,                 ///
      bool                                  shift_pressed,            ///
      bool                                  ctrl_pressed,             ///
      bool                                  alt_pressed)              ///
                        OVERRIDE;
  virtual bool RemoveAccelerator(                                     ///!< Remove the keyboard accelerator for the specified |command_id|. Returns
      int                                   command_id)               ///   true on success.
                        OVERRIDE;
  virtual bool RemoveAcceleratorAt(                                   ///!< Remove the keyboard accelerator at the specified |index|. Returns true on success.
      int                                   index)                    ///
                        OVERRIDE;
  virtual bool GetAccelerator(                                        ///!< Retrieves the keyboard accelerator for the specified |command_id|. Returns
      int                                   command_id,               ///   true on success.
      int&                                  key_code,                 ///
      bool&                                 shift_pressed,            ///
      bool&                                 ctrl_pressed,             ///
      bool&                                 alt_pressed)              ///
                        OVERRIDE;
  virtual bool GetAcceleratorAt(                                      ///!< Retrieves the keyboard accelerator for the specified |index|.
      int                                   index,                    ///   Returns true on success.
      int&                                  key_code,                 ///
      bool&                                 shift_pressed,            ///
      bool&                                 ctrl_pressed,             ///
      bool&                                 alt_pressed)              ///
                        OVERRIDE;
//}//@}
*/
private:
  CAddonGUIWindow*                m_window;

  CefRefPtr<CefBrowser>           m_browser;
  CefRefPtr<CefFrame>             m_frame;
  CefRefPtr<CefContextMenuParams> m_params;
  CefRefPtr<CefMenuModel>         m_model;
};
