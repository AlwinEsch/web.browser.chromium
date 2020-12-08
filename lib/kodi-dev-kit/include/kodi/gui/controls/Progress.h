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

class CProgress
{
public:
  CProgress(CWindow* window, int controlId);
  ~CProgress();

  void SetVisible(bool visible);
  void SetEnabled(bool enabled);
  void SetPercentage(float percent);
  float GetPercentage() const;

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
