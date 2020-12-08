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

class CLabel
{
public:
  CLabel(CWindow* window, int controlId);
  ~CLabel();

  void SetVisible(bool visible);
  void SetLabel(const std::string& label);
  std::string GetLabel() const;

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
