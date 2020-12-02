/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DOMVisitor.h"
#include "../MessageIds.h"

CDOMVisitor::CDOMVisitor(CefRefPtr<CefBrowser> browser)
  : m_browser(browser)
{

}

void CDOMVisitor::Visit(CefRefPtr<CefDOMDocument> document)
{

}
