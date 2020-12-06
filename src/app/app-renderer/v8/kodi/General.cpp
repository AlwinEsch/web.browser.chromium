/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "General.h"

// Dev kit
#include "../../../../../lib/kodi-dev-kit/include/kodi/General.h"

namespace chromium
{
namespace app
{
namespace renderer
{
namespace v8
{
namespace kodi
{

CGeneral::CGeneral(const CefRefPtr<CefV8Value>& newObject,
                   const CefRefPtr<CefV8Value>& parent,
                   CefV8Handler* handler)
{
  // clang-format off
  // kodi's typedef enum QueueMsg
  parent->SetValue("QUEUE_INFO", CefV8Value::CreateUInt(QUEUE_INFO), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("QUEUE_WARNING", CefV8Value::CreateUInt(QUEUE_WARNING), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("QUEUE_ERROR", CefV8Value::CreateUInt(QUEUE_ERROR), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("QUEUE_OWN_STYLE", CefV8Value::CreateUInt(QUEUE_OWN_STYLE), V8_PROPERTY_ATTRIBUTE_READONLY);

  // kodi's typedef enum ADDON_STATUS
  parent->SetValue("ADDON_STATUS_OK", CefV8Value::CreateUInt(ADDON_STATUS_OK), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_STATUS_LOST_CONNECTION", CefV8Value::CreateUInt(ADDON_STATUS_LOST_CONNECTION), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_STATUS_NEED_RESTART", CefV8Value::CreateUInt(ADDON_STATUS_NEED_RESTART), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_STATUS_NEED_SETTINGS", CefV8Value::CreateUInt(ADDON_STATUS_NEED_SETTINGS), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_STATUS_UNKNOWN", CefV8Value::CreateUInt(ADDON_STATUS_UNKNOWN), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_STATUS_PERMANENT_FAILURE", CefV8Value::CreateUInt(ADDON_STATUS_PERMANENT_FAILURE), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_STATUS_NOT_IMPLEMENTED", CefV8Value::CreateUInt(ADDON_STATUS_NOT_IMPLEMENTED), V8_PROPERTY_ATTRIBUTE_READONLY);

  // kodi's typedef enum AddonLog
  parent->SetValue("ADDON_LOG_DEBUG", CefV8Value::CreateUInt(ADDON_LOG_DEBUG), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_LOG_INFO", CefV8Value::CreateUInt(ADDON_LOG_INFO), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_LOG_WARNING", CefV8Value::CreateUInt(ADDON_LOG_WARNING), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_LOG_ERROR", CefV8Value::CreateUInt(ADDON_LOG_ERROR), V8_PROPERTY_ATTRIBUTE_READONLY);
  parent->SetValue("ADDON_LOG_FATAL", CefV8Value::CreateUInt(ADDON_LOG_FATAL), V8_PROPERTY_ATTRIBUTE_READONLY);

  // Namespace ::kodi functions
  parent->SetValue("kodi", newObject, V8_PROPERTY_ATTRIBUTE_NONE);
  newObject->SetValue("Log", CefV8Value::CreateFunction("kodi_CGeneral_kodi_Log", handler), V8_PROPERTY_ATTRIBUTE_NONE);
  newObject->SetValue("QueueNotification", CefV8Value::CreateFunction("kodi_CGeneral_kodi_QueueNotification", handler), V8_PROPERTY_ATTRIBUTE_NONE);
  newObject->SetValue("GetAddonInfo", CefV8Value::CreateFunction("kodi_CGeneral_kodi_GetAddonInfo", handler), V8_PROPERTY_ATTRIBUTE_NONE);
// clang-format on
}

bool CGeneral::Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception)
{
  if (name == "kodi_CGeneral_kodi_Log")
  {
    if (arguments.size() == 2)
    {
      if (!arguments[0]->IsInt() || !arguments[1]->IsString())
      {
        exception = "Wrong values on '" + name.ToString() + "'";
        return true;
      }
      const AddonLog level = static_cast<AddonLog>(arguments[0]->GetIntValue());
      const std::string message = arguments[1]->GetStringValue();

      ::kodi::Log(level, message.c_str());
    }
    else if (arguments.size() == 1)
    {
      CefRefPtr<CefV8Value> opt = arguments[0];

      if (!opt->HasValue("message"))
      {
        exception = "Missing value 'message' on '" + name.ToString() + "'";
        return true;
      }

      const AddonLog level = static_cast<AddonLog>(
          opt->HasValue("level") ? opt->GetValue("level")->GetIntValue() : ADDON_LOG_INFO);
      const std::string message = opt->GetValue("message")->GetStringValue();

      ::kodi::Log(level, message.c_str());
    }
    else
    {
      exception = "No values on '" + name.ToString() + "'";
    }

    return true;
  }
  //--------------------------------------------------------------------------
  if (name == "kodi_CGeneral_kodi_QueueNotification")
  {
    CefRefPtr<CefV8Value> opt = arguments[0];

    if (!opt->HasValue("message"))
    {
      exception = "Missing value 'message' on '" + name.ToString() + "'";
      return true;
    }

    const std::string header = opt->GetValue("header")->GetStringValue();
    const std::string message = opt->GetValue("message")->GetStringValue();
    const QueueMsg type = static_cast<QueueMsg>(
        opt->HasValue("type") ? opt->GetValue("type")->GetIntValue() : QUEUE_INFO);
    const std::string imageFile =
        opt->HasValue("imageFile") ? opt->GetValue("imageFile")->GetStringValue() : "";
    const int displayTime =
        opt->HasValue("displayTime") ? opt->GetValue("displayTime")->GetIntValue() : 5000;
    const bool withSound =
        opt->HasValue("withSound") ? opt->GetValue("withSound")->GetBoolValue() : true;
    const int messageTime =
        opt->HasValue("messageTime") ? opt->GetValue("messageTime")->GetIntValue() : 5000;

    ::kodi::QueueNotification(type, header, message, imageFile, displayTime, withSound,
                              messageTime);
    return true;
  }
  //--------------------------------------------------------------------------
  if (name == "kodi_CGeneral_kodi_GetAddonInfo")
  {
    if (!arguments[0]->IsString())
    {
      exception = "Wrong value on '" + name.ToString() + "'";
      return true;
    }

    const std::string ret = ::kodi::GetAddonInfo(arguments[0]->GetStringValue());
    retval = CefV8Value::CreateString(ret);
    return true;
  }

  return false;
}

} /* namespace kodi */
} /* namespace v8 */
} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
