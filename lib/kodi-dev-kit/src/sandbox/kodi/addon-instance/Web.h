/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../shared_process_func_group.h"
#include "../../../../include/kodi/c-api/addon-instance/web.h"
#include "../gui/input/Action.h"

#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

struct WEB_GUI_PROPS
{
  std::string strName;

  uint64_t pDevice;

  float fXPos;
  float fYPos;
  float fWidth;
  float fHeight;
  float fPixelRatio;
  float fFPS;

  float fSkinXPos;
  float fSkinYPos;
  float fSkinWidth;
  float fSkinHeight;

  bool bUseTransparentBackground;
  uint32_t iBackgroundColorARGB;

  int iGUIItemLeft;
  int iGUIItemRight;
  int iGUIItemTop;
  int iGUIItemBottom;
  int iGUIItemBack;

  uint64_t pControlIdent;

  MSGPACK_DEFINE(strName, pDevice, fXPos, fYPos, fWidth, fHeight, fPixelRatio, fFPS, fSkinXPos, fSkinYPos, fSkinWidth, fSkinHeight, bUseTransparentBackground, iBackgroundColorARGB, iGUIItemLeft, iGUIItemRight, iGUIItemTop, iGUIItemBottom, iGUIItemBack, pControlIdent);
};

typedef enum funcParent_addoninstance_Web_h : int
{
  kodi_addoninstance_CInstanceWeb_create,
  kodi_addoninstance_CInstanceWeb_destroy,
  kodi_addoninstance_CInstanceWeb_StartDone,
  kodi_addoninstance_CInstanceWeb_InhibitShutdown,
  kodi_addoninstance_CInstanceWeb_InhibitScreensaver,
  kodi_addoninstance_CInstanceWeb_UserPath,
  kodi_addoninstance_CInstanceWeb_AddonLibPath,
  kodi_addoninstance_CInstanceWeb_AddonSharePath,
  kodi_addoninstance_CInstanceWeb_IsMuted,

  kodi_addoninstance_CWebControl_create,
  kodi_addoninstance_CWebControl_destroy,
  kodi_addoninstance_CWebControl_SetControlReady,
  kodi_addoninstance_CWebControl_SetOpenedAddress,
  kodi_addoninstance_CWebControl_SetOpenedTitle,
  kodi_addoninstance_CWebControl_SetIconURL,
  kodi_addoninstance_CWebControl_SetFullscreen,
  kodi_addoninstance_CWebControl_SetLoadingState,
  kodi_addoninstance_CWebControl_SetTooltip,
  kodi_addoninstance_CWebControl_SetStatusMessage,
  kodi_addoninstance_CWebControl_RequestOpenSiteInNewTab,
  funcParent_Network_h_LAST
} funcParent_gui_ListItem_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, std::string>                               msgParent__IN_kodi_addoninstance_CInstanceWeb_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_addoninstance_CInstanceWeb_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_addoninstance_CInstanceWeb_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_addoninstance_CInstanceWeb_destroy;
typedef std::tuple<uint64_t, uint64_t>                                            msgParent__IN_kodi_addoninstance_CInstanceWeb_StartDone;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CInstanceWeb_StartDone;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_addoninstance_CInstanceWeb_InhibitShutdown;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CInstanceWeb_InhibitShutdown;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_addoninstance_CInstanceWeb_InhibitScreensaver;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CInstanceWeb_InhibitScreensaver;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_addoninstance_CInstanceWeb_UserPath;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_addoninstance_CInstanceWeb_UserPath;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_addoninstance_CInstanceWeb_AddonLibPath;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_addoninstance_CInstanceWeb_AddonLibPath;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_addoninstance_CInstanceWeb_AddonSharePath;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_addoninstance_CInstanceWeb_AddonSharePath;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_addoninstance_CInstanceWeb_IsMuted;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_addoninstance_CInstanceWeb_IsMuted;

typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_addoninstance_CWebControl_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_addoninstance_CWebControl_create;
typedef std::tuple<uint64_t, uint64_t, uint64_t>                                  msgParent__IN_kodi_addoninstance_CWebControl_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_addoninstance_CWebControl_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_addoninstance_CWebControl_SetControlReady;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetControlReady;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_addoninstance_CWebControl_SetOpenedAddress;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetOpenedAddress;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_addoninstance_CWebControl_SetOpenedTitle;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetOpenedTitle;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_addoninstance_CWebControl_SetIconURL;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetIconURL;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_addoninstance_CWebControl_SetFullscreen;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetFullscreen;
typedef std::tuple<uint64_t, bool, bool, bool>                                    msgParent__IN_kodi_addoninstance_CWebControl_SetLoadingState;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetLoadingState;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_addoninstance_CWebControl_SetTooltip;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetTooltip;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_addoninstance_CWebControl_SetStatusMessage;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_SetStatusMessage;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_addoninstance_CWebControl_RequestOpenSiteInNewTab;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_addoninstance_CWebControl_RequestOpenSiteInNewTab;
// clang-format on

