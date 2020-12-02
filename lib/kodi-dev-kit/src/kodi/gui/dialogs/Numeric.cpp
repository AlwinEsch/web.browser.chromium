/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/Numeric.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/Numeric.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace Numeric
{

bool ShowAndVerifyNewPassword(std::string& newPassword)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h,
                                  kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword(newPassword));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword t = ident.get().as<decltype(t)>();
  newPassword = std::get<1>(t);
  return std::get<0>(t);
}

int ShowAndVerifyPassword(const std::string& password, const std::string& heading, int retries)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h,
                                  kodi_gui_dialogs_Numeric_ShowAndVerifyPassword));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyPassword(password, heading, retries));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyPassword t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool ShowAndVerifyInput(std::string& toVerify, const std::string& heading, bool verifyInput)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h,
                                  kodi_gui_dialogs_Numeric_ShowAndVerifyInput));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndVerifyInput(toVerify, heading,
                                                                              verifyInput));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndVerifyInput t = ident.get().as<decltype(t)>();
  toVerify = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetTime(tm& time, const std::string& heading)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h, kodi_gui_dialogs_Numeric_ShowAndGetTime));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetTime(time, heading));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetTime t = ident.get().as<decltype(t)>();
  time = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetDate(tm& date, const std::string& heading)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h, kodi_gui_dialogs_Numeric_ShowAndGetDate));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetDate(date, heading));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetDate t = ident.get().as<decltype(t)>();
  date = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetIPAddress(std::string& ipAddress, const std::string& heading)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h,
                                  kodi_gui_dialogs_Numeric_ShowAndGetIPAddress));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetIPAddress(ipAddress, heading));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetIPAddress t = ident.get().as<decltype(t)>();
  ipAddress = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetNumber(std::string& input,
                      const std::string& heading,
                      unsigned int autoCloseTimeoutMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h,
                                  kodi_gui_dialogs_Numeric_ShowAndGetNumber));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetNumber(input, heading,
                                                                            autoCloseTimeoutMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetNumber t = ident.get().as<decltype(t)>();
  input = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetSeconds(std::string& time, const std::string& heading)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Numeric_h,
                                  kodi_gui_dialogs_Numeric_ShowAndGetSeconds));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Numeric_ShowAndGetSeconds(time, heading));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Numeric_ShowAndGetSeconds t = ident.get().as<decltype(t)>();
  time = std::get<1>(t);
  return std::get<0>(t);
}

} /* namespace Numeric */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
