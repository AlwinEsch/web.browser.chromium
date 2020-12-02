/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "RadioButton.h"

#include "../../../../sandbox/kodi/gui/controls/RadioButton.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/RadioButton.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_RadioButton_h::HandleMessage(int funcGroup,
                                                 int func,
                                                 const msgpack::unpacked& in,
                                                 msgpack::sbuffer& out,
                                                 const char* data,
                                                 std::size_t size,
                                                 std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_RadioButton_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CRadioButton_create:
    {
      msgParent__IN_kodi_gui_controls_CRadioButton_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CRadioButton* control = new kodi::gui::controls::CRadioButton(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CRadioButton_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CRadioButton_destroy:
    {
      msgParent__IN_kodi_gui_controls_CRadioButton_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CRadioButton* control =
          reinterpret_cast<kodi::gui::controls::CRadioButton*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CRadioButton_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CRadioButton_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CRadioButton* control =
          reinterpret_cast<kodi::gui::controls::CRadioButton*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CRadioButton_SetLabel:
    {
      msgParent__IN_kodi_gui_controls_CRadioButton_SetLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CRadioButton* control =
          reinterpret_cast<kodi::gui::controls::CRadioButton*>(std::get<0>(t));
      if (!control)
        break;

      control->SetLabel(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CRadioButton_GetLabel:
    {
      msgParent__IN_kodi_gui_controls_CRadioButton_GetLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CRadioButton* control =
          reinterpret_cast<kodi::gui::controls::CRadioButton*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_controls_CRadioButton_GetLabel(control->GetLabel()));
      break;
    }
    case kodi_gui_controls_CRadioButton_SetSelected:
    {
      msgParent__IN_kodi_gui_controls_CRadioButton_SetSelected t = in.get().as<decltype(t)>();
      kodi::gui::controls::CRadioButton* control =
          reinterpret_cast<kodi::gui::controls::CRadioButton*>(std::get<0>(t));
      if (!control)
        break;

      control->SetSelected(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CRadioButton_IsSelected:
    {
      msgParent__IN_kodi_gui_controls_CRadioButton_IsSelected t = in.get().as<decltype(t)>();
      kodi::gui::controls::CRadioButton* control =
          reinterpret_cast<kodi::gui::controls::CRadioButton*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_controls_CRadioButton_IsSelected(control->IsSelected()));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_RadioButton_h::%s: addon called with unknown "
                               "function id '%i' on group 'C_gui_controls_RadioButton_h'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
