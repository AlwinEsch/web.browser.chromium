/*
 *      Copyright (C) 2015-2019 Team KODI
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

#include "DialogBrowserContextMenu.h"

#include "addon.h"
#include "WebBrowserClient.h"
#include "utils/Utils.h"

#include <kodi/General.h>
#include <kodi/gui/dialogs/ContextMenu.h>
#include <thread>

/**
 * @todo For context menu's must be a own dialog system added to support
 * the types:
 *  - Checkbox
 *  - Radiobutton
 *  - Submenus
 *  - Separators
 *
 * Currently with Kodi's context menu system only the button is supported.
 */

//#define DEBUG_LOGS

// Custom menu command Ids.
enum client_menu_ids {
  CLIENT_ID_OPEN_SELECTED_SIDE = MENU_ID_USER_FIRST,
  CLIENT_ID_OPEN_SELECTED_SIDE_IN_NEW_TAB,
  CLIENT_ID_OPEN_KEYBOARD,
};

CBrowerDialogContextMenu::CBrowerDialogContextMenu(CWebBrowserClient* client) : m_client(client)
{

}

void CBrowerDialogContextMenu::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                                   CefRefPtr<CefFrame> frame,
                                                   CefRefPtr<CefContextMenuParams> params,
                                                   CefRefPtr<CefMenuModel> model)
{
  std::string url = params->GetLinkUrl().ToString();
  if (!url.empty())
  {
    model->InsertItemAt(0, CLIENT_ID_OPEN_SELECTED_SIDE, kodi::GetLocalizedString(30000 + CLIENT_ID_OPEN_SELECTED_SIDE));
    if (!m_client->IsFullscreen())
      model->InsertItemAt(0, CLIENT_ID_OPEN_SELECTED_SIDE_IN_NEW_TAB, kodi::GetLocalizedString(30000 + CLIENT_ID_OPEN_SELECTED_SIDE_IN_NEW_TAB));
  }

  int flags = params->GetTypeFlags();
  if (flags & CM_TYPEFLAG_EDITABLE)
    model->InsertItemAt(0, CLIENT_ID_OPEN_KEYBOARD, kodi::GetLocalizedString(30000 + CLIENT_ID_OPEN_KEYBOARD));

#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "CefContextMenuParams");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- %ix%i - TypeFlags: 0x%X - ImageContents: %s - MediaType: %i - MediaStateFlags %i - EditStateFlags %i", params->GetXCoord(),
      params->GetYCoord(), (int)params->GetTypeFlags(), params->HasImageContents() ? "yes" : "no",
      (int)params->GetMediaType(), (int)params->GetMediaStateFlags(), (int)params->GetEditStateFlags());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- LinkUrl:                %s", params->GetLinkUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- UnfilteredLinkUrl:      %s", params->GetUnfilteredLinkUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- SourceUrl:              %s", params->GetSourceUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- PageUrl:                %s", params->GetPageUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- FrameUrl :              %s", params->GetFrameUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- FrameCharset :          %s", params->GetFrameCharset().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- SelectionText :         %s", params->GetSelectionText().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- MisspelledWord :        %s", params->GetMisspelledWord().ToString().c_str());
  std::vector<CefString> suggestions;
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- DictionarySuggestions : %s", params->GetDictionarySuggestions(suggestions) ? "OK" : "fail");
  for (unsigned int i = 0; i < suggestions.size(); i++)
    LOG_MESSAGE(ADDON_LOG_DEBUG, "  - %02i: %s", i, suggestions[i].ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsEditable :            %s", params->IsEditable() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsSpellCheckEnabled :   %s", params->IsSpellCheckEnabled() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsCustomMenu :          %s", params->IsCustomMenu() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsPepperMenu :          %s", params->IsPepperMenu() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "CefMenuModel");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- Count:                  %i", model->GetCount());
  for (unsigned int i = 0; i < model->GetCount(); i++)
    LOG_MESSAGE(ADDON_LOG_DEBUG, "  - %02i: ID '%i' Type '%i' - Name '%s'",
                    i, model->GetCommandIdAt(i), model->GetTypeAt(i), model->GetLabelAt(i).ToString().c_str());
#endif
}

bool CBrowerDialogContextMenu::RunContextMenu(CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame,
                                              CefRefPtr<CefContextMenuParams> params,
                                              CefRefPtr<CefMenuModel> model,
                                              CefRefPtr<CefRunContextMenuCallback> callback)
{
  std::vector<std::pair<int, std::string>> entries;
  for (unsigned int i = 0; i < model->GetCount(); ++i)
  {
    int id = model->GetCommandIdAt(i);
    if (id < 0 ||
        id == MENU_ID_PRINT ||
        id == MENU_ID_VIEW_SOURCE ||
        !model->IsEnabled(id))
    {
      // ignored parts!
      continue;
    }

    cef_menu_item_type_t type = model->GetTypeAt(i);
    switch (type)
    {
      case MENUITEMTYPE_COMMAND:
        entries.push_back(std::pair<int, std::string>(id, kodi::GetLocalizedString(30000 + id)));
        break;
      // TODO add support for other formats e.g. boolean check
      case MENUITEMTYPE_CHECK:
      case MENUITEMTYPE_RADIO:
      case MENUITEMTYPE_SUBMENU:
        LOG_MESSAGE(ADDON_LOG_ERROR, "Context menu \"cef_menu_item_type_t '%i'\" currently not supported!", type);
        break;
      // TODO add support for separator
      case MENUITEMTYPE_SEPARATOR:
        break;
      case MENUITEMTYPE_NONE:
      default:
        break;
    }
  }

  if (!entries.empty())
  {
    m_client->SetContextMenuOpen(true);
    /* Start the independent thread to prevent block on main thread */
    std::thread(RunContextMenuProcess, m_client, entries, callback).detach();
  }
  else
  {
    /* If no entries cancel the context menu */
    callback->Cancel();
  }

  /* Always true, otherwise Chromium's own context menu might be displayed. */
  return true;
}

void CBrowerDialogContextMenu::RunContextMenuProcess(CWebBrowserClient* client,
                                                     std::vector<std::pair<int, std::string>> entries,
                                                     CefRefPtr<CefRunContextMenuCallback> callback)
{
  int ret = kodi::gui::dialogs::ContextMenu::Show("", entries);
  if (ret >= 0)
    callback->Continue(entries[ret].first, EVENTFLAG_LEFT_MOUSE_BUTTON);
  else
    callback->Cancel();

  client->SetContextMenuOpen(false);
}

bool CBrowerDialogContextMenu::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                                    CefRefPtr<CefFrame> frame,
                                                    CefRefPtr<CefContextMenuParams> params,
                                                    int command_id,
                                                    EventFlags event_flags)
{
  CEF_REQUIRE_UI_THREAD();
  switch (command_id)
  {
    case CLIENT_ID_OPEN_SELECTED_SIDE:
      m_client->OpenWebsite(params->GetLinkUrl().ToString());
      break;
    case CLIENT_ID_OPEN_SELECTED_SIDE_IN_NEW_TAB:
      m_client->RequestOpenSiteInNewTab(params->GetLinkUrl().ToString());
      break;
    case CLIENT_ID_OPEN_KEYBOARD:
      m_client->GetMain().GetGUIManager().GetKeyboard().Show(m_client, CEF_TEXT_INPUT_MODE_DEFAULT);
      break;
    default:
      return false;
  }

  return true;
}
