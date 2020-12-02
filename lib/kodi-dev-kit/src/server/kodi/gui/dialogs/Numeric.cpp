/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Numeric.h"

#include "../../../../sandbox/kodi/gui/dialogs/Numeric.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/Numeric.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_Numeric_h::HandleMessage(int funcGroup,
                                            int func,
                                            const msgpack::unpacked& in,
                                            msgpack::sbuffer& out,
                                            const char* data,
                                            std::size_t size,
                                            std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_Numeric_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword t =
          in.get().as<decltype(t)>();
      std::string& password = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Numeric::ShowAndVerifyNewPassword(password);
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword(ret, password));
      break;
    }
    case kodi_gui_dialogs_Numeric_ShowAndVerifyPassword:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyPassword t = in.get().as<decltype(t)>();
      bool ret = kodi::gui::dialogs::Numeric::ShowAndVerifyPassword(std::get<0>(t), std::get<1>(t),
                                                                    std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyPassword(ret));
      break;
    }
    case kodi_gui_dialogs_Numeric_ShowAndVerifyInput:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyInput t = in.get().as<decltype(t)>();
      std::string& toVerify = std::get<0>(t);
      bool ret =
          kodi::gui::dialogs::Numeric::ShowAndVerifyInput(toVerify, std::get<1>(t), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyInput(ret, toVerify));
      break;
    }
    case kodi_gui_dialogs_Numeric_ShowAndGetTime:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetTime t = in.get().as<decltype(t)>();
      struct tm time = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Numeric::ShowAndGetTime(time, std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetTime(ret, time));
      break;
    }
    case kodi_gui_dialogs_Numeric_ShowAndGetDate:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetDate t = in.get().as<decltype(t)>();
      struct tm date = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Numeric::ShowAndGetDate(date, std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetDate(ret, date));
      break;
    }
    case kodi_gui_dialogs_Numeric_ShowAndGetIPAddress:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetIPAddress t = in.get().as<decltype(t)>();
      std::string& toVerify = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Numeric::ShowAndGetIPAddress(toVerify, std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetIPAddress(ret, toVerify));
      break;
    }
    case kodi_gui_dialogs_Numeric_ShowAndGetNumber:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetNumber t = in.get().as<decltype(t)>();
      std::string& toVerify = std::get<0>(t);
      bool ret =
          kodi::gui::dialogs::Numeric::ShowAndGetNumber(toVerify, std::get<1>(t), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetNumber(ret, toVerify));
      break;
    }
    case kodi_gui_dialogs_Numeric_ShowAndGetSeconds:
    {
      msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetSeconds t = in.get().as<decltype(t)>();
      std::string& toVerify = std::get<0>(t);
      bool ret = kodi::gui::dialogs::Numeric::ShowAndGetSeconds(toVerify, std::get<1>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetSeconds(ret, toVerify));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_Numeric_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
