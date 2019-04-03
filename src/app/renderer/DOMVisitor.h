/*
 *      Copyright (C) 2015-2019 Team KODI
 *      http:/kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
