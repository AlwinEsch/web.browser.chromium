/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Action.h"

#include "../../../../sandbox/kodi/gui/input/Action.h"

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_input_Action_h::HandleMessage(int funcGroup,
                    int func,
                    const msgpack::unpacked& in,
                    msgpack::sbuffer& out,
                    const char* data,
                    std::size_t size,
                    std::size_t offset)
{
  return false;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
