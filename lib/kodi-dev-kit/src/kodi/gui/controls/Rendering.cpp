/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Rendering.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Rendering.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CRendering::CRendering(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Rendering_h, kodi_gui_controls_CRendering_create));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRendering_create(uint64_t(this),
                                                                      m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CRendering_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CRendering::~CRendering()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Rendering_h, kodi_gui_controls_CRendering_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CRendering_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
