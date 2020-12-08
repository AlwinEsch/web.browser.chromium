/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "AddonBaseInternal.h"

#include "../../include/kodi/AddonBase.h"

#include "../sandbox/kodi/AddonBase.h"
#include "../../include/kodi/Processor.h"
#include "../sandbox/ShareProcessTransmitter.h"

using namespace kodi::addon;
using namespace kodi::sandbox;
using namespace kodi::sandbox::child;

CAddonBase::CAddonBase()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AddonBase_h, kodi_CAddonBase_constructor));
  msgpack::pack(in, msgParent__IN_kodi_CAddonBase_constructor(uint64_t(this)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_CAddonBase_constructor t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CAddonBase::~CAddonBase()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AddonBase_h, kodi_CAddonBase_destructor));
  msgpack::pack(in, msgParent__IN_kodi_CAddonBase_destructor(uint64_t(this)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

//------------------------------------------------------------------------------

bool C_AddonBase_h::HandleMessage(int funcGroup,
                     int func,
                     const msgpack::unpacked& in,
                     msgpack::sbuffer& out,
                     const char* data,
                     std::size_t size,
                     std::size_t offset)
{
  return false;
}

