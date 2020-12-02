/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../IMsgHdl.h"

namespace kodi
{
namespace sandbox
{
namespace parent
{

class C_gui_dialogs_ExtendedProgress_h : public IMsgHdl
{
public:
  C_gui_dialogs_ExtendedProgress_h() = default;

  bool HandleMessage(int funcGroup,
                     int func,
                     const msgpack::unpacked& in,
                     msgpack::sbuffer& out,
                     const char* data,
                     std::size_t size,
                     std::size_t offset) override;
};

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
