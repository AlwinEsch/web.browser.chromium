/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// Own
#include "IV8SubPart.h"

// CEF
#include "include/cef_v8.h"
#include "include/wrapper/cef_helpers.h"

// Global
#include <memory>
#include <vector>

namespace chromium
{
namespace app
{
namespace renderer
{

class CWebAppRenderer;

namespace v8
{

class CV8Handler : public CefV8Handler
{
public:
  CV8Handler(CWebAppRenderer& renderer);
  ~CV8Handler() override;

  bool InitKodiAPI(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   CefRefPtr<CefV8Context> context);

  bool Execute(const CefString& name,
               CefRefPtr<CefV8Value> object,
               const CefV8ValueList& arguments,
               CefRefPtr<CefV8Value>& retval,
               CefString& exception) override;

private:
  IMPLEMENT_REFCOUNTING(CV8Handler);
  DISALLOW_COPY_AND_ASSIGN(CV8Handler);

  CWebAppRenderer& m_renderer;

  std::vector<std::shared_ptr<IV8SubPart>> m_subParts;
};

} /* namespace v8 */
} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
