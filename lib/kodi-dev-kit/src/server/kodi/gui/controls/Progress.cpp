/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Progress.h"

#include "../../../../sandbox/kodi/gui/controls/Progress.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/Progress.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_Progress_h::HandleMessage(int funcGroup,
                                              int func,
                                              const msgpack::unpacked& in,
                                              msgpack::sbuffer& out,
                                              const char* data,
                                              std::size_t size,
                                              std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_Progress_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CProgress_create:
    {
      msgParent__IN_kodi_gui_controls_CProgress_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CProgress* control = new kodi::gui::controls::CProgress(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CProgress_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CProgress_destroy:
    {
      msgParent__IN_kodi_gui_controls_CProgress_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CProgress* control =
          reinterpret_cast<kodi::gui::controls::CProgress*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CProgress_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CProgress_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CProgress* control =
          reinterpret_cast<kodi::gui::controls::CProgress*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CProgress_SetPercentage:
    {
      msgParent__IN_kodi_gui_controls_CProgress_SetPercentage t = in.get().as<decltype(t)>();
      kodi::gui::controls::CProgress* control =
          reinterpret_cast<kodi::gui::controls::CProgress*>(std::get<0>(t));
      if (!control)
        break;

      control->SetPercentage(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CProgress_GetPercentage:
    {
      msgParent__IN_kodi_gui_controls_CProgress_GetPercentage t = in.get().as<decltype(t)>();
      kodi::gui::controls::CProgress* control =
          reinterpret_cast<kodi::gui::controls::CProgress*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(
          out, msgParent_OUT_kodi_gui_controls_CProgress_GetPercentage(control->GetPercentage()));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_Progress_h::%s: addon called with unknown "
                               "function id '%i' on group 'C_gui_controls_Progress_h'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
