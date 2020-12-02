/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "TextViewer.h"

#include "../../../../sandbox/kodi/gui/dialogs/TextViewer.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/TextViewer.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_TextViewer_h::HandleMessage(int funcGroup,
                                               int func,
                                               const msgpack::unpacked& in,
                                               msgpack::sbuffer& out,
                                               const char* data,
                                               std::size_t size,
                                               std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_TextViewer_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_TextViewer_Show:
    {
      msgParent__IN_kodi_gui_dialogs_TextViewer_Show t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::TextViewer::Show(std::get<0>(t), std::get<1>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_TextViewer_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