//==============================================================================

typedef enum funcChild_ListItem_h
{
  kodi_addoninstance_CInstanceWeb_SetMute,
  kodi_addoninstance_CInstanceWeb_SetLanguage,
  kodi_addoninstance_CInstanceWeb_CreateControl,
  kodi_addoninstance_CInstanceWeb_DestroyControl,
  kodi_addoninstance_CInstanceWeb_MainLoop,
  kodi_addoninstance_CInstanceWeb_MainShutdown,

  kodi_addoninstance_CWebControl_Render,
  kodi_addoninstance_CWebControl_Dirty,
  kodi_addoninstance_CWebControl_OnInit,
  kodi_addoninstance_CWebControl_OnAction,
  kodi_addoninstance_CWebControl_OnMouseEvent,
  kodi_addoninstance_CWebControl_OpenWebsite,
  kodi_addoninstance_CWebControl_GetHistory,
  kodi_addoninstance_CWebControl_SearchText,
  kodi_addoninstance_CWebControl_StopSearch,
  kodi_addoninstance_CWebControl_Reload,
  kodi_addoninstance_CWebControl_StopLoad,
  kodi_addoninstance_CWebControl_GoBack,
  kodi_addoninstance_CWebControl_GoForward,
  kodi_addoninstance_CWebControl_OpenOwnContextMenu,
  kodi_addoninstance_CWebControl_ScreenSizeChange,
  funcChild_ListItem_h_LAST
} funcChild_ListItem_h;

// clang-format off
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CInstanceWeb_MainLoop;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CInstanceWeb_MainLoop;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CInstanceWeb_MainShutdown;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CInstanceWeb_MainShutdown;
typedef std::tuple<uint64_t, std::string>                                         msgChild__IN_kodi_addoninstance_CInstanceWeb_SetLanguage;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_addoninstance_CInstanceWeb_SetLanguage;
typedef std::tuple<uint64_t, bool>                                                msgChild__IN_kodi_addoninstance_CInstanceWeb_SetMute;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CInstanceWeb_SetMute;
typedef std::tuple<uint64_t, WEB_GUI_PROPS, std::string, std::string, uint64_t>   msgChild__IN_kodi_addoninstance_CInstanceWeb_CreateControl;
typedef std::tuple<uint64_t, int, uint64_t>                                       msgChild_OUT_kodi_addoninstance_CInstanceWeb_CreateControl;
typedef std::tuple<uint64_t, uint64_t, bool>                                      msgChild__IN_kodi_addoninstance_CInstanceWeb_DestroyControl;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_addoninstance_CInstanceWeb_DestroyControl;

typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_Render;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_Render;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_Dirty;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_addoninstance_CWebControl_Dirty;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_OnInit;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_addoninstance_CWebControl_OnInit;
typedef std::tuple<uint64_t, gui_input_CAction, int>                              msgChild__IN_kodi_addoninstance_CWebControl_OnAction;
typedef std::tuple<bool, int>                                                     msgChild_OUT_kodi_addoninstance_CWebControl_OnAction;
typedef std::tuple<uint64_t, int, double, double, double, double, int>            msgChild__IN_kodi_addoninstance_CWebControl_OnMouseEvent;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_addoninstance_CWebControl_OnMouseEvent;
typedef std::tuple<uint64_t, std::string>                                         msgChild__IN_kodi_addoninstance_CWebControl_OpenWebsite;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_addoninstance_CWebControl_OpenWebsite;
typedef std::tuple<uint64_t, std::vector<std::string>, bool>                      msgChild__IN_kodi_addoninstance_CWebControl_GetHistory;
typedef std::tuple<bool, std::vector<std::string>>                                msgChild_OUT_kodi_addoninstance_CWebControl_GetHistory;
typedef std::tuple<uint64_t, std::string, bool, bool, bool>                       msgChild__IN_kodi_addoninstance_CWebControl_SearchText;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_SearchText;
typedef std::tuple<uint64_t, bool>                                                msgChild__IN_kodi_addoninstance_CWebControl_StopSearch;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_StopSearch;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_Reload;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_Reload;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_StopLoad;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_StopLoad;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_GoBack;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_GoBack;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_GoForward;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_GoForward;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_addoninstance_CWebControl_OpenOwnContextMenu;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_OpenOwnContextMenu;
typedef std::tuple<uint64_t, float, float, float, float, bool>                    msgChild__IN_kodi_addoninstance_CWebControl_ScreenSizeChange;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_addoninstance_CWebControl_ScreenSizeChange;
// clang-format on

} /* namespace sandbox */
} /* namespace kodi */

