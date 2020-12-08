/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Edit.h"

#include "../../../../sandbox/kodi/gui/controls/Edit.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/Edit.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_Edit_h::HandleMessage(int funcGroup,
                                          int func,
                                          const msgpack::unpacked& in,
                                          msgpack::sbuffer& out,
                                          const char* data,
                                          std::size_t size,
                                          std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_Edit_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CEdit_create:
    {
      msgParent__IN_kodi_gui_controls_CEdit_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control = new kodi::gui::controls::CEdit(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CEdit_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CEdit_destroy:
    {
      msgParent__IN_kodi_gui_controls_CEdit_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CEdit_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CEdit_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CEdit_SetEnabled:
    {
      msgParent__IN_kodi_gui_controls_CEdit_SetEnabled t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      control->SetEnabled(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CEdit_SetLabel:
    {
      msgParent__IN_kodi_gui_controls_CEdit_SetLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      control->SetLabel(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CEdit_GetLabel:
    {
      msgParent__IN_kodi_gui_controls_CEdit_GetLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CEdit_GetLabel(control->GetLabel()));
      break;
    }
    case kodi_gui_controls_CEdit_SetText:
    {
      msgParent__IN_kodi_gui_controls_CEdit_SetText t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      control->SetText(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CEdit_GetText:
    {
      msgParent__IN_kodi_gui_controls_CEdit_GetText t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CEdit_GetText(control->GetText()));
      break;
    }
    case kodi_gui_controls_CEdit_SetCursorPosition:
    {
      msgParent__IN_kodi_gui_controls_CEdit_SetCursorPosition t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      control->SetCursorPosition(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CEdit_GetCursorPosition:
    {
      msgParent__IN_kodi_gui_controls_CEdit_GetCursorPosition t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CEdit_GetCursorPosition(
                             control->GetCursorPosition()));
      break;
    }
    case kodi_gui_controls_CEdit_SetInputType:
    {
      msgParent__IN_kodi_gui_controls_CEdit_SetInputType t = in.get().as<decltype(t)>();
      kodi::gui::controls::CEdit* control =
          reinterpret_cast<kodi::gui::controls::CEdit*>(std::get<0>(t));
      if (!control)
        break;

      control->SetInputType(std::get<1>(t), std::get<2>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_Edit_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
