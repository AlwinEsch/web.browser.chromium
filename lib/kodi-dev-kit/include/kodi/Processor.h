/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <array>
#include <map>
#include <memory>
#include <msgpack.hpp>
#include <mutex>
#include <thread>

namespace kodi
{
namespace sandbox
{
class IMsgHdl;
class CShareProcessReceiver;
class CShareProcessTransmitter;
} /* namespace sandbox */
} /* namespace kodi */

namespace kodi
{
namespace sandbox
{

std::string CheckSandbox(int argc, char* argv[]);
std::string CheckMainShared(int argc, char* argv[]);
uint64_t CheckBaseHandle(int argc, char* argv[]);

struct InterfaceStatic;

class CChildProcessor
{
public:
  CChildProcessor(const std::string& main_shared, bool viaMainThread, bool noReceive);
  virtual ~CChildProcessor();

  static CChildProcessor& GetActiveProcessor();

  std::string InitSubChild(const std::string& identifier);

  bool ProcessOutside();

  const std::string GetIdentifier() const { return m_mainShared; }

  static std::shared_ptr<kodi::sandbox::CShareProcessTransmitter> GetCurrentProcessor();

protected:
  bool HandleMessage(uint32_t group, uint32_t func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                                       const char* data,
                                       std::size_t size,
                                       std::size_t offset);

private:
  static std::shared_ptr<kodi::sandbox::CShareProcessTransmitter> CreateNewProcessor();

  static InterfaceStatic* g_interface;

  const std::string m_mainShared;

  bool m_active{true};
};

} /* namespace sandbox */
} /* namespace kodi */
