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

#include <unistd.h>

#include "include/internal/cef_types.h"

#include "WebGUIDialogContextMenu.h"

/*!
 * @brief Static creation functions
 */
//{
// bool CWebGUIDialogContextMenu::OnInitCB(GUIHANDLE cbhdl)
// {
//   CWebGUIDialogContextMenu* menu = static_cast<CWebGUIDialogContextMenu*>(cbhdl);
//   return menu->OnInit();
// }
//
// bool CWebGUIDialogContextMenu::OnClickCB(GUIHANDLE cbhdl, int controlId)
// {
//   CWebGUIDialogContextMenu* menu = static_cast<CWebGUIDialogContextMenu*>(cbhdl);
//   return menu->OnClick(controlId);
// }
//
// bool CWebGUIDialogContextMenu::OnFocusCB(GUIHANDLE cbhdl, int controlId)
// {
//   CWebGUIDialogContextMenu* menu = static_cast<CWebGUIDialogContextMenu*>(cbhdl);
//   return menu->OnFocus(controlId);
// }
//
// bool CWebGUIDialogContextMenu::OnActionCB(GUIHANDLE cbhdl, int actionId)
// {
//   CWebGUIDialogContextMenu* menu = static_cast<CWebGUIDialogContextMenu*>(cbhdl);
//   return menu->OnAction(actionId);
// }
//}

/*!
 * @brief Dynamic functions
 */
//{
CWebGUIDialogContextMenu::CWebGUIDialogContextMenu()
{
}

CWebGUIDialogContextMenu::~CWebGUIDialogContextMenu()
{

}

bool CWebGUIDialogContextMenu::Open(
  CefRefPtr<CefBrowser>                 browser,
  CefRefPtr<CefFrame>                   frame,
  CefRefPtr<CefContextMenuParams>       params,
  CefRefPtr<CefMenuModel>               model)
{
  m_browser = browser;
  m_frame = frame;
  m_params = params;
  m_model = model;

  /* Load the Window as Dialog */
/*  m_window = GUI->Window_create("DialogContextMenu.xml", "Confluence", false, true);
  m_window->m_cbhdl   = this;
  m_window->CBOnInit  = OnInitCB;
  m_window->CBOnFocus = OnFocusCB;
  m_window->CBOnClick = OnClickCB;
  m_window->CBOnAction= OnActionCB;
  m_window->DoModal();

  GUI->Window_destroy(m_window);
*/
  return true;
}

bool CWebGUIDialogContextMenu::OnClick(int controlId)
{
  return false;
}

bool CWebGUIDialogContextMenu::OnFocus(int controlId)
{
  return false;
}

bool CWebGUIDialogContextMenu::OnInit()
{
  return false;
}

bool CWebGUIDialogContextMenu::OnAction(int actionId)
{
  return false;
}
//}
