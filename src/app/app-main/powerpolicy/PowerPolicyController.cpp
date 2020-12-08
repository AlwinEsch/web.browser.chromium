/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PowerPolicyController.h"

// Own
#include "../MainCEFProcess.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"

/*
TODO:
Seems mostly final only to observe it works correct and not override by Kodi
and e.g. screensaver starts again on video playback by website.
*/

namespace chromium
{
namespace app
{
namespace main
{
namespace powerpolicy
{

namespace
{
static std::atomic_int m_ctorcount{0}; // For debug purposes and to see destructs done
}

CPowerPolicyController::CPowerPolicyController(std::shared_ptr<CMainCEFProcess> addonMain)
  : m_addonMain(addonMain)
{
  fprintf(stderr, "CPowerPolicyController START (%p) count open %i\n", this, ++m_ctorcount);
}

CPowerPolicyController::~CPowerPolicyController()
{
  fprintf(stderr, "CPowerPolicyController¸ STOP (%p) count open %i\n", this, --m_ctorcount);
}

/*
 * CefPowerPolicyController
 *
 * To inihibit screensavers and system sleep in case of e.g. playback via
 * browser.
 */
//@{

int CPowerPolicyController::AddSystemWakeLock(WakeLockReason reason, const CefString& description)
{
  const int block_id = m_nextLockId++;
  m_locks.emplace(block_id, std::make_pair(WakeType::System, reason));
  m_addonMain->InhibitShutdown(true);
  kodi::Log(ADDON_LOG_DEBUG, "CPowerPolicyController::%s: System sleep inhibit started, why: '%s'",
            __func__, description.ToString().c_str());
  return block_id;
}

int CPowerPolicyController::AddScreenWakeLock(WakeLockReason reason, const CefString& description)
{
  const int block_id = m_nextLockId++;
  m_locks.emplace(block_id, std::make_pair(WakeType::Screen, reason));
  m_addonMain->InhibitScreensaver(true);
  kodi::Log(ADDON_LOG_DEBUG,
            "CPowerPolicyController::%s: System screensaver inhibit started, why: '%s'", __func__,
            description.ToString().c_str());
  return block_id;
}

int CPowerPolicyController::AddDimWakeLock(WakeLockReason reason, const CefString& description)
{
  const int block_id = m_nextLockId++;
  m_locks.emplace(block_id, std::make_pair(WakeType::Dim, reason));
  kodi::Log(
      ADDON_LOG_DEBUG,
      "CPowerPolicyController::%s: System dim inhibit started, why: '%s' (currently unsupported "
      "in Kodi)",
      __func__, description.ToString().c_str());
  return block_id;
}

void CPowerPolicyController::RemoveWakeLock(int block_id)
{
  auto entry =
      std::find_if(m_locks.begin(), m_locks.end(),
                   [block_id](const std::pair<int, std::pair<WakeType, WakeLockReason>> entry) {
                     return entry.first == block_id;
                   });
  if (entry != m_locks.end())
  {
    const WakeType type = entry->second.first;
    m_locks.erase(entry);

    if (!ContainsType(type))
    {
      if (entry->second.first == WakeType::System)
      {
        m_addonMain->InhibitShutdown(false);
        kodi::Log(ADDON_LOG_DEBUG, "CPowerPolicyController::%s: System sleep inhibit ended",
                  __func__);
      }
      else if (entry->second.first == WakeType::Screen)
      {
        m_addonMain->InhibitScreensaver(false);
        kodi::Log(ADDON_LOG_DEBUG, "CPowerPolicyController::%s: System screensaver inhibit ended",
                  __func__);
      }
      else if (entry->second.first == WakeType::Dim)
      {
        kodi::Log(ADDON_LOG_DEBUG, "CPowerPolicyController::%s: System dim inhibit ended",
                  __func__);
      }
    }
  }
}

//@}

bool CPowerPolicyController::ContainsType(WakeType reason)
{
  auto entry =
      std::find_if(m_locks.begin(), m_locks.end(),
                   [reason](const std::pair<int, std::pair<WakeType, WakeLockReason>> entry) {
                     return entry.second.first == reason;
                   });
  return entry != m_locks.end();
}

} /* namespace powerpolicy */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
