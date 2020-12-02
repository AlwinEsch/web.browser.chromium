/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "ExtendedProgress.h"

#include "../../../../sandbox/kodi/gui/dialogs/ExtendedProgress.h"
#include "../../../utils/Log.h"

#include <kodi/gui/dialogs/ExtendedProgress.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool C_gui_dialogs_ExtendedProgress_h::HandleMessage(int funcGroup,
                                                     int func,
                                                     const msgpack::unpacked& in,
                                                     msgpack::sbuffer& out,
                                                     const char* data,
                                                     std::size_t size,
                                                     std::size_t offset)
{
  if (funcGroup != funcGroup_gui_dialogs_ExtendedProgress_h)
    return false;

  switch (func)
  {
    case kodi_gui_dialogs_CExtendedProgress_create:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_create t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          new kodi::gui::dialogs::CExtendedProgress(std::get<1>(t));
      msgpack::pack(out,
                    msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_create(uint64_t(progress)));

      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_destroy:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_destroy t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      delete progress;
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_Title:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Title t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Title(progress->Title()));
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_SetTitle:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetTitle t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetTitle(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_Text:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Text t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      msgpack::pack(out, msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Text(progress->Text()));
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_SetText:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetText t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetText(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_IsFinished:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_IsFinished t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      msgpack::pack(
          out, msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_IsFinished(progress->IsFinished()));
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_MarkFinished:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_MarkFinished t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->MarkFinished();
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_Percentage:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Percentage t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      msgpack::pack(
          out, msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Percentage(progress->Percentage()));
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_SetPercentage:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetPercentage t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetPercentage(std::get<1>(t));
      break;
    }
    case kodi_gui_dialogs_CExtendedProgress_SetProgress:
    {
      msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetProgress t = in.get().as<decltype(t)>();
      kodi::gui::dialogs::CExtendedProgress* progress =
          reinterpret_cast<kodi::gui::dialogs::CExtendedProgress*>(std::get<0>(t));
      if (!progress)
        break;

      progress->SetProgress(std::get<1>(t), std::get<2>(t));
      break;
    }

    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_dialogs_ExtendedProgress_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */
