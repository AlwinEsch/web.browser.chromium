/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../sandbox/kodi/gui/dialogs/YesNo.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/c-api/gui/dialogs/yes_no.h"
#include "../../../sandbox/ShareProcessTransmitter.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace YesNo
{

bool ShowAndGetInput(const std::string& heading,
                     const std::string& text,
                     bool& canceled,
                     const std::string& noLabel,
                     const std::string& yesLabel)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_YesNo_h, kodi_gui_dialogs_YesNo_ShowAndGetInput));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput(heading, text, canceled,
                                                                         noLabel, yesLabel));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput t = ident.get().as<decltype(t)>();
  canceled = std::get<1>(t);
  return std::get<0>(t);
}

bool ShowAndGetInput(const std::string& heading,
                     const std::string& line0,
                     const std::string& line1,
                     const std::string& line2,
                     const std::string& noLabel,
                     const std::string& yesLabel)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_YesNo_h, kodi_gui_dialogs_YesNo_ShowAndGetInput2));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput2(
                        heading, line0, line1, line2, noLabel, yesLabel));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput2 t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}


bool ShowAndGetInput(const std::string& heading,
                     const std::string& line0,
                     const std::string& line1,
                     const std::string& line2,
                     bool& canceled,
                     const std::string& noLabel,
                     const std::string& yesLabel)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_YesNo_h, kodi_gui_dialogs_YesNo_ShowAndGetInput3));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_YesNo_ShowAndGetInput3(
                        heading, line0, line1, line2, canceled, noLabel, yesLabel));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_YesNo_ShowAndGetInput3 t = ident.get().as<decltype(t)>();
  canceled = std::get<1>(t);
  return std::get<0>(t);
}

} /* namespace YesNo */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
