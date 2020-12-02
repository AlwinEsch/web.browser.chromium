/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "shared_data_structure.h"

#include <atomic>
#include <memory>
#include <msgpack.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_set>

namespace kodi
{
namespace sandbox
{

class CSharedMemControl;
class CShareProcessTransmitter;

class CShareProcessReceiver
{
public:
  CShareProcessReceiver(const std::string& identifier, std::shared_ptr<CShareProcessTransmitter> mainTransmitter, bool child, size_t size = SHARED_DEFAULT_MEM_SIZE);
  ~CShareProcessReceiver() = default;

  void RegisterReceiver(SandboxProcess func);

  bool Create(bool initial, bool useThread);
  void Destroy();

  bool ProcessOutside();
  bool ProcessMemory(apiShareData* sharedMem);

protected:
  void Process();

private:
  bool HandleMessage(int group, int func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                     const char* data, std::size_t size, std::size_t offset);

  const bool m_child;
  std::shared_ptr<CShareProcessTransmitter> m_mainTransmitter;
  std::mutex m_lock;
  std::thread m_thread;
  std::atomic_bool m_running{false};
  std::shared_ptr<CSharedMemControl> m_memControl;
  std::vector<SandboxProcess> m_receivers;
};

} /* namespace sandbox */
} /* namespace kodi */
