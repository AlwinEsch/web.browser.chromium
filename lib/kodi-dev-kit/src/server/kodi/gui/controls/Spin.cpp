/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Spin.h"

#include "../../../../sandbox/kodi/gui/controls/Spin.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/Spin.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_Spin_h::HandleMessage(int funcGroup,
                                          int func,
                                          const msgpack::unpacked& in,
                                          msgpack::sbuffer& out,
                                          const char* data,
                                          std::size_t size,
                                          std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_Spin_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CSpin_create:
    {
      msgParent__IN_kodi_gui_controls_CSpin_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control = new kodi::gui::controls::CSpin(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CSpin_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CSpin_destroy:
    {
      msgParent__IN_kodi_gui_controls_CSpin_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CSpin_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSpin_SetEnabled:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetEnabled t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetEnabled(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSpin_SetText:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetText t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetText(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSpin_Reset:
    {
      msgParent__IN_kodi_gui_controls_CSpin_Reset t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->Reset();
      break;
    }
    case kodi_gui_controls_CSpin_SetType:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetType t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetType(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSpin_AddLabel:
    {
      msgParent__IN_kodi_gui_controls_CSpin_AddLabel t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->AddLabel(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_controls_CSpin_AddLabel2:
    {
      msgParent__IN_kodi_gui_controls_CSpin_AddLabel2 t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->AddLabel(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_controls_CSpin_SetStringValue:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetStringValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetStringValue(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSpin_GetStringValue:
    {
      msgParent__IN_kodi_gui_controls_CSpin_GetStringValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(
          out, msgParent_OUT_kodi_gui_controls_CSpin_GetStringValue(control->GetStringValue()));
      break;
    }
    case kodi_gui_controls_CSpin_SetIntRange:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetIntRange t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetIntRange(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_controls_CSpin_SetIntValue:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetIntValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetIntValue(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSpin_GetIntValue:
    {
      msgParent__IN_kodi_gui_controls_CSpin_GetIntValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CSpin_GetIntValue(control->GetIntValue()));
      break;
    }
    case kodi_gui_controls_CSpin_SetFloatRange:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetFloatRange t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetFloatRange(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_controls_CSpin_SetFloatValue:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetFloatValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetFloatValue(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CSpin_GetFloatValue:
    {
      msgParent__IN_kodi_gui_controls_CSpin_GetFloatValue t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_controls_CSpin_GetFloatValue(control->GetFloatValue()));
      break;
    }
    case kodi_gui_controls_CSpin_SetFloatInterval:
    {
      msgParent__IN_kodi_gui_controls_CSpin_SetFloatInterval t = in.get().as<decltype(t)>();
      kodi::gui::controls::CSpin* control =
          reinterpret_cast<kodi::gui::controls::CSpin*>(std::get<0>(t));
      if (!control)
        break;

      control->SetFloatInterval(std::get<1>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_Spin_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
