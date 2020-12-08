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

class CSlider
{
public:
  CSlider(CWindow* window, int controlId);
  ~CSlider();

  void SetVisible(bool visible);
  void SetEnabled(bool enabled);
  void Reset();
  std::string GetDescription() const;
  void SetIntRange(int start, int end);
  void SetIntValue(int value);
  int GetIntValue() const;
  void SetIntInterval(float interval);
  void SetPercentage(float percent);
  float GetPercentage() const;
  void SetFloatRange(float start, float end);
  void SetFloatValue(float value);
  float GetFloatValue() const;
  void SetFloatInterval(float interval);

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
