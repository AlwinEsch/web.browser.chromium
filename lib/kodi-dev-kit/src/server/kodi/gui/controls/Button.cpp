/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Button.h"

#include "../../../../sandbox/kodi/gui/controls/Button.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/Button.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_Button_h::HandleMessage(int funcGroup,
                                            int func,
                                            const msgpack::unpacked& in,
                                            msgpack::sbuffer& out,
                                            const char* data,
                                            std::size_t size,
                                            std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_Button_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CButton_create:
    {
      msgParent__IN_kodi_gui_controls_CButton_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control = new kodi::gui::controls::CButton(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CButton_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CButton_destroy:
    {
      msgParent__IN_kodi_gui_controls_CButton_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control =
          reinterpret_cast<kodi::gui::controls::CButton*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CButton_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CButton_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control =
          reinterpret_cast<kodi::gui::controls::CButton*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CButton_SetEnabled:
    {
      msgParent__IN_kodi_gui_controls_CButton_SetEnabled t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control =
          reinterpret_cast<kodi::gui::controls::CButton*>(std::get<0>(t));
      if (!control)
        break;

      control->SetEnabled(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CButton_SetLabel:
    {
      msgParent__IN_kodi_gui_controls_CButton_SetLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control =
          reinterpret_cast<kodi::gui::controls::CButton*>(std::get<0>(t));
      if (!control)
        break;

      control->SetLabel(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CButton_GetLabel:
    {
      msgParent__IN_kodi_gui_controls_CButton_GetLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control =
          reinterpret_cast<kodi::gui::controls::CButton*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CButton_GetLabel(control->GetLabel()));
      break;
    }
    case kodi_gui_controls_CButton_SetLabel2:
    {
      msgParent__IN_kodi_gui_controls_CButton_SetLabel2 t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control =
          reinterpret_cast<kodi::gui::controls::CButton*>(std::get<0>(t));
      if (!control)
        break;

      control->SetLabel2(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CButton_GetLabel2:
    {
      msgParent__IN_kodi_gui_controls_CButton_GetLabel2 t = in.get().as<decltype(t)>();
      kodi::gui::controls::CButton* control =
          reinterpret_cast<kodi::gui::controls::CButton*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CButton_GetLabel2(control->GetLabel2()));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_Button_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
