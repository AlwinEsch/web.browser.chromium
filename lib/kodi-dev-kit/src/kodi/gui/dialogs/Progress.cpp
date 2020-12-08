/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/Progress.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/Progress.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{

CProgress::CProgress()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_create(uint64_t(this)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CProgress_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CProgress::~CProgress()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::Open()
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_Open));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_Open(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::SetHeading(const std::string& heading)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_SetHeading));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_SetHeading(m_handle, heading));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::SetLine(unsigned int iLine, const std::string& line)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_SetLine));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_SetLine(m_handle, iLine, line));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::SetCanCancel(bool canCancel)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_SetCanCancel));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_SetCanCancel(m_handle, canCancel));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CProgress::IsCanceled() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_IsCanceled));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_IsCanceled(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CProgress_IsCanceled t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CProgress::SetPercentage(int percentage)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Progress_h,
                                  kodi_gui_dialogs_CProgress_SetPercentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_SetPercentage(m_handle, percentage));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

int CProgress::GetPercentage() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Progress_h,
                                  kodi_gui_dialogs_CProgress_GetPercentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_GetPercentage(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CProgress_GetPercentage t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CProgress::ShowProgressBar(bool onOff)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Progress_h,
                                  kodi_gui_dialogs_CProgress_ShowProgressBar));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_ShowProgressBar(m_handle, onOff));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::SetProgressMax(int max)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Progress_h,
                                  kodi_gui_dialogs_CProgress_SetProgressMax));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_SetProgressMax(m_handle, max));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CProgress::SetProgressAdvance(int steps)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_Progress_h,
                                  kodi_gui_dialogs_CProgress_SetProgressAdvance));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_SetProgressAdvance(m_handle, steps));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

bool CProgress::Abort()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_dialogs_Progress_h, kodi_gui_dialogs_CProgress_Abort));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CProgress_Abort(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CProgress_Abort t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
