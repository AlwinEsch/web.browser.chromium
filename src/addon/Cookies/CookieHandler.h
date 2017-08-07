#pragma once
/*
 *      Copyright (C) 2015-2017 Team KODI
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

#include "include/cef_cookie.h"

#include <kodi/gui/Window.h>
#include <p8-platform/threads/threads.h>

class CCookieHandler : public kodi::gui::CWindow
{
public:
  CCookieHandler();

  void Open();

  virtual bool OnInit() override;
  virtual bool OnClick(int controlId) override;
  virtual void GetContextButtons(int itemNumber, std::vector<std::pair<unsigned int, std::string>> &buttons) override;
  virtual bool OnContextButton(int itemNumber, unsigned int button) override;

  void AddCookie(const CefCookie& cookie);

private:
  void AddGUIEntry(const CefCookie& cookie);

  bool m_inited;
  P8PLATFORM::CMutex m_cookieMutex;
  std::vector<CefCookie> m_items;
  int m_findPosition;
  std::string m_lastSearchText;
};
