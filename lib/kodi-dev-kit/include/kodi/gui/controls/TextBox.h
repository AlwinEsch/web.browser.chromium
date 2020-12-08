/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

namespace kodi
{
namespace gui
{

class CWindow;

namespace controls
{

class CTextBox
{
public:
  CTextBox(CWindow* window, int controlId);
  ~CTextBox();

  void SetVisible(bool visible);
  void Reset();
  void SetText(const std::string& label);
  std::string GetText() const;
  void Scroll(unsigned int position);
  void SetAutoScrolling(int delay, int time, int repeat);

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
