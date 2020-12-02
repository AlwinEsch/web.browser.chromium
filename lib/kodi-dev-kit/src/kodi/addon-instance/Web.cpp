/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Web.h"

// Addon API
#include "../../../include/kodi/General.h"
#include "../../../include/kodi/Processor.h"
#include "../../../include/kodi/addon-instance/Web.h"

// Sandbox interact
#include "../../sandbox/ShareProcessReceiver.h"
#include "../../sandbox/ShareProcessTransmitter.h"
#include "../../sandbox/kodi/addon-instance/Web.h"

using namespace kodi::addon;
using namespace kodi::sandbox;
using namespace kodi::sandbox::child;

CWebControl::CWebControl(const WEB_ADDON_GUI_PROPS& properties, uint64_t handle, int dataIdentifier)
  : m_handle(handle), m_dataIdentifier(dataIdentifier), m_properties(properties)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_create));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_create(m_handle, reinterpret_cast<uint64_t>(this), dataIdentifier));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_addoninstance_CWebControl_create t = ident.get().as<decltype(t)>();
  m_handleBase = std::get<0>(t);
}

CWebControl::~CWebControl()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_destroy));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_destroy(m_handle, reinterpret_cast<uint64_t>(this), m_handleBase));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

void CWebControl::SetControlReady(bool ready)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_SetControlReady));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_SetControlReady(m_handle, ready));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::SetOpenedAddress(const std::string& title)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_SetOpenedAddress));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_SetOpenedAddress(m_handle, title));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::SetOpenedTitle(const std::string& title)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_SetOpenedTitle));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_SetOpenedTitle(m_handle, title));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::SetIconURL(const std::string& icon)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_SetIconURL));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_SetIconURL(m_handle, icon));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::SetFullscreen(bool fullscreen)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_SetFullscreen));
  msgpack::pack(in,
                msgParent__IN_kodi_addoninstance_CWebControl_SetFullscreen(m_handle, fullscreen));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::SetLoadingState(bool isLoading, bool canGoBack, bool canGoForward)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_SetLoadingState));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_SetLoadingState(
                        m_handle, isLoading, canGoBack, canGoForward));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::SetTooltip(const std::string& tooltip)
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CWebControl_SetTooltip));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CWebControl_SetTooltip(m_handle, tooltip));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::SetStatusMessage(const std::string& status)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_SetStatusMessage));
  msgpack::pack(in,
                msgParent__IN_kodi_addoninstance_CWebControl_SetStatusMessage(m_handle, status));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CWebControl::RequestOpenSiteInNewTab(const std::string& url)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CWebControl_RequestOpenSiteInNewTab));
  msgpack::pack(
      in, msgParent__IN_kodi_addoninstance_CWebControl_RequestOpenSiteInNewTab(m_handle, url));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

//##############################################################################

CInstanceWeb::CInstanceWeb(uint64_t handle) : m_handle(handle)
{
}

CInstanceWeb::~CInstanceWeb()
{
}

void CInstanceWeb::StartDone()
{
  msgpack::sbuffer in;
  msgpack::pack(
      in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_StartDone));
  msgpack::pack(in,
                msgParent__IN_kodi_addoninstance_CInstanceWeb_StartDone(m_handle, uint64_t(this)));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CInstanceWeb::InhibitShutdown(bool inhibit)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CInstanceWeb_InhibitShutdown));
  msgpack::pack(in,
                msgParent__IN_kodi_addoninstance_CInstanceWeb_InhibitShutdown(m_handle, inhibit));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

void CInstanceWeb::InhibitScreensaver(bool inhibit)
{
  msgpack::sbuffer in;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CInstanceWeb_InhibitScreensaver));
  msgpack::pack(
      in, msgParent__IN_kodi_addoninstance_CInstanceWeb_InhibitScreensaver(m_handle, inhibit));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in);
}

std::string CInstanceWeb::UserPath(const std::string& append)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_UserPath));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CInstanceWeb_UserPath(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_addoninstance_CInstanceWeb_UserPath t = ident.get().as<decltype(t)>();

  std::string ret = std::get<0>(t);
  if (!ret.empty() && (ret.at(ret.length() - 1) == '\\' || ret.at(ret.length() - 1) == '/'))
    ret = ret.substr(0, ret.length() - 1);

  if (!append.empty())
  {
    if (append.at(0) != '\\' && append.at(0) != '/')
#ifdef TARGET_WINDOWS
      ret.append("\\");
#else
      ret.append("/");
#endif
    ret.append(append);
  }

  return ret;
}

