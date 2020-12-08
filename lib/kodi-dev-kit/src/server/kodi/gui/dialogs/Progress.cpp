/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Progress.h"

#include "../../../../sandbox/kodi/gui/dialogs/Progress.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/Progress.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_Progress_h::HandleMessage(int funcGroup,
                                             int func,
                                             const msgpack::unpacked& in,
                                             msgpack::sbuffer& out,
                                             const char* data,
                                             std::size_t size,
                                             std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_Progress_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_CProgress_create:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_create t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress = new kodi::gui::dialogs::CProgress;
      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_CProgress_create(uint64_t(progress)));

      break;
    }
    case kodi_gui_dialogs_CProgress_destroy:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_destroy t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      delete progress;
      break;
    }
    case kodi_gui_dialogs_CProgress_Open:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_Open t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->Open();
      break;
    }
    case kodi_gui_dialogs_CProgress_SetHeading:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_SetHeading t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetHeading(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CProgress_SetLine:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_SetLine t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetLine(std::get<1>(t), std::get<2>(t));
      break;
    }
    case kodi_gui_dialogs_CProgress_SetCanCancel:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_SetCanCancel t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetCanCancel(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CProgress_IsCanceled:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_IsCanceled t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_CProgress_IsCanceled(progress->IsCanceled()));
      break;
    }
    case kodi_gui_dialogs_CProgress_SetPercentage:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_SetPercentage t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetPercentage(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CProgress_GetPercentage:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_GetPercentage t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      msgpack::pack(
          out, msgParent_OUT_kodi_gui_dialogs_CProgress_GetPercentage(progress->GetPercentage()));
      break;
    }
    case kodi_gui_dialogs_CProgress_ShowProgressBar:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_ShowProgressBar t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->ShowProgressBar(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CProgress_SetProgressMax:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_SetProgressMax t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetProgressMax(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CProgress_SetProgressAdvance:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_SetProgressAdvance t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetProgressAdvance(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CProgress_Abort:
    {
      msgParent__IN_kodi_gui_dialogs_CProgress_Abort t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CProgress*>(std::get<0>(t));
      if (!progress)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_CProgress_Abort(progress->Abort()));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_Progress_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
