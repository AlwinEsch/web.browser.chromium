/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/OK.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/OK.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace OK
{

void ShowAndGetInput(const std::string& heading, const std::string& text)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_OK_h, kodi_gui_dialogs_OK_ShowAndGetInput));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_OK_ShowAndGetInput(heading, text));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void ShowAndGetInput(const std::string& heading,
                     const std::string& line0,
                     const std::string& line1,
                     const std::string& line2)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_OK_h, kodi_gui_dialogs_OK_ShowAndGetInput));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_dialogs_OK_ShowAndGetInput2(heading, line0, line1, line2));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace OK */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
