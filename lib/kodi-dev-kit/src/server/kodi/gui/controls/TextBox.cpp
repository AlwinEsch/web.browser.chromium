/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "TextBox.h"

#include "../../../../sandbox/kodi/gui/controls/TextBox.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/TextBox.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_TextBox_h::HandleMessage(int funcGroup,
                                             int func,
                                             const msgpack::unpacked& in,
                                             msgpack::sbuffer& out,
                                             const char* data,
                                             std::size_t size,
                                             std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_TextBox_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CTextBox_create:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control = new kodi::gui::controls::CTextBox(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CTextBox_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CTextBox_destroy:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control =
          reinterpret_cast<kodi::gui::controls::CTextBox*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CTextBox_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control =
          reinterpret_cast<kodi::gui::controls::CTextBox*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CTextBox_Reset:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control =
          reinterpret_cast<kodi::gui::controls::CTextBox*>(std::get<0>(t));
      if (!control)
        break;

      control->Reset();
      break;
    }
    case kodi_gui_controls_CTextBox_SetText:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_SetText t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control =
          reinterpret_cast<kodi::gui::controls::CTextBox*>(std::get<0>(t));
      if (!control)
        break;

      control->SetText(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CTextBox_GetText:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_GetText t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control =
          reinterpret_cast<kodi::gui::controls::CTextBox*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CTextBox_GetText(control->GetText()));
      break;
    }
    case kodi_gui_controls_CTextBox_Scroll:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_Scroll t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control =
          reinterpret_cast<kodi::gui::controls::CTextBox*>(std::get<0>(t));
      if (!control)
        break;

      control->Scroll(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CTextBox_SetAutoScrolling:
    {
      msgParent__IN_kodi_gui_controls_CTextBox_SetAutoScrolling t = in.get().as<decltype(t)>();
      kodi::gui::controls::CTextBox* control =
          reinterpret_cast<kodi::gui::controls::CTextBox*>(std::get<0>(t));
      if (!control)
        break;

      control->SetAutoScrolling(std::get<1>(t), std::get<2>(t), std::get<3>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_TextBox_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
