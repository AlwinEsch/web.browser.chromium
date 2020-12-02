/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "shared_data_structure.h"
#include "shared_process_func_group.h"

#include <atomic>
#include <memory>
#include <msgpack.hpp>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace kodi
{
namespace sandbox
{

class CSharedMemControl;
class CShareProcessReceiver;

class CShareProcessTransmitter
{
public:
  CShareProcessTransmitter(const std::string& identifier, bool child, bool mainThread, size_t size = SHARED_DEFAULT_MEM_SIZE);
  ~CShareProcessTransmitter() = default;

  const std::string& GetIdentifier() const { return m_identifier; }
  bool IsActive() const { return m_active; };

  bool Create(bool initial);
  void Destroy();

  void SendMessage(const msgpack::sbuffer& in);
  void SendMessage(const msgpack::sbuffer& in, msgpack::sbuffer& ret);
  bool ProcessMainThreadReceive(apiShareData* sharedMem, CShareProcessReceiver* receiver);

private:
  const std::string m_identifier;
  const bool m_child;
  const bool m_mainThread;
  std::atomic_bool m_active{false};
  std::mutex m_lock;
  std::shared_ptr<CSharedMemControl> m_memControl;

  std::atomic_bool m_mainThreadReceive{false};
  apiShareData* m_mainReceiveSharedMem{nullptr};
  CShareProcessReceiver* m_mainReceiver{nullptr};
};

} /* namespace sandbox */
} /* namespace kodi */
