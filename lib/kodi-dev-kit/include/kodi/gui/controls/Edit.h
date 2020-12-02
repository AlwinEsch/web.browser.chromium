/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "../../c-api/gui/controls/edit.h"

#include <string>

namespace kodi
{
namespace gui
{

class CWindow;

namespace controls
{

class CEdit
{
public:
  CEdit(CWindow* window, int controlId);
  ~CEdit();

  void SetVisible(bool visible);
  void SetEnabled(bool enabled);
  void SetLabel(const std::string& label);
  std::string GetLabel() const;
  void SetText(const std::string& label);
  std::string GetText() const;
  void SetCursorPosition(unsigned int position);
  unsigned int GetCursorPosition();
  void SetInputType(AddonGUIInputType type, const std::string& heading);

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