std::string CInstanceWeb::AddonLibPath(const std::string& append)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CInstanceWeb_AddonLibPath));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CInstanceWeb_AddonLibPath(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_addoninstance_CInstanceWeb_AddonLibPath t = ident.get().as<decltype(t)>();

  std::string ret = std::get<0>(t);
  if (!ret.empty() && (ret.at(ret.length() - 1) == '\\' || ret.at(ret.length() - 1) == '/'))
    ret = ret.substr(0, ret.length() - 1);

  if (!append.empty())
  {
    if (append.at(0) != '\\' && append.at(0) != '/')
#ifdef TARGET_WINDOWS
      ret.append("\\");
#else
      ret.append("/");
#endif
    ret.append(append);
  }

  return ret;
}

std::string CInstanceWeb::AddonSharePath(const std::string& append)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_addoninstance_Web_h,
                                  kodi_addoninstance_CInstanceWeb_AddonSharePath));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CInstanceWeb_AddonSharePath(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_addoninstance_CInstanceWeb_AddonSharePath t = ident.get().as<decltype(t)>();

  std::string ret = std::get<0>(t);
  if (!ret.empty() && (ret.at(ret.length() - 1) == '\\' || ret.at(ret.length() - 1) == '/'))
    ret = ret.substr(0, ret.length() - 1);

  if (!append.empty())
  {
    if (append.at(0) != '\\' && append.at(0) != '/')
#ifdef TARGET_WINDOWS
      ret.append("\\");
#else
      ret.append("/");
#endif
    ret.append(append);
  }

  return ret;
}

