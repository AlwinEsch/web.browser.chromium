/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <map>
#include <memory>
#include <msgpack.hpp>
#include <mutex>

namespace kodi
{
namespace sandbox
{
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

class CChildProcessor
{
public:
  CChildProcessor(const std::string& main_shared, bool viaMainThread);
  virtual ~CChildProcessor();

  bool ProcessOutside();

  const std::string GetIdentifier() const { return m_mainShared; }

  static std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> GetCurrentProcessor();

protected:
  bool HandleMessage(uint32_t group, uint32_t func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                                       const char* data,
                                       std::size_t size,
                                       std::size_t offset);

private:
  // TODO: Not make like this where limited! Allow much more for all cases
  static std::shared_ptr<::kodi::sandbox::CShareProcessReceiver> g_mainIn;
  static std::shared_ptr<::kodi::sandbox::CShareProcessReceiver> g_otherIn;
  static std::shared_ptr<::kodi::sandbox::CShareProcessReceiver> g_otherIn2;
  static std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> g_mainOut;
  static std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> g_otherOut;
  static std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> g_otherOut2;
  static std::mutex g_lock;

  const std::string m_mainShared;

  bool m_active{true};
};

} /* namespace sandbox */
} /* namespace kodi */
