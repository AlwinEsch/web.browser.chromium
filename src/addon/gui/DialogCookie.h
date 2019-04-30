/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_cookie.h"

#include <kodi/gui/Window.h>
#include <mutex>

class CBrowserDialogCookie : public kodi::gui::CWindow
{
public:
  CBrowserDialogCookie();

  void Open();

  bool OnInit() override;
  bool OnClick(int controlId) override;
  void GetContextButtons(int itemNumber, std::vector<std::pair<unsigned int, std::string>> &buttons) override;
  bool OnContextButton(int itemNumber, unsigned int button) override;

  void AddCookie(const CefCookie& cookie);

private:
  void AddGUIEntry(const CefCookie& cookie);

  bool m_inited;
  std::mutex m_mutex;
  std::vector<CefCookie> m_items;
  int m_findPosition;
  std::string m_lastSearchText;
};
