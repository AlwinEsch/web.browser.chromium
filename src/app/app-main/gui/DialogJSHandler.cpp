/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogJSHandler.h"

// Own
#include "../WebBrowserClient.h"

// CEF
#include "include/cef_parser.h"

// Dev-Kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/Keyboard.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/OK.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/YesNo.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// Global
#include <thread>

/*
TODO:
- Maybe needed to use own GUI dialogs to support for
  CefJSDialogHandler::OnResetDialogState as on by request not possible at the
  moment.
*/

using kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

bool CJSDialogHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
                                  const CefString& origin_url,
                                  JSDialogType dialog_type,
                                  const CefString& message_text,
                                  const CefString& default_prompt_text,
                                  CefRefPtr<CefJSDialogCallback> callback,
                                  bool& suppress_message)
{
  kodi::Log(ADDON_LOG_DEBUG,
            "OnJSDialog (origin_url '%s', dialog_type %X message_text '%s', default_prompt_text "
            "'%s', suppress_message %s)",
            origin_url.ToString().c_str(), dialog_type, message_text.ToString().c_str(),
            default_prompt_text.ToString().c_str(), suppress_message ? "yes" : "no");

  switch (dialog_type)
  {
    case JSDIALOGTYPE_ALERT:
    case JSDIALOGTYPE_CONFIRM:
    case JSDIALOGTYPE_PROMPT:
      std::thread(OnJSDialogProcess, origin_url.ToString(), dialog_type, message_text.ToString(),
                  default_prompt_text.ToString(), callback)
          .detach();
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
      bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30034),
                                                            message_text, canceled);
      callback->Continue(ret, "");
      break;
    }
    case JSDIALOGTYPE_PROMPT:
    {
      std::string text;
      std::string header =
          StringUtils::Format(kodi::GetLocalizedString(30035).c_str(), message_text.c_str());
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
  const std::string formatURL =
      CefFormatUrlForSecurityDisplay(browser->GetFocusedFrame()->GetURL());
  const std::string messageText = message_text;

  std::thread([formatURL, messageText, is_reload, callback]() {
    bool canceled;
    bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(
        formatURL, kodi::GetLocalizedString(30036), canceled);
    callback->Continue(ret, "");

    kodi::Log(ADDON_LOG_DEBUG, "OnBeforeUnloadDialog (url '%s', message_text '%s', is_reload '%s')",
              formatURL.c_str(), messageText.c_str(), is_reload ? "yes" : "no");
  }).detach();
  return true;
}

void CJSDialogHandler::OnResetDialogState(CefRefPtr<CefBrowser> browser)
{

}

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
