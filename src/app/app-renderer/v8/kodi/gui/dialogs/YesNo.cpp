/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "YesNo.h"

// Dev kit
#include "../../../../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/YesNo.h"

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
namespace gui
{
namespace dialogs
{

CYesNo::CYesNo(const CefRefPtr<CefV8Value>& newObject,
               const CefRefPtr<CefV8Value>& parent,
               CefV8Handler* handler)
{
  // clang-format off
  parent->SetValue("YesNo", newObject, V8_PROPERTY_ATTRIBUTE_NONE);
  newObject->SetValue("ShowAndGetInput", CefV8Value::CreateFunction("kodi_gui_dialogs_CYesNo_ShowAndGetInput", handler), V8_PROPERTY_ATTRIBUTE_NONE);
  // clang-format on
}

bool CYesNo::Execute(const CefString& name,
                     CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& exception)
{
  if (name == "kodi_gui_dialogs_CYesNo_ShowAndGetInput")
  {
    if (!arguments[0]->IsString() || !arguments[1]->IsString())
    {
      exception = "Wrong value on '" + name.ToString() + "'";
      return true;
    }

    bool canceled;
    std::string yes;
    std::string no;
    if (arguments.size() > 2 && arguments[2]->IsString())
    {
      no = arguments[2]->GetStringValue();
      if (arguments.size() > 3 && arguments[3]->IsString())
      {
        yes = arguments[3]->GetStringValue();
      }
    }
    bool ret = ::kodi::gui::dialogs::YesNo::ShowAndGetInput(
        arguments[0]->GetStringValue(), arguments[1]->GetStringValue(), canceled, no, yes);
    CefRefPtr<CefV8Value> objRet = CefV8Value::CreateObject(nullptr, nullptr);
    objRet->SetValue("ret", CefV8Value::CreateBool(ret), V8_PROPERTY_ATTRIBUTE_READONLY);
    objRet->SetValue("canceled", CefV8Value::CreateBool(canceled), V8_PROPERTY_ATTRIBUTE_READONLY);
    retval = objRet;
    return true;
  }

  return false;
}

} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
} /* namespace v8 */
} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
