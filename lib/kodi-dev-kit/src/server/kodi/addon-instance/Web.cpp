/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Web.h"

#include "../../../sandbox/kodi/addon-instance/Web.h"
#include "../../utils/Log.h"

#include "../../../../../../src/addon/addon.h"

namespace kodi
{
namespace sandbox
{
namespace parent
{





bool C_addoninstance_Web_h::HandleMessage(int funcGroup,
                                   int func,
                                   const msgpack::unpacked& in,
                                   msgpack::sbuffer& out,
                                   const char* data,
                                   std::size_t size,
                                   std::size_t offset)
{
  if (funcGroup != funcGroup_addoninstance_Web_h)
    return false;

  switch (func)
  {
    case kodi_addoninstance_CInstanceWeb_create:
    {
/*
      msgParent__IN_kodi_addoninstance_CInstanceWeb_create t = in.get().as<decltype(t)>();
      CInstanceWebHdl* instance = new CInstanceWebHdl(std::get<0>(t), m_childLauncher, reinterpret_cast<void*>(std::get<1>(t)),
                                          std::get<2>(t));
      msgpack::pack(out, msgParent_OUT_kodi_addoninstance_CInstanceWeb_create(uint64_t(instance)));*/
      break;
    }
    case kodi_addoninstance_CInstanceWeb_destroy:
    {
//       msgParent__IN_kodi_addoninstance_CInstanceWeb_destroy t = in.get().as<decltype(t)>();
//       CInstanceWebHdl* instance = reinterpret_cast<CInstanceWebHdl*>(std::get<0>(t));
//       if (!instance)
//         break;
//
//       delete instance;
      break;
    }
    case kodi_addoninstance_CInstanceWeb_StartDone:
    {
      msgParent__IN_kodi_addoninstance_CInstanceWeb_StartDone t = in.get().as<decltype(t)>();
      kodi::addon::CInstanceWebOwn* instance = reinterpret_cast<kodi::addon::CInstanceWebOwn*>(std::get<0>(t));
      instance->StartDone(std::get<1>(t));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_InhibitShutdown:
    {
      msgParent__IN_kodi_addoninstance_CInstanceWeb_InhibitShutdown t = in.get().as<decltype(t)>();
      kodi::addon::CInstanceWebOwn* instance = reinterpret_cast<kodi::addon::CInstanceWebOwn*>(std::get<0>(t));
      instance->InhibitShutdown(std::get<1>(t));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_InhibitScreensaver:
    {
      msgParent__IN_kodi_addoninstance_CInstanceWeb_InhibitScreensaver t = in.get().as<decltype(t)>();
      kodi::addon::CInstanceWebOwn* instance = reinterpret_cast<kodi::addon::CInstanceWebOwn*>(std::get<0>(t));
      instance->InhibitScreensaver(std::get<1>(t));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_UserPath:
    {
      msgParent__IN_kodi_addoninstance_CInstanceWeb_UserPath t = in.get().as<decltype(t)>();
      kodi::addon::CInstanceWebOwn* instance = reinterpret_cast<kodi::addon::CInstanceWebOwn*>(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_addoninstance_CInstanceWeb_UserPath(instance->UserPath()));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_AddonLibPath:
    {
      msgParent__IN_kodi_addoninstance_CInstanceWeb_AddonLibPath t = in.get().as<decltype(t)>();
      kodi::addon::CInstanceWebOwn* instance = reinterpret_cast<kodi::addon::CInstanceWebOwn*>(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_addoninstance_CInstanceWeb_AddonLibPath(instance->AddonLibPath()));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_AddonSharePath:
    {
      msgParent__IN_kodi_addoninstance_CInstanceWeb_AddonSharePath t = in.get().as<decltype(t)>();
      kodi::addon::CInstanceWebOwn* instance = reinterpret_cast<kodi::addon::CInstanceWebOwn*>(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_addoninstance_CInstanceWeb_AddonSharePath(instance->AddonSharePath()));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_IsMuted:
    {
      msgParent__IN_kodi_addoninstance_CInstanceWeb_IsMuted t = in.get().as<decltype(t)>();
      kodi::addon::CInstanceWebOwn* instance = reinterpret_cast<kodi::addon::CInstanceWebOwn*>(std::get<0>(t));
      msgpack::pack(out, msgParent_OUT_kodi_addoninstance_CInstanceWeb_IsMuted(instance->IsMuted()));
      break;
    }


    case kodi_addoninstance_CWebControl_create:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_create t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      CWebBrowserClient* client = new CWebBrowserClient(*control, std::get<1>(t), std::get<2>(t), this);
      msgpack::pack(out, msgParent_OUT_kodi_addoninstance_CWebControl_create(uint64_t(client)));
      break;
    }
    case kodi_addoninstance_CWebControl_destroy:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_destroy t = in.get().as<decltype(t)>();
      CWebBrowserClient* client = reinterpret_cast<CWebBrowserClient*>(std::get<2>(t));
      delete client;
      break;
    }
    case kodi_addoninstance_CWebControl_SetControlReady:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetControlReady t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_control_ready(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t));
      break;
    }
    case kodi_addoninstance_CWebControl_SetOpenedAddress:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetOpenedAddress t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_opened_address(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t).c_str());
      break;
    }
    case kodi_addoninstance_CWebControl_SetOpenedTitle:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetOpenedTitle t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_opened_title(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t).c_str());
      break;
    }
    case kodi_addoninstance_CWebControl_SetIconURL:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetIconURL t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_icon_url(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t).c_str());
      break;
    }
    case kodi_addoninstance_CWebControl_SetFullscreen:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetFullscreen t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_fullscreen(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t));
      break;
    }
    case kodi_addoninstance_CWebControl_SetLoadingState:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetLoadingState t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_loading_state(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t), std::get<2>(t), std::get<3>(t));
      break;
    }
    case kodi_addoninstance_CWebControl_SetTooltip:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetTooltip t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_tooltip(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t).c_str());
      break;
    }
    case kodi_addoninstance_CWebControl_SetStatusMessage:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_SetStatusMessage t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_set_status_message(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t).c_str());
      break;
    }
    case kodi_addoninstance_CWebControl_RequestOpenSiteInNewTab:
    {
      msgParent__IN_kodi_addoninstance_CWebControl_RequestOpenSiteInNewTab t = in.get().as<decltype(t)>();
      WEB_CONTROL_HANDLE* control = reinterpret_cast<WEB_CONTROL_HANDLE*>(std::get<0>(t));
      const AddonInstance_WebAddon* data = control->instanceData;
      data->toKodi->control_request_open_site_in_new_tab(data->toKodi->kodiInstance, control->props->pControlIdent,
                                            std::get<1>(t).c_str());
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "C_addoninstance_Web_h::%s: addon called with unknown function id '%i' on group 'kodi'",
          __func__, func);
      return false;
  }

  return true;
}

