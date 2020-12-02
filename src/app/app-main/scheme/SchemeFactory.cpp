/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "SchemeFactory.h"

// Own
#include "SchemeKodi.h"

// CEF
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

/*
Mostly Final

TODO:
- Other schemes needed or good to have?
*/

namespace chromium
{
namespace app
{
namespace main
{
namespace scheme
{

namespace
{

class CSchemeHandlerFactory : public CefSchemeHandlerFactory
{
public:
  CSchemeHandlerFactory() = default;

  // Return a new scheme handler instance to handle the request.
  CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       const CefString& scheme_name,
                                       CefRefPtr<CefRequest> request) override
  {
    CEF_REQUIRE_IO_THREAD();
    if (scheme_name == "kodi")
      return new CSchemeKodiHandler();

    return nullptr;
  }

  IMPLEMENT_REFCOUNTING(CSchemeHandlerFactory);
  DISALLOW_COPY_AND_ASSIGN(CSchemeHandlerFactory);
};

} /* namespace */

void RegisterSchemeHandlers()
{
  CefRegisterSchemeHandlerFactory("kodi", "home", new CSchemeHandlerFactory());
}

void ClearSchemeHandlers()
{
  CefClearSchemeHandlerFactories();
}

} /* namespace scheme */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
