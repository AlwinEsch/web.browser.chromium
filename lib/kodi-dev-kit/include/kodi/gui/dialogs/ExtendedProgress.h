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
namespace dialogs
{

class CExtendedProgress
{
public:
  explicit CExtendedProgress(const std::string& title = "");
  ~CExtendedProgress();

  std::string Title() const;
  void SetTitle(const std::string& title);
  std::string Text() const;
  void SetText(const std::string& text);
  bool IsFinished() const;
  void MarkFinished();
  float Percentage() const;
  void SetPercentage(float percentage);
  void SetProgress(int currentItem, int itemCount);

private:
  uint64_t m_handle;
};

} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
