/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_browser.h"
#include "include/cef_dom.h"

class CDOMVisitor : public CefDOMVisitor
{
public:
  explicit CDOMVisitor(CefRefPtr<CefBrowser> browser);

  void Visit(CefRefPtr<CefDOMDocument> document) override;

private:
  IMPLEMENT_REFCOUNTING(CDOMVisitor);
  DISALLOW_COPY_AND_ASSIGN(CDOMVisitor);

  CefRefPtr<CefBrowser> m_browser;
};
