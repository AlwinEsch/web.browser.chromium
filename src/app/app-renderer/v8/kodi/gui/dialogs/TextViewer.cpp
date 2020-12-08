/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "TextViewer.h"

// Dev kit
#include "../../../../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/TextViewer.h"

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

CTextViewer::CTextViewer(const CefRefPtr<CefV8Value>& newObject,
               const CefRefPtr<CefV8Value>& parent,
               CefV8Handler* handler)
{
  // clang-format off
  parent->SetValue("TextViewer", newObject, V8_PROPERTY_ATTRIBUTE_NONE);
  newObject->SetValue("Show", CefV8Value::CreateFunction("kodi_gui_dialogs_CTextViewer_Show", handler), V8_PROPERTY_ATTRIBUTE_NONE);
  // clang-format on
}

bool CTextViewer::Execute(const CefString& name,
                     CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& exception)
{
  if (name == "kodi_gui_dialogs_CTextViewer_Show")
  {
    if (!arguments[0]->IsString() || !arguments[1]->IsString())
    {
      exception = "Wrong value on '" + name.ToString() + "'";
      return true;
    }

    ::kodi::gui::dialogs::TextViewer::Show(arguments[0]->GetStringValue(), arguments[1]->GetStringValue());
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
