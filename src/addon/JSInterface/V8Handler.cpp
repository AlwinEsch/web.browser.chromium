/*
 *      Copyright (C) 2015-2017 Team KODI
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

#include "V8Handler.h"
#include "addon.h"

#include <kodi/General.h>

bool CV8Handler::Execute(const CefString& name,
                         CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& exception)
{
  if (name == "Log")
  {
    CefRefPtr<CefV8Value> opt = arguments[0];
    AddonLog level;
    std::string message = "(" + CefV8StackTrace::GetCurrent(1)->GetFrame(0)->GetScriptNameOrSourceURL().ToString() + ") ";
    if (!opt->HasValue("message"))
    {
      if (arguments.size() != 2)
      {
        exception = "Invalid values on '" + name.ToString() + "'";
        return true;
      }

      level = static_cast<AddonLog>(arguments.at(0)->GetIntValue());
      message += arguments.at(1)->GetStringValue();
    }
    else
    {
      level = opt->HasValue("level") ? static_cast<AddonLog>(opt->GetValue("level")->GetIntValue()) : ADDON_LOG_INFO;
      message += opt->GetValue("message")->GetStringValue();
    }
    kodi::Log(level, message.c_str());
    return true;
  }
  else if (name == "QueueNotification")
  {
    CefRefPtr<CefV8Value> opt = arguments[0];

    if (!opt->HasValue("message"))
    {
      exception = "Missing value 'message' on '" + name.ToString() + "'";
      return true;
    }

    std::string header = opt->GetValue("header")->GetStringValue();
    std::string message = opt->GetValue("message")->GetStringValue();
    QueueMsg type = opt->HasValue("type") ? static_cast<QueueMsg>(opt->GetValue("type")->GetIntValue()) : QUEUE_INFO;
    std::string imageFile = opt->HasValue("imageFile") ? opt->GetValue("imageFile")->GetStringValue() : "";
    unsigned int displayTime = opt->HasValue("displayTime") ? opt->GetValue("displayTime")->GetIntValue() : 5000;
    bool withSound = opt->HasValue("withSound") ? opt->GetValue("withSound")->GetBoolValue() : true;
    unsigned int messageTime = opt->HasValue("messageTime") ? opt->GetValue("messageTime")->GetIntValue() : 5000;

    kodi::QueueNotification(type, header, message, imageFile, displayTime, withSound, messageTime);
    return true;
  }
  return false;
}

void CV8Handler::OnWebKitInitialized()
{
  // Register the client_app extension.
  std::string app_code =
    "var kodi;"
    "if (!kodi) {"
    "    kodi = {};"
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
    "const ADDON_LOG_NOTICE = " + std::to_string(ADDON_LOG_NOTICE) + ";"
    "const ADDON_LOG_WARNING = " + std::to_string(ADDON_LOG_WARNING) + ";"
    "const ADDON_LOG_ERROR = " + std::to_string(ADDON_LOG_ERROR) + ";"
    "const ADDON_LOG_SEVERE = " + std::to_string(ADDON_LOG_SEVERE) + ";"
    "const ADDON_LOG_FATAL = " + std::to_string(ADDON_LOG_FATAL) + ";"
    ""
    "(function() {"
    "  kodi.Log = function(name, arguments) {"
    "    native function Log();"
    "    return Log(name, arguments);"
    "  };"
    "  kodi.QueueNotification = function(options) {"
    "    native function QueueNotification();"
    "    return QueueNotification(options);"
    "  };"
    "})();";

  CefRegisterExtension("kodi", app_code, new CV8Handler());
}