void C_addoninstance_Web_h::MainLoop(uint64_t hdl)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_MainLoop));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CInstanceWeb_MainLoop(hdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in);
}

void C_addoninstance_Web_h::MainShutdown(uint64_t hdl)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_MainShutdown));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CInstanceWeb_MainShutdown(hdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in);
}

void C_addoninstance_Web_h::SetMute(uint64_t hdl, bool mute)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_SetMute));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CInstanceWeb_SetMute(hdl, mute));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in);
}

bool C_addoninstance_Web_h::SetLanguage(uint64_t hdl, const std::string& language)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_SetLanguage));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CInstanceWeb_SetLanguage(hdl, language));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CInstanceWeb_SetLanguage t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

uint64_t C_addoninstance_Web_h::CreateControl(uint64_t hdl,
                                              const std::string& sourceName,
                                              const std::string& startURL,
                                              WEB_CONTROL_HANDLE& handle,
                                              int& dataIdentifier)
{
  WEB_GUI_PROPS props;
  props.strName = handle.props->strName;
  props.pDevice = uint64_t(handle.props->pDevice);
  props.fXPos = handle.props->fXPos;
  props.fYPos = handle.props->fYPos;
  props.fWidth = handle.props->fWidth;
  props.fHeight = handle.props->fHeight;
  props.fPixelRatio = handle.props->fPixelRatio;
  props.fFPS = handle.props->fFPS;
  props.fSkinXPos = handle.props->fSkinXPos;
  props.fSkinYPos = handle.props->fSkinYPos;
  props.fSkinWidth = handle.props->fSkinWidth;
  props.fSkinHeight = handle.props->fSkinHeight;
  props.bUseTransparentBackground = handle.props->bUseTransparentBackground;
  props.iBackgroundColorARGB = handle.props->iBackgroundColorARGB;
  props.iGUIItemLeft = handle.props->iGUIItemLeft;
  props.iGUIItemRight = handle.props->iGUIItemRight;
  props.iGUIItemTop = handle.props->iGUIItemTop;
  props.iGUIItemBottom = handle.props->iGUIItemBottom;
  props.iGUIItemBack = handle.props->iGUIItemBack;
  props.pControlIdent = uint64_t(handle.props->pControlIdent);

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_CreateControl));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CInstanceWeb_CreateControl(hdl, props, sourceName, startURL, uint64_t(&handle)));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CInstanceWeb_CreateControl t = ident.get().as<decltype(t)>();

  handle.controlHdlAddonChild = std::get<0>(t);
  handle.controlHdlAddonParent = reinterpret_cast<void*>(std::get<2>(t));
  handle.dataIdentifier = std::get<1>(t);
  dataIdentifier = std::get<1>(t);
  return std::get<0>(t);
}

