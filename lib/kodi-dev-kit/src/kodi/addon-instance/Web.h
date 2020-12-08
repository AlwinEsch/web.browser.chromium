/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../sandbox/IMsgHdl.h"

namespace kodi
{
namespace sandbox
{

class CChildProcessor;

namespace child
{

class C_addoninstance_Web_h : public IMsgHdl
{
public:
  C_addoninstance_Web_h(CChildProcessor& childProcessor) : m_childProcessor(childProcessor) {}

  bool HandleMessage(int funcGroup,
                     int func,
                     const msgpack::unpacked& in,
                     msgpack::sbuffer& out,
                     const char* data,
                     std::size_t size,
                     std::size_t offset) override;

private:
  CChildProcessor& m_childProcessor;
};

} /* namespace child */
} /* namespace sandbox */
} /* namespace kodi */
