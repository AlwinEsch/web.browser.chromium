/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "OK.h"

#include "../../../../sandbox/kodi/gui/dialogs/OK.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/OK.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_OK_h::HandleMessage(int funcGroup,
                                       int func,
                                       const msgpack::unpacked& in,
                                       msgpack::sbuffer& out,
                                       const char* data,
                                       std::size_t size,
                                       std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_OK_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_OK_ShowAndGetInput:
    {
      msgParent__IN_kodi_gui_dialogs_OK_ShowAndGetInput t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::OK::ShowAndGetInput(std::get<0>(t), std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_OK_ShowAndGetInput2:
    {
      msgParent__IN_kodi_gui_dialogs_OK_ShowAndGetInput2 t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::OK::ShowAndGetInput(std::get<0>(t), std::get<1>(t), std::get<2>(t),
                                              std::get<3>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_OK_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
