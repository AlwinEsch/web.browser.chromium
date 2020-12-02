/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "Launcher.h"

namespace kodi
{
namespace sandbox
{
namespace parent
{

class CChildLauncherPosix : public CChildLauncher
{
public:
  CChildLauncherPosix();
  ~CChildLauncherPosix();

  bool Launch(const std::vector<std::string>& argv, bool waitForExit) override;
  ChildStatus ProcessActive() override;
  bool Kill(bool wait) override;

private:
  void ProcessStatus(int status);

  pid_t m_pid = -1;
};

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
