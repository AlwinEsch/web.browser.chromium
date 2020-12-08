/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppOther.h"
#include "../common/Scheme.h"

void CWebAppOther::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  chromium::app::common::RegisterCustomSchemes(registrar);
}
