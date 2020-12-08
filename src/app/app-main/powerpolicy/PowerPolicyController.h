/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_power_policy_controller.h"

// Global
#include <atomic>
#include <map>
#include <memory>

namespace chromium
{
namespace app
{
namespace main
{

class CMainCEFProcess;

namespace powerpolicy
{

class CPowerPolicyController : public CefPowerPolicyController
{
public:
  CPowerPolicyController(std::shared_ptr<CMainCEFProcess> addonMain);
  ~CPowerPolicyController();

  /// CefPowerPolicyController
  //@{
  int AddSystemWakeLock(WakeLockReason reason, const CefString& description) override;
  int AddScreenWakeLock(WakeLockReason reason, const CefString& description) override;
  int AddDimWakeLock(WakeLockReason reason, const CefString& description) override;
  void RemoveWakeLock(int block_id) override;
  //@}

private:
  IMPLEMENT_REFCOUNTING(CPowerPolicyController);
  DISALLOW_COPY_AND_ASSIGN(CPowerPolicyController);

  enum class WakeType
  {
    System,
    Screen,
    Dim
  };

  bool ContainsType(WakeType reason);

  std::shared_ptr<CMainCEFProcess> m_addonMain;
  std::map<int, std::pair<WakeType, WakeLockReason>> m_locks;
  std::atomic_int m_nextLockId{1};
};

} /* namespace powerpolicy */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
