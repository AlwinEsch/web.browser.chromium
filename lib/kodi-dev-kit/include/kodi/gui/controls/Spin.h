/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "../../c-api/gui/controls/spin.h"

#include <string>

namespace kodi
{
namespace gui
{

class CWindow;

namespace controls
{

class CSpin
{
public:
  CSpin(CWindow* window, int controlId);
  ~CSpin();

  void SetVisible(bool visible);
  void SetEnabled(bool enabled);
  void SetText(const std::string& label);
  void Reset();
  void SetType(AddonGUISpinControlType type);
  void AddLabel(const std::string& label, const std::string& value);
  void AddLabel(const std::string& label, int value);
  void SetStringValue(const std::string& value);
  std::string GetStringValue() const;
  void SetIntRange(int start, int end);
  void SetIntValue(int value);
  int GetIntValue() const;
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