bool C_addoninstance_Web_h::DestroyControl(uint64_t hdl, uint64_t control, bool complete)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_DestroyControl));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CInstanceWeb_DestroyControl(hdl, control, complete));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CInstanceWeb_DestroyControl t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void C_addoninstance_Web_h::ControlRender(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_Render));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_Render(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

bool C_addoninstance_Web_h::ControlDirty(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_Dirty));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_Dirty(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CWebControl_Dirty t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool C_addoninstance_Web_h::ControlOnInit(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_OnInit));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_OnInit(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CWebControl_OnInit t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool C_addoninstance_Web_h::ControlOnAction(uint64_t controlHdl, const kodi::gui::input::CAction& action, int& nextItem)
{
  gui_input_CAction ifcAction;
  ifcAction.id = action.GetID();
  ifcAction.name = action.GetName();
  ifcAction.amount = action.GetAmountList();
  ifcAction.repeat = action.GetRepeat();
  ifcAction.holdTime = action.GetHoldTime();
  ifcAction.buttonCode = action.GetButtonCode();
  ifcAction.unicode = action.GetUnicode();
  ifcAction.text = action.GetText();

  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_OnAction));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_OnAction(controlHdl, ifcAction, nextItem));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CWebControl_OnAction t = ident.get().as<decltype(t)>();
  nextItem = std::get<1>(t);
  return std::get<0>(t);
}

bool C_addoninstance_Web_h::ControlOnMouseEvent(uint64_t controlHdl, int id, double x, double y, double offsetX, double offsetY, int state)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_OnMouseEvent));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_OnMouseEvent(controlHdl, id, x, y, offsetX, offsetY, state));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CWebControl_OnMouseEvent t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool C_addoninstance_Web_h::ControlOpenWebsite(uint64_t controlHdl, const std::string& url)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_OpenWebsite));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_OpenWebsite(controlHdl, url));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CWebControl_OpenWebsite t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool C_addoninstance_Web_h::ControlGetHistory(uint64_t controlHdl, std::vector<std::string>& historyWebsiteNames, bool behindCurrent)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_GetHistory));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_GetHistory(controlHdl, historyWebsiteNames, behindCurrent));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgChild_OUT_kodi_addoninstance_CWebControl_GetHistory t = ident.get().as<decltype(t)>();
  historyWebsiteNames = std::get<1>(t);
  return std::get<0>(t);
}

void C_addoninstance_Web_h::ControlSearchText(uint64_t controlHdl, const std::string& text, bool forward, bool matchCase, bool findNext)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_SearchText));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_SearchText(controlHdl, text, forward, matchCase, findNext));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

void C_addoninstance_Web_h::ControlStopSearch(uint64_t controlHdl, bool clearSelection)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_StopSearch));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_StopSearch(controlHdl, clearSelection));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

void C_addoninstance_Web_h::ControlReload(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_Reload));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_Reload(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

void C_addoninstance_Web_h::ControlStopLoad(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_StopLoad));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_StopLoad(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

void C_addoninstance_Web_h::ControlGoBack(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_GoBack));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_GoBack(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

void C_addoninstance_Web_h::ControlGoForward(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_GoForward));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_GoForward(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

void C_addoninstance_Web_h::ControlOpenOwnContextMenu(uint64_t controlHdl)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_OpenOwnContextMenu));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_OpenOwnContextMenu(controlHdl));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

void C_addoninstance_Web_h::ControlScreenSizeChange(uint64_t controlHdl, float x, float y, float width, float height, bool fullscreen)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_ScreenSizeChange));
  msgpack::pack(in, msgChild__IN_kodi_addoninstance_CWebControl_ScreenSizeChange(controlHdl, x, y, width, height, fullscreen));
  m_childLauncher.GetCurrentTransmitter()->SendMessage(in, out);
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */

