/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../include/kodi/gui/General.h"

#include "../../../include/kodi/Processor.h"
#include "../../sandbox/ShareProcessTransmitter.h"
#include "../../sandbox/kodi/gui/General.h"
#include "../../sandbox/shared_process_func_group.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{

void Lock()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_Lock));
  msgpack::pack(in, msgParent__IN_kodi_gui_Lock(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void Unlock()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_Unlock));
  msgpack::pack(in, msgParent__IN_kodi_gui_Unlock(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int GetScreenHeight()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_GetScreenHeight));
  msgpack::pack(in, msgParent__IN_kodi_gui_GetScreenHeight(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_GetScreenHeight t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int GetScreenWidth()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_GetScreenWidth));
  msgpack::pack(in, msgParent__IN_kodi_gui_GetScreenWidth(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_GetScreenWidth t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int GetVideoResolution()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_GetVideoResolution));
  msgpack::pack(in, msgParent__IN_kodi_gui_GetVideoResolution(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_GetVideoResolution t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int GetCurrentWindowDialogId()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_GetCurrentWindowDialogId));
  msgpack::pack(in, msgParent__IN_kodi_gui_GetCurrentWindowDialogId(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_GetCurrentWindowDialogId t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

int GetCurrentWindowId()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_GetCurrentWindowId));
  msgpack::pack(in, msgParent__IN_kodi_gui_GetCurrentWindowId(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_GetCurrentWindowId t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

kodi::HardwareContext GetHWContext()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup::funcGroup_gui_General_h,
                                  funcParent_gui_General_h::kodi_gui_GetHWContext));
  msgpack::pack(in, msgParent__IN_kodi_gui_GetHWContext(0));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_GetHWContext t = ident.get().as<decltype(t)>();
  return reinterpret_cast<kodi::HardwareContext>(std::get<0>(t));
}

} /* namespace gui */
} /* namespace kodi */
