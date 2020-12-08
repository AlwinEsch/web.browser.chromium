/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Image.h"

#include "../../../../sandbox/kodi/gui/controls/Image.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/Image.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_controls_Image_h::HandleMessage(int funcGroup,
                                           int func,
                                           const msgpack::unpacked& in,
                                           msgpack::sbuffer& out,
                                           const char* data,
                                           std::size_t size,
                                           std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_Image_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CImage_create:
    {
      msgParent__IN_kodi_gui_controls_CImage_create t = in.get().as<decltype(t)>();
      kodi::gui::controls::CImage* control = new kodi::gui::controls::CImage(
          reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CImage_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CImage_destroy:
    {
      msgParent__IN_kodi_gui_controls_CImage_destroy t = in.get().as<decltype(t)>();
      kodi::gui::controls::CImage* control =
          reinterpret_cast<kodi::gui::controls::CImage*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    case kodi_gui_controls_CImage_SetVisible:
    {
      msgParent__IN_kodi_gui_controls_CImage_SetVisible t = in.get().as<decltype(t)>();
      kodi::gui::controls::CImage* control =
          reinterpret_cast<kodi::gui::controls::CImage*>(std::get<0>(t));
      if (!control)
        break;

      control->SetVisible(std::get<1>(t));
      break;
    }
    case kodi_gui_controls_CImage_SetFileName:
    {
      msgParent__IN_kodi_gui_controls_CImage_SetFileName t = in.get().as<decltype(t)>();
      kodi::gui::controls::CImage* control =
          reinterpret_cast<kodi::gui::controls::CImage*>(std::get<0>(t));
      if (!control)
        break;

      control->SetFileName(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_controls_CImage_SetColorDiffuse:
    {
      msgParent__IN_kodi_gui_controls_CImage_SetColorDiffuse t = in.get().as<decltype(t)>();
      kodi::gui::controls::CImage* control =
          reinterpret_cast<kodi::gui::controls::CImage*>(std::get<0>(t));
      if (!control)
        break;

      control->SetColorDiffuse(std::get<1>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_CImage_h::%s: addon called with unknown "
                               "function id '%i' on group 'C_gui_controls_Image_h'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