bool CInstanceWeb::IsMuted() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_addoninstance_Web_h, kodi_addoninstance_CInstanceWeb_IsMuted));
  msgpack::pack(in, msgParent__IN_kodi_addoninstance_CInstanceWeb_IsMuted(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_addoninstance_CInstanceWeb_IsMuted t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

//##############################################################################

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
    case kodi_addoninstance_CInstanceWeb_MainLoop:
    {
      msgChild__IN_kodi_addoninstance_CInstanceWeb_MainLoop t = in.get().as<decltype(t)>();
      CInstanceWeb* instance = reinterpret_cast<CInstanceWeb*>(std::get<0>(t));
      instance->MainLoop();
      break;
    }
    case kodi_addoninstance_CInstanceWeb_MainShutdown:
    {
      msgChild__IN_kodi_addoninstance_CInstanceWeb_MainLoop t = in.get().as<decltype(t)>();
      CInstanceWeb* instance = reinterpret_cast<CInstanceWeb*>(std::get<0>(t));
      instance->MainShutdown();
      break;
    }
    case kodi_addoninstance_CInstanceWeb_SetLanguage:
    {
      msgChild__IN_kodi_addoninstance_CInstanceWeb_SetLanguage t = in.get().as<decltype(t)>();
      CInstanceWeb* instance = reinterpret_cast<CInstanceWeb*>(std::get<0>(t));
      bool ret = instance->SetLanguage(std::get<1>(t));
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CInstanceWeb_SetLanguage(ret));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_SetMute:
    {
      msgChild__IN_kodi_addoninstance_CInstanceWeb_SetMute t = in.get().as<decltype(t)>();
      CInstanceWeb* instance = reinterpret_cast<CInstanceWeb*>(std::get<0>(t));
      instance->SetMute(std::get<1>(t));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_CreateControl:
    {
      msgChild__IN_kodi_addoninstance_CInstanceWeb_CreateControl t = in.get().as<decltype(t)>();
      CInstanceWeb* instance = reinterpret_cast<CInstanceWeb*>(std::get<0>(t));
      WEB_GUI_PROPS& props = std::get<1>(t);
      WEB_ADDON_GUI_PROPS propsAddon;
      strncpy(propsAddon.strName, props.strName.c_str(), sizeof(propsAddon.strName));
      propsAddon.pDevice = reinterpret_cast<void*>(props.pDevice);
      propsAddon.fXPos = props.fXPos;
      propsAddon.fYPos = props.fYPos;
      propsAddon.fWidth = props.fWidth;
      propsAddon.fHeight = props.fHeight;
      propsAddon.fPixelRatio = props.fPixelRatio;
      propsAddon.fFPS = props.fFPS;
      propsAddon.fSkinXPos = props.fSkinXPos;
      propsAddon.fSkinYPos = props.fSkinYPos;
      propsAddon.fSkinWidth = props.fSkinWidth;
      propsAddon.fSkinHeight = props.fSkinHeight;
      propsAddon.bUseTransparentBackground = props.bUseTransparentBackground;
      propsAddon.iBackgroundColorARGB = props.iBackgroundColorARGB;
      propsAddon.iGUIItemLeft = props.iGUIItemLeft;
      propsAddon.iGUIItemRight = props.iGUIItemRight;
      propsAddon.iGUIItemTop = props.iGUIItemTop;
      propsAddon.iGUIItemBottom = props.iGUIItemBottom;
      propsAddon.iGUIItemBack = props.iGUIItemBack;
      propsAddon.pControlIdent = reinterpret_cast<void*>(props.pControlIdent);

      CWebControl* control =
          instance->CreateControl(propsAddon, std::get<2>(t), std::get<3>(t), std::get<4>(t));

      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CInstanceWeb_CreateControl(
                             uint64_t(control),
                             control ? control->GetDataIdentifier() : -1,
                             control ? control->GetParentHdl_CWebControl() : 0));
      break;
    }
    case kodi_addoninstance_CInstanceWeb_DestroyControl:
    {
      msgChild__IN_kodi_addoninstance_CInstanceWeb_DestroyControl t = in.get().as<decltype(t)>();
      CInstanceWeb* instance = reinterpret_cast<CInstanceWeb*>(std::get<0>(t));
      bool ret =
          instance->DestroyControl(reinterpret_cast<CWebControl*>(std::get<1>(t)), std::get<2>(t));
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CInstanceWeb_DestroyControl(ret));
      break;
    }
    case kodi_addoninstance_CWebControl_Render:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_Render t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->Render();
      break;
    }
    case kodi_addoninstance_CWebControl_Dirty:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_Dirty t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      bool ret = control->Dirty();
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CWebControl_Dirty(ret));
      break;
    }
    case kodi_addoninstance_CWebControl_OnInit:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_OnInit t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      bool ret = control->OnInit();
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CWebControl_OnInit(ret));
      break;
    }
    case kodi_addoninstance_CWebControl_OnAction:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_OnAction t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      int nextItem = std::get<2>(t);
      bool ret = control->OnAction(std::get<1>(t), nextItem);
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CWebControl_OnAction(ret, nextItem));
      break;
    }
    case kodi_addoninstance_CWebControl_OnMouseEvent:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_OnMouseEvent t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      bool ret = control->OnMouseEvent(std::get<1>(t), std::get<2>(t), std::get<3>(t),
                                       std::get<4>(t), std::get<5>(t), std::get<6>(t));
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CWebControl_OnMouseEvent(ret));
      break;
    }
    case kodi_addoninstance_CWebControl_OpenWebsite:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_OpenWebsite t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      bool ret = control->OpenWebsite(std::get<1>(t));
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CWebControl_OpenWebsite(ret));
      break;
    }
    case kodi_addoninstance_CWebControl_GetHistory:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_GetHistory t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      std::vector<std::string>& history = std::get<1>(t);
      bool ret = control->GetHistory(history, std::get<2>(t));
      msgpack::pack(out, msgChild_OUT_kodi_addoninstance_CWebControl_GetHistory(ret, history));
      break;
    }
    case kodi_addoninstance_CWebControl_SearchText:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_SearchText t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->SearchText(std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t));
      break;
    }
    case kodi_addoninstance_CWebControl_StopSearch:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_StopSearch t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->StopSearch(std::get<1>(t));
      break;
    }
    case kodi_addoninstance_CWebControl_Reload:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_Reload t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->Reload();
      break;
    }
    case kodi_addoninstance_CWebControl_StopLoad:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_StopLoad t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->StopLoad();
      break;
    }
    case kodi_addoninstance_CWebControl_GoBack:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_GoBack t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->GoBack();
      break;
    }
    case kodi_addoninstance_CWebControl_GoForward:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_GoForward t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->GoForward();
      break;
    }
    case kodi_addoninstance_CWebControl_OpenOwnContextMenu:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_OpenOwnContextMenu t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->OpenOwnContextMenu();
      break;
    }
    case kodi_addoninstance_CWebControl_ScreenSizeChange:
    {
      msgChild__IN_kodi_addoninstance_CWebControl_ScreenSizeChange t = in.get().as<decltype(t)>();
      CWebControl* control = reinterpret_cast<CWebControl*>(std::get<0>(t));
      control->ScreenSizeChange(std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t),
                                std::get<5>(t));
      break;
    }
    default:
      kodi::Log(ADDON_LOG_ERROR,
                "C_addoninstance_Web_h::%s: addon called with unknown function id '%i' on group",
                __func__, func);
      return false;
  }

  return true;
}
