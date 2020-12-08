/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// Own
#include "../../../IV8SubPart.h"

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

class CYesNo : public IV8SubPart
{
public:
  CYesNo(const CefRefPtr<CefV8Value>& newObject,
         const CefRefPtr<CefV8Value>& parent,
         CefV8Handler* handler);
  virtual ~CYesNo() = default;

  const std::string& GetIdentfier() const override { return m_identifier; }
  bool Execute(const CefString& name,
               CefRefPtr<CefV8Value> object,
               const CefV8ValueList& arguments,
               CefRefPtr<CefV8Value>& retval,
               CefString& exception) override;

private:
  const std::string m_identifier{"kodi_gui_dialogs_CYesNo"};
};

} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
} /* namespace v8 */
} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
