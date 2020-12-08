/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Scheme.h"

namespace chromium
{
namespace app
{
namespace common
{

void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  registrar->AddCustomScheme("kodi", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
}

} /* namespace common */
} /* namespace app */
} /* namespace chromium */
