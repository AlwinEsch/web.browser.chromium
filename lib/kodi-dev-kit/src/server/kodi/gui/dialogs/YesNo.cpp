/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "YesNo.h"

#include "../../../../sandbox/kodi/gui/dialogs/YesNo.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/YesNo.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_YesNo_h::HandleMessage(int funcGroup,
                                          int func,
                                          const msgpack::unpacked& in,
                                          msgpack::sbuffer& out,
                                          const char* data,
                                          std::size_t size,
                                          std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_YesNo_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_YesNo_ShowAndGetInput:
    {
      msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput t = in.get().as<decltype(t)>();
      bool select = std::get<2>(t);
      bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(std::get<0>(t), std::get<1>(t), select,
                                                            std::get<3>(t), std::get<4>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput(ret, select));
      break;
    }
    case kodi_gui_dialogs_YesNo_ShowAndGetInput2:
    {
      msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput2 t = in.get().as<decltype(t)>();
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput2(
                             kodi::gui::dialogs::YesNo::ShowAndGetInput(
                                 std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t),
                                 std::get<4>(t), std::get<5>(t))));
      break;
    }
    case kodi_gui_dialogs_YesNo_ShowAndGetInput3:
    {
      msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput3 t = in.get().as<decltype(t)>();
      bool select = std::get<4>(t);
      bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(std::get<0>(t), std::get<1>(t),
                                                            std::get<2>(t), std::get<3>(t), select,
                                                            std::get<5>(t), std::get<6>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput3(ret, select));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_YesNo_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
