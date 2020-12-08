/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "General.h"

#include "../../../sandbox/kodi/General.h"
#include "../../../sandbox/kodi/gui/General.h"
#include "../../utils/Log.h"

#include <kodi/gui/General.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_General_h::HandleMessage(int funcGroup,
                                    int func,
                                    const msgpack::unpacked& in,
                                    msgpack::sbuffer& out,
                                    const char* data,
                                    std::size_t size,
                                    std::size_t offset)
{
  if (funcGroup != funcGroup_gui_General_h)
    return false;

  switch (func)
  {
    case funcParent_gui_General_h::kodi_gui_Lock:
    {
      kodi::gui::Lock();
      break;
    }
    case funcParent_gui_General_h::kodi_gui_Unlock:
    {
      kodi::gui::Unlock();
      break;
    }
    case funcParent_gui_General_h::kodi_gui_GetScreenHeight:
    {
      msgpack::pack(out, msgParent_OUT_kodi_gui_GetHWContext(kodi::gui::GetScreenHeight()));
      break;
    }
    case funcParent_gui_General_h::kodi_gui_GetScreenWidth:
    {
      msgpack::pack(out, msgParent_OUT_kodi_gui_GetHWContext(kodi::gui::GetScreenWidth()));
      break;
    }
    case funcParent_gui_General_h::kodi_gui_GetVideoResolution:
    {
      msgpack::pack(out, msgParent_OUT_kodi_gui_GetHWContext(kodi::gui::GetVideoResolution()));
      break;
    }
    case funcParent_gui_General_h::kodi_gui_GetCurrentWindowDialogId:
    {
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_GetHWContext(kodi::gui::GetCurrentWindowDialogId()));
      break;
    }
    case funcParent_gui_General_h::kodi_gui_GetCurrentWindowId:
    {
      msgpack::pack(out, msgParent_OUT_kodi_gui_GetHWContext(kodi::gui::GetCurrentWindowId()));
      break;
    }
    case funcParent_gui_General_h::kodi_gui_GetHWContext:
    {
      msgpack::pack(out, msgParent_OUT_kodi_gui_GetHWContext(uint64_t(kodi::gui::GetHWContext())));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "C_gui_General_h::%s: addon called with unknown function id '%i' on group 'kodi'",
          __func__, func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */
