/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "FadeLabel.h"

#include "../../../../sandbox/kodi/gui/controls/FadeLabel.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/FadeLabel.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_FadeLabel_h::HandleMessage(int funcGroup,
                                               int func,
                                               const msgpack::unpacked& in,
                                               msgpack::sbuffer& out,
                                               const char* data,
                                               std::size_t size,
                                               std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_FadeLabel_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CFadeLabel_create:
    {
      msgParent__IN_kodi_gui_controls_CFadeLabel_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CFadeLabel* control = new kodi::gui::controls::CFadeLabel(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CFadeLabel_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CFadeLabel_destroy:
    {
      msgParent__IN_kodi_gui_controls_CFadeLabel_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CFadeLabel* control =
          reinterpret_cast<kodi::gui::controls::CFadeLabel*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CFadeLabel_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CFadeLabel_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CFadeLabel* control =
          reinterpret_cast<kodi::gui::controls::CFadeLabel*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CFadeLabel_GetLabel:
    {
      msgParent__IN_kodi_gui_controls_CFadeLabel_GetLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CFadeLabel* control =
          reinterpret_cast<kodi::gui::controls::CFadeLabel*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CFadeLabel_GetLabel(control->GetLabel()));
      break;
    }
    case kodi_gui_controls_CFadeLabel_SetScrolling:
    {
      msgParent__IN_kodi_gui_controls_CFadeLabel_SetScrolling t = in.get().as<decltype(t)>();
      kodi::gui::controls::CFadeLabel* control =
          reinterpret_cast<kodi::gui::controls::CFadeLabel*>(std::get<0>(t));
      if (!control)
        break;

      control->SetScrolling(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CFadeLabel_Reset:
    {
      msgParent__IN_kodi_gui_controls_CFadeLabel_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CFadeLabel* control =
          reinterpret_cast<kodi::gui::controls::CFadeLabel*>(std::get<0>(t));
      if (!control)
        break;

      control->Reset();
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_FadeLabel_h::%s: addon called with unknown "
                               "function id '%i' on group 'C_gui_controls_FadeLabel_h'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
