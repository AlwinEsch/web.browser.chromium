/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "IMsgHdl.h"
#include "../sandbox/ShareProcessTransmitter.h"
#include "../sandbox/ShareProcessReceiver.h"
#include "../sandbox/shared_process_func_group.h"

#include <array>
#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace kodi
{
namespace sandbox
{
namespace parent
{

enum class ChildStatus
{
  Running,
  NotStarted,
  ExitedNormal,
  StoppedBySignal,
  StoppedByUnknown,
  KilledFromOutside,
  SeqmentionFault,
};

class CChildLauncher : public IMsgHdl
{
public:
  CChildLauncher();
  ~CChildLauncher();

  const std::string& GetIdentifier() const { return m_identifier; }
//   const std::shared_ptr<CShareProcessTransmitter> GetMainTransmitter() { return m_mainTransmitter; }
  const std::shared_ptr<CShareProcessTransmitter> GetCurrentTransmitter();

  virtual bool Launch(const std::vector<std::string>& argv, bool waitForExit) = 0;
  virtual ChildStatus ProcessActive() = 0;
  virtual bool Kill(bool wait = true) = 0;

  ChildStatus GetLastChildStatus() const { return m_lastStatus;  }
  int GetLExitCode() const { return m_exitCode;  }

  IMsgHdl* GetHdl(funcGroup group);

protected:
  bool StartObserver();
  bool StopObserver();

  const std::string m_identifier;

  std::thread::id mainThreadId;

  std::shared_ptr<kodi::sandbox::CShareProcessTransmitter> m_mainThreadTransmit;
  std::shared_ptr<kodi::sandbox::CShareProcessReceiver> m_mainThreadReceive;

  std::vector<std::shared_ptr<kodi::sandbox::CShareProcessTransmitter>> m_childThreadTransmit;
  std::vector<std::shared_ptr<kodi::sandbox::CShareProcessReceiver>> m_childThreadReceive;
  uint32_t m_nextChildIdentifier{0};

  std::atomic<ChildStatus> m_lastStatus{ChildStatus::NotStarted};
  int m_exitCode = 0;

private:
  void ObserveProcess();

  bool HandleMainMessage(int group, int func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                         const char* data, std::size_t size, std::size_t offset);
  bool HandleMessage(int group, int func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                     const char* data, std::size_t size, std::size_t offset) override;

  std::atomic_bool m_running{false};
  std::thread m_thread;
  std::array<IMsgHdl*, funcGroup_Max> m_handlers;
};

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
