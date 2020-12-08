/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Keyboard.h"

#include "../../../../sandbox/kodi/gui/dialogs/Keyboard.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/Keyboard.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_Keyboard_h::HandleMessage(int funcGroup,
                                             int func,
                                             const msgpack::unpacked& in,
                                             msgpack::sbuffer& out,
                                             const char* data,
                                             std::size_t size,
                                             std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_Keyboard_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_Keyboard_ShowAndGetInput:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetInput t = in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Keyboard::ShowAndGetInput(text, std::get<1>(t), std::get<2>(t),
                                                               std::get<3>(t), std::get<4>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetInput(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_ShowAndGetInput2:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetInput2 t = in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      bool ret =
          kodi::gui::dialogs::Keyboard::ShowAndGetInput(text, std::get<1>(t), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetInput2(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword t = in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Keyboard::ShowAndGetNewPassword(
          text, std::get<1>(t), std::get<2>(t), std::get<3>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2 t = in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Keyboard::ShowAndGetNewPassword(text, std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword t =
          in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Keyboard::ShowAndVerifyNewPassword(
          text, std::get<1>(t), std::get<2>(t), std::get<3>(t));
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2 t =
          in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Keyboard::ShowAndVerifyNewPassword(text, std::get<1>(t));
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword t = in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      int ret = kodi::gui::dialogs::Keyboard::ShowAndVerifyPassword(text, std::get<1>(t),
                                                                    std::get<2>(t), std::get<3>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_ShowAndGetFilter:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetFilter t = in.get().as<decltype(t)>();
      std::string& text = std::get<0>(t);
      bool ret =
          kodi::gui::dialogs::Keyboard::ShowAndGetFilter(text, std::get<1>(t), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetFilter(ret, text));
      break;
    }
    case kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard:
    {
      msgParent__IN_kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard t =
          in.get().as<decltype(t)>();
      bool ret =
          kodi::gui::dialogs::Keyboard::SendTextToActiveKeyboard(std::get<0>(t), std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard(ret));
      break;
    }
    case kodi_gui_dialogs_Keyboard_IsKeyboardActivated:
    {
      bool ret = kodi::gui::dialogs::Keyboard::IsKeyboardActivated();
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Keyboard_IsKeyboardActivated(ret));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_Keyboard_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
