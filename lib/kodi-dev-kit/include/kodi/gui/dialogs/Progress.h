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

class CProgress
{
public:
  CProgress();
  ~CProgress();

  void Open();
  void SetHeading(const std::string& heading);
  void SetLine(unsigned int iLine, const std::string& line);
  void SetCanCancel(bool canCancel);
  bool IsCanceled() const;
  void SetPercentage(int percentage);
  int GetPercentage() const;
  void ShowProgressBar(bool onOff);
  void SetProgressMax(int max);
  void SetProgressAdvance(int steps = 1);
  bool Abort();

private:
  uint64_t m_handle;
};

} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
