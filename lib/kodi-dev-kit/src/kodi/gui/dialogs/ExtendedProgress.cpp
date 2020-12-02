/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/dialogs/ExtendedProgress.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/dialogs/ExtendedProgress.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace dialogs
{

CExtendedProgress::CExtendedProgress(const std::string& title)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_create(uint64_t(this), title));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CExtendedProgress::~CExtendedProgress()
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CExtendedProgress::Title() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_Title));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Title(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Title t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CExtendedProgress::SetTitle(const std::string& title)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_SetTitle));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetTitle(m_handle, title));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

std::string CExtendedProgress::Text() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_Text));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Text(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Title t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CExtendedProgress::SetText(const std::string& text)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_SetText));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetText(m_handle, text));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

bool CExtendedProgress::IsFinished() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_IsFinished));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_IsFinished(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_IsFinished t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CExtendedProgress::MarkFinished()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_MarkFinished));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_MarkFinished(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

float CExtendedProgress::Percentage() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_Percentage));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_Percentage(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_dialogs_CExtendedProgress_Percentage t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CExtendedProgress::SetPercentage(float percentage)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_SetPercentage));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetPercentage(m_handle, percentage));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CExtendedProgress::SetProgress(int currentItem, int itemCount)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_dialogs_ExtendedProgress_h,
                                  kodi_gui_dialogs_CExtendedProgress_SetProgress));
  msgpack::pack(in, msgParent__IN_kodi_gui_dialogs_CExtendedProgress_SetProgress(
                        m_handle, currentItem, itemCount));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
