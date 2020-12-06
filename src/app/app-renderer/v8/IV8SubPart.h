/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_v8.h"
#include "include/wrapper/cef_helpers.h"

namespace chromium
{
namespace app
{
namespace renderer
{
namespace v8
{

class IV8SubPart
{
public:
  IV8SubPart() = default;
  virtual ~IV8SubPart() = default;

  virtual const std::string& GetIdentfier() const = 0;

  virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) = 0;
};

} /* namespace v8 */
} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
