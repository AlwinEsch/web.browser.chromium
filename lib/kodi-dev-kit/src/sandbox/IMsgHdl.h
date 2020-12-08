/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "shared_process_func_group.h"

#include <msgpack.hpp>

namespace kodi
{
namespace sandbox
{

class IMsgHdl
{
public:
  IMsgHdl() = default;
  virtual ~IMsgHdl() = default;

  virtual bool HandleMessage(int funcGroup, int func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                             const char* data, std::size_t size, std::size_t offset) = 0;
};

} /* namespace sandbox */
} /* namespace kodi */
