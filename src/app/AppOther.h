/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_app.h"

// Client app implementation for other process types.
class CWebAppOther : public CefApp
{
 public:
  CWebAppOther() = default;

  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

 private:
  IMPLEMENT_REFCOUNTING(CWebAppOther);
  DISALLOW_COPY_AND_ASSIGN(CWebAppOther);
};
