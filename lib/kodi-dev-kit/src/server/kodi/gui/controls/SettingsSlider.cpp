/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "SettingsSlider.h"

#include "../../../../sandbox/kodi/gui/controls/SettingsSlider.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/SettingsSlider.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_SettingsSlider_h::HandleMessage(int funcGroup,
                                                    int func,
                                                    const msgpack::unpacked& in,
                                                    msgpack::sbuffer& out,
                                                    const char* data,
                                                    std::size_t size,
                                                    std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_SettingsSlider_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CSettingsSlider_create:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control = new kodi::gui::controls::CSettingsSlider(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CSettingsSlider_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CSettingsSlider_destroy:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetEnabled:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetEnabled t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetEnabled(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetText:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetText t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetText(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_Reset:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_Reset t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->Reset();
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetIntRange:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntRange t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetIntRange(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetIntValue:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetIntValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetIntValue(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_GetIntValue:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_GetIntValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(
          out, msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetIntValue(control->GetIntValue()));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetIntInterval:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatInterval t =
          in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetIntInterval(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetPercentage:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetPercentage t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetPercentage(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_GetPercentage:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_GetPercentage t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetPercentage(
                             control->GetIntValue()));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetFloatRange:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatRange t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetFloatRange(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetFloatValue:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetFloatValue(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_GetFloatValue:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_GetFloatValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CSettingsSlider_GetFloatValue(
                             control->GetFloatValue()));
      break;
    }
    case kodi_gui_controls_CSettingsSlider_SetFloatInterval:
    {
      msgParent__IN_kodi_gui_controls_CSettingsSlider_SetFloatInterval t =
          in.get().as<decltype(t)>();
      kodi::gui::controls::CSettingsSlider* control =
          reinterpret_cast<kodi::gui::controls::CSettingsSlider*>(std::get<0>(t));
      if (!control)
        break;

      control->SetFloatInterval(std::get<1>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "C_gui_controls_SettingsSlider_h::%s: addon called with unknown "
          "function id '%i' on group 'funcGroup_gui_controls_SettingsSlider_h'",
          __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
