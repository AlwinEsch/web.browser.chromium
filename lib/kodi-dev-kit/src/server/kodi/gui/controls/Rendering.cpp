/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Rendering.h"

#include "../../../../sandbox/kodi/gui/controls/Rendering.h"
#include "../../../utils/Log.h"

#include <kodi/gui/controls/Rendering.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

class ATTRIBUTE_HIDDEN CRenderingHdl : public kodi::gui::controls::CRendering
{
public:
  CRenderingHdl(uint64_t handle, CChildLauncher& launcher, gui::CWindow* window, int controlId)
    : kodi::gui::controls::CRendering(window, controlId), m_handle(handle), m_launcher(launcher)
  {
  }

  virtual ~CRenderingHdl() {}

  bool Create(int x, int y, int w, int h, kodi::HardwareContext device) override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(
        in, msgIdentifier(funcGroup_gui_controls_Rendering_h, kodi_gui_controls_CRendering_Create));
    msgpack::pack(in, msgChild__IN_kodi_gui_controls_CRendering_Create(m_handle, x, y, w, h,
                                                                       uint64_t(device)));
    m_launcher.GetCurrentTransmitter()->SendMessage(in);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_controls_CRendering_Create t = ident.get().as<decltype(t)>();
    return std::get<0>(t);
  }

  void Render() override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(
        in, msgIdentifier(funcGroup_gui_controls_Rendering_h, kodi_gui_controls_CRendering_Render));
    msgpack::pack(in, msgChild__IN_kodi_gui_controls_CRendering_Render(m_handle));
    m_launcher.GetCurrentTransmitter()->SendMessage(in);
  }

  void Stop() override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(
        in, msgIdentifier(funcGroup_gui_controls_Rendering_h, kodi_gui_controls_CRendering_Stop));
    msgpack::pack(in, msgChild__IN_kodi_gui_controls_CRendering_Stop(m_handle));
    m_launcher.GetCurrentTransmitter()->SendMessage(in);
  }

  bool Dirty() override
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(
        in, msgIdentifier(funcGroup_gui_controls_Rendering_h, kodi_gui_controls_CRendering_Dirty));
    msgpack::pack(in, msgChild__IN_kodi_gui_controls_CRendering_Dirty(m_handle));
    m_launcher.GetCurrentTransmitter()->SendMessage(in);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgChild_OUT_kodi_gui_controls_CRendering_Dirty t = ident.get().as<decltype(t)>();
    return std::get<0>(t);
  }

protected:
  uint64_t m_handle;
  CChildLauncher& m_launcher;
};


bool C_gui_controls_Rendering_h::HandleMessage(int funcGroup,
                                               int func,
                                               const msgpack::unpacked& in,
                                               msgpack::sbuffer& out,
                                               const char* data,
                                               std::size_t size,
                                               std::size_t offset)
{
  if (funcGroup != funcGroup_gui_controls_Rendering_h)
    return false;

  switch (func)
  {
    case kodi_gui_controls_CRendering_create:
    {
      msgParent__IN_kodi_gui_controls_CRendering_create t = in.get().as<decltype(t)>();
      CRenderingHdl* control =
          new CRenderingHdl(std::get<0>(t), m_childLauncher,
                            reinterpret_cast<gui::CWindow*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_gui_controls_CRendering_create(uint64_t(control)));

      break;
    }
    case kodi_gui_controls_CRendering_destroy:
    {
      msgParent__IN_kodi_gui_controls_CRendering_destroy t = in.get().as<decltype(t)>();
      CRenderingHdl* control = reinterpret_cast<CRenderingHdl*>(std::get<0>(t));
      if (!control)
        break;

      delete control;
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(ADDON_LOG_ERROR,
                               "C_gui_controls_Rendering_h::%s: addon called with unknown "
                               "function id '%i' on group 'kodi'",
                               __func__, func);
      return false;
  }

  return true;
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
