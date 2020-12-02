/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "../../../../include/kodi/gui/controls/Image.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../../include/kodi/gui/Window.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/controls/Image.h"

using namespace kodi::sandbox;

namespace kodi
{
namespace gui
{
namespace controls
{

CImage::CImage(CWindow* window, int controlId) : m_handleWindow(window->GetControlHandle())
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_gui_controls_Image_h, kodi_gui_controls_CImage_create));
  msgpack::pack(
      in, msgParent__IN_kodi_gui_controls_CImage_create(uint64_t(this), m_handleWindow, controlId));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_gui_controls_CImage_create t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
}

CImage::~CImage()
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Image_h, kodi_gui_controls_CImage_destroy));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CImage_destroy(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CImage::SetVisible(bool visible)
{
  msgpack::sbuffer in;
  msgpack::pack(in,
                msgIdentifier(funcGroup_gui_controls_Image_h, kodi_gui_controls_CImage_SetVisible));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CImage_SetVisible(m_handle, visible));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CImage::SetFileName(const std::string& filename, bool useCache)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Image_h, kodi_gui_controls_CImage_SetFileName));
  msgpack::pack(in,
                msgParent__IN_kodi_gui_controls_CImage_SetFileName(m_handle, filename, useCache));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CImage::SetColorDiffuse(uint32_t colorDiffuse)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_gui_controls_Image_h, kodi_gui_controls_CImage_SetColorDiffuse));
  msgpack::pack(in, msgParent__IN_kodi_gui_controls_CImage_SetColorDiffuse(m_handle, colorDiffuse));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
