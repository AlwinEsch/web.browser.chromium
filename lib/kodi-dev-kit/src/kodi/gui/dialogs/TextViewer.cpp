/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/TextViewer.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/TextViewer.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace TextViewer
{

void Show(const std::string& heading, const std::string& text)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_TextViewer_h, kodi_gui_dialogs_TextViewer_Show));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_TextViewer_Show(heading, text));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace TextViewer */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
