/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_cookie.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/Window.h"

// Global
#include <mutex>

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

class CBrowserDialogCookie : public kodi::gui::CWindow
{
public:
  CBrowserDialogCookie();

  void Open();

  bool OnInit() override;
  bool OnClick(int controlId) override;
  void GetContextButtons(int itemNumber,
                         std::vector<std::pair<unsigned int, std::string>>& buttons) override;
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

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
