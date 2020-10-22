/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "V8Handler.h"
#include "AppRenderer.h"
#include "MessageIds.h"

#include <kodi/General.h>

/*
 * VERY BIG TODO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * Create Sandbox interface between here and Kodi to allow correct interact with V8 JavaScripts.
 *
 * With use of SendProcessMessage(...) or with script `window.kodiQuery()` goes on Kodi's
 * side over main loop and blocks the flow.
 * Basically does it work and if on Kodi is not a website open with stream does it mostly work,
 * but unsafe.
 *
 * With `window.kodiQuery` are currently the most working where the call on Kodi main thread
 * becomes separated to new Thread and this then do the callback for here.
 *
 * Also becomes it a bit muddled up if all Kodi addon interface functions are added by hand.
 *
 *
 * JavaScript testcalls
function test_kodi_Log()
{
  kodi.Log(ADDON_LOG_ERROR, "ADDON_LOG_ERROR test call");
}

function test_kodi_QueueNotification()
{
  kodi.QueueNotification({header:"Test call header", message:"Hello World!"});
}

function test_kodi_GetAddonInfo()
{
  kodi.GetAddonInfo('id', function(name) { alert("GetAddonInfo: "+name); });
}

function test_kodi_DialogOK()
{
  kodi.gui.dialogs.OK.ShowAndGetInput('Test', "Hello World!");
}

function test_kodi_DialogYesNo()
{
  kodi.gui.dialogs.YesNo.ShowAndGetInput('Test', "Hello World!", function(ret)
  {
    document.getElementById('yesNoRet').innerText = "returned "+ret;
  });
}
 */

bool CV8Handler::Execute(const CefString& name,
                         CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& exception)
{
  if (!m_renderer->GetBrowser() ||
      !m_renderer->CurrentSiteInterfaceAllowed())
    return false;

  CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
  std::string url = context->GetFrame()->GetURL().ToString();

  if (name == "QueueNotification")
  {
    CefRefPtr<CefV8Value> opt = arguments[0];

    if (!opt->HasValue("message"))
    {
      exception = "Missing value 'message' on '" + name.ToString() + "'";
      return true;
    }

    std::string header = opt->GetValue("header")->GetStringValue();
    std::string message = opt->GetValue("message")->GetStringValue();
    int type = opt->HasValue("type") ? opt->GetValue("type")->GetIntValue() : QUEUE_INFO;
    std::string imageFile = opt->HasValue("imageFile") ? opt->GetValue("imageFile")->GetStringValue() : "";
    int displayTime = opt->HasValue("displayTime") ? opt->GetValue("displayTime")->GetIntValue() : 5000;
    bool withSound = opt->HasValue("withSound") ? opt->GetValue("withSound")->GetBoolValue() : true;
    int messageTime = opt->HasValue("messageTime") ? opt->GetValue("messageTime")->GetIntValue() : 5000;

    CefRefPtr<CefProcessMessage> browserMessage = CefProcessMessage::Create(RendererMessage::V8AddonCall);
    browserMessage->GetArgumentList()->SetString(0, name);
    browserMessage->GetArgumentList()->SetInt(1, type);
    browserMessage->GetArgumentList()->SetString(2, header);
    browserMessage->GetArgumentList()->SetString(3, message);
    browserMessage->GetArgumentList()->SetString(4, imageFile);
    browserMessage->GetArgumentList()->SetInt(5, displayTime);
    browserMessage->GetArgumentList()->SetBool(6, withSound);
    browserMessage->GetArgumentList()->SetInt(7, messageTime);
    m_renderer->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_BROWSER, browserMessage);
    return true;
  }

  return false;
}

