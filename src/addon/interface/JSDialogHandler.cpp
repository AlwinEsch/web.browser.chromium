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

#include "JSDialogHandler.h"
#include "WebBrowserClient.h"
#include "utils/Utils.h"
#include "utils/StringUtils.h"

#include "include/cef_parser.h"

#include <kodi/General.h>
#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/gui/dialogs/Keyboard.h>
#include <thread>

bool CJSDialogHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                  const CefString& origin_url,
                                  JSDialogType dialog_type,
                                  const CefString& message_text,
                                  const CefString& default_prompt_text,
                                  CefRefPtr<CefJSDialogCallback> callback,
                                  bool& suppress_message)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "OnJSDialog (origin_url '%s', dialog_type %X message_text '%s', default_prompt_text '%s', suppress_message %s)",
                       origin_url.ToString().c_str(), dialog_type, message_text.ToString().c_str(), default_prompt_text.ToString().c_str(),
                       suppress_message ? "yes" : "no");

  switch (dialog_type)
  {
    case JSDIALOGTYPE_ALERT:
    case JSDIALOGTYPE_CONFIRM:
    case JSDIALOGTYPE_PROMPT:
      std::thread(OnJSDialogProcess, origin_url.ToString(), dialog_type, message_text.ToString(), default_prompt_text.ToString(), callback).detach();
      break;
    default:
      return false;
  }

  return true;
}

void CJSDialogHandler::OnJSDialogProcess(std::string origin_url,
                                         JSDialogType dialog_type,
                                         std::string message_text,
                                         std::string default_prompt_text,
                                         CefRefPtr<CefJSDialogCallback> callback)
{
  switch (dialog_type)
  {
    case JSDIALOGTYPE_ALERT:
    {
      kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30033), message_text);
      callback->Continue(true, "");
      break;
    }
    case JSDIALOGTYPE_CONFIRM:
    {
      bool canceled;
      bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30034), message_text, canceled);
      callback->Continue(ret, "");
      break;
    }
    case JSDIALOGTYPE_PROMPT:
    {
      std::string text;
      std::string header = StringUtils::Format(kodi::GetLocalizedString(30035).c_str(), message_text.c_str());
      bool ret = kodi::gui::dialogs::Keyboard::ShowAndGetInput(text, header, true, false);
      callback->Continue(ret, text);
      break;
    }
    default:
      break;
  }
}

bool CJSDialogHandler::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
                                            const CefString& message_text,
                                            bool is_reload,
                                            CefRefPtr<CefJSDialogCallback> callback)
{
  bool canceled;
  bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(CefFormatUrlForSecurityDisplay(browser->GetFocusedFrame()->GetURL()).ToString(),
                                                        kodi::GetLocalizedString(30036), canceled);
  callback->Continue(ret, "");

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "OnBeforeUnloadDialog (url '%s', message_text '%s', is_reload '%s')",
                       CefFormatUrlForSecurityDisplay(browser->GetFocusedFrame()->GetURL()).ToString().c_str(),
                       message_text.ToString().c_str(), is_reload ? "yes" : "no");
  return true;
}
