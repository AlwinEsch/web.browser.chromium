/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "MessageJSHandler.h"

// Own
#include "../WebBrowserClient.h"

// CEF
#include "include/cef_parser.h"

// DevKit
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/OK.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/TextViewer.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/YesNo.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// Global
#include <thread>

using kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace main
{
namespace interface
{

namespace
{
static std::atomic_int m_ctorcount{0}; // For debug purposes and to see destructs done
}

CMessageJSHandler::CMessageJSHandler(CefRefPtr<CWebBrowserClient> client)
  : m_client(client)
{
  fprintf(stderr, "CMessageJSHandler START (%p) count open %i\n", this, ++m_ctorcount);
}

CMessageJSHandler::~CMessageJSHandler()
{
  fprintf(stderr, "CMessageJSHandler STOP (%p) count open %i\n", this, --m_ctorcount);
}

bool CMessageJSHandler::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id,
                         const CefString& request, bool persistent, CefRefPtr<Callback> callback)
{
  CEF_REQUIRE_UI_THREAD();

  const std::string url = frame->GetURL().ToString();
  CefRefPtr<CefValue> value = CefParseJSON(request, JSON_PARSER_RFC);
  if (!value.get())
    return false;

  std::thread([url, query_id, value, persistent, callback]() {
    if (value->GetType() == VTYPE_DICTIONARY)
    {
      CefRefPtr<CefDictionaryValue> dict = value->GetDictionary();
      std::string func = dict->GetString("func");
      if (!func.empty())
      {
        CefRefPtr<CefDictionaryValue> params = dict->GetDictionary("params");

        if (func == "kodi.gui.dialogs.OK.ShowAndGetInput")
        {
          kodi::gui::dialogs::OK::ShowAndGetInput(params->GetString("heading"), params->GetString("text"));
          callback->Success(CefString());
          return;
        }
        if (func == "kodi.gui.dialogs.TextViewer.Show")
        {
          kodi::gui::dialogs::TextViewer::Show(params->GetString("heading"), params->GetString("text"));
          callback->Success(CefString());
          return;
        }
        if (func == "kodi.gui.dialogs.YesNo.ShowAndGetInput")
        {
          bool canceled = false;
          const bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(params->GetString("heading"), params->GetString("text"), canceled, params->GetString("no"), params->GetString("yes"));
          const std::string jsonRet = StringUtils::Format("{\"returns\": { \"ret\":%s, \"canceled\":%s } }", ret ? "true" : "false", canceled ? "true" : "false");
          callback->Success(jsonRet);
          return;
        }
      }
    }
  }).detach();
  return true;
}

void CMessageJSHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id)
{
  fprintf(stderr, "OnQueryCanceled\n");

}

} /* namespace interface */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