void CV8Handler::OnWebKitInitialized(CWebAppRenderer* renderer)
{
  // Register the client_app extension.
  std::string app_code =
    "var kodi;"
    "if (!kodi) {"
    "    kodi = {};"
    "    kodi.gui = {};"
    "    kodi.gui.dialogs = {};"
    "    kodi.gui.dialogs.OK = {};"
    "    kodi.gui.dialogs.YesNo = {};"
    "}"
    "const QUEUE_INFO = " + std::to_string(QUEUE_INFO) + ";"
    "const QUEUE_WARNING = " + std::to_string(QUEUE_WARNING) + ";"
    "const QUEUE_ERROR = " + std::to_string(QUEUE_ERROR) + ";"
    "const QUEUE_OWN_STYLE = " + std::to_string(QUEUE_OWN_STYLE) + ";"
    ""
    "const ADDON_STATUS_OK = " + std::to_string(ADDON_STATUS_OK) + ";"
    "const ADDON_STATUS_LOST_CONNECTION = " + std::to_string(ADDON_STATUS_LOST_CONNECTION) + ";"
    "const ADDON_STATUS_NEED_RESTART = " + std::to_string(ADDON_STATUS_NEED_RESTART) + ";"
    "const ADDON_STATUS_NEED_SETTINGS = " + std::to_string(ADDON_STATUS_NEED_SETTINGS) + ";"
    "const ADDON_STATUS_UNKNOWN = " + std::to_string(ADDON_STATUS_UNKNOWN) + ";"
    "const ADDON_STATUS_PERMANENT_FAILURE = " + std::to_string(ADDON_STATUS_PERMANENT_FAILURE) + ";"
    "const ADDON_STATUS_NOT_IMPLEMENTED = " + std::to_string(ADDON_STATUS_NOT_IMPLEMENTED) + ";"
    ""
    "const ADDON_LOG_DEBUG = " + std::to_string(ADDON_LOG_DEBUG) + ";"
    "const ADDON_LOG_INFO = " + std::to_string(ADDON_LOG_INFO) + ";"
    "const ADDON_LOG_WARNING = " + std::to_string(ADDON_LOG_WARNING) + ";"
    "const ADDON_LOG_ERROR = " + std::to_string(ADDON_LOG_ERROR) + ";"
    "const ADDON_LOG_FATAL = " + std::to_string(ADDON_LOG_FATAL) + ";"
    ""
    "(function() {"
    "  kodi.Log = function(level, text) {"
    "    window.kodiQuery({"
    "      request: 'kodi.Log '+level+':'+text ,"
    "      onSuccess: function(response) {},"
    "      onFailure: function(error_code, error_message) {}"
    "    });"
    "  };"
    "  kodi.QueueNotification = function(options) {"
    "    native function QueueNotification();"
    "    return QueueNotification(options);"
    "  };"
    "  kodi.GetAddonInfo = function(id, cb) {"
    "    window.kodiQuery({"
    "      request: 'kodi.GetAddonInfo '+id ,"
    "      onSuccess: function(response) {"
    "        cb(response);"
    "      },"
    "      onFailure: function(error_code, error_message) {"
    "        alert(error_message);"
    "      }"
    "    });"
    "  };"
    "  kodi.gui.dialogs.OK.ShowAndGetInput = function(heading, text, cb) {"
    "    window.kodiQuery({"
    "      request: 'kodi.gui.dialog.OK.ShowAndGetInput '+heading+':'+text ,"
    "      onSuccess: function(response) {"
    "        if (cb)"
    "          cb();"
    "      },"
    "      onFailure: function(error_code, error_message) {"
    "        alert(error_message);"
    "      }"
    "    });"
    "  };"
    "  kodi.gui.dialogs.YesNo.ShowAndGetInput = function(heading, text, cb) {"
    "    window.kodiQuery({"
    "      request: 'kodi.gui.dialog.YesNo.ShowAndGetInput '+heading+':'+text ,"
    "      onSuccess: function(response) {"
    "        if (cb)"
    "          cb(true);"
    "      },"
    "      onFailure: function(error_code, error_message) {"
    "        if (cb)"
    "          cb(false);"
    "      }"
    "    });"
    "  };"
    "})();";

  CefRegisterExtension("kodi", app_code, new CV8Handler(renderer));
}

