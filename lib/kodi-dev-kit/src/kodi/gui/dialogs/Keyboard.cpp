/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/Keyboard.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/Keyboard.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace Keyboard
{

bool ShowAndGetInput(std::string& text,
                     const std::string& heading,
                     bool allowEmptyResult,
                     bool hiddenInput,
                     unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndGetInput));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetInput(
                        text, heading, allowEmptyResult, hiddenInput, autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetInput t = ident.get().as<decltype(t)>();
  text = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetInput(std::string& text, bool allowEmptyResult, unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndGetInput2));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetInput2(text, allowEmptyResult,
                                                                             autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetInput2 t = ident.get().as<decltype(t)>();
  text = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetNewPassword(std::string& newPassword,
                           const std::string& heading,
                           bool allowEmptyResult,
                           unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword(
                        newPassword, heading, allowEmptyResult, autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword t = ident.get().as<decltype(t)>();
  newPassword = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetNewPassword(std::string& newPassword, unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2(newPassword, autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2 t = ident.get().as<decltype(t)>();
  newPassword = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndVerifyNewPassword(std::string& newPassword,
                              const std::string& heading,
                              bool allowEmptyResult,
                              unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword(
                        newPassword, heading, allowEmptyResult, autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword t =
      ident.get().as<decltype(t)>();
  newPassword = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndVerifyNewPassword(std::string& newPassword, unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2(newPassword,
                                                                                      autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword t =
      ident.get().as<decltype(t)>();
  newPassword = std::get<1>(t);
  return std::get<0>(t);
}

int ShowAndVerifyPassword(std::string& password,
                          const std::string& heading,
                          int retries,
                          unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword(
                        password, heading, retries, autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword t = ident.get().as<decltype(t)>();
  password = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetFilter(std::string& text, bool searching, unsigned int autoCloseMs)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_ShowAndGetFilter));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_dialogs_Keyboard_ShowAndGetFilter(text, searching, autoCloseMs));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_ShowAndGetFilter t = ident.get().as<decltype(t)>();
  text = std::get<1>(t);
  return std::get<0>(t);
}

bool SendTextToActiveKeyboard(const std::string& text, bool closeKeyboard)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard(text, closeKeyboard));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard t =
      ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool IsKeyboardActivated()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Keyboard_h,
                                  kodi_gui_dialogs_Keyboard_IsKeyboardActivated));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_Keyboard_IsKeyboardActivated(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_Keyboard_IsKeyboardActivated t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace Keyboard */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
