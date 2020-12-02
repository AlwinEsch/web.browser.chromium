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

class CRadioButton
{
public:
  CRadioButton(CWindow* window, int controlId);
  ~CRadioButton();

  void SetVisible(bool visible);
  void SetEnabled(bool enabled);
  void SetLabel(const std::string& label);
  std::string GetLabel() const;
  void SetSelected(bool selected);
  bool IsSelected() const;

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
