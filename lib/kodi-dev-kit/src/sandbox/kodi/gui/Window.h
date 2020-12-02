/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../shared_process_func_group.h"
#include "input/Action.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_Window_h : int
{
  kodi_gui_CWindow_create,
  kodi_gui_CWindow_destroy,
  kodi_gui_CWindow_Show,
  kodi_gui_CWindow_Close,
  kodi_gui_CWindow_DoModal,
  kodi_gui_CWindow_SetFocusId,
  kodi_gui_CWindow_GetFocusId,
  kodi_gui_CWindow_SetControlLabel,
  kodi_gui_CWindow_SetControlVisible,
  kodi_gui_CWindow_SetControlSelected,
  kodi_gui_CWindow_SetProperty,
  kodi_gui_CWindow_GetProperty,
  kodi_gui_CWindow_SetPropertyInt,
  kodi_gui_CWindow_GetPropertyInt,
  kodi_gui_CWindow_SetPropertyBool,
  kodi_gui_CWindow_GetPropertyBool,
  kodi_gui_CWindow_SetPropertyDouble,
  kodi_gui_CWindow_GetPropertyDouble,
  kodi_gui_CWindow_ClearProperties,
  kodi_gui_CWindow_ClearProperty,
  kodi_gui_CWindow_ClearList,
  kodi_gui_CWindow_AddListItem,
  kodi_gui_CWindow_AddListItem2,
  kodi_gui_CWindow_RemoveListItem,
  kodi_gui_CWindow_RemoveListItem2,
  kodi_gui_CWindow_GetListItem,
  kodi_gui_CWindow_SetCurrentListPosition,
  kodi_gui_CWindow_GetCurrentListPosition,
  kodi_gui_CWindow_GetListSize,
  kodi_gui_CWindow_SetContainerProperty,
  kodi_gui_CWindow_SetContainerContent,
  kodi_gui_CWindow_GetCurrentContainerId,
  kodi_gui_CWindow_MarkDirtyRegion,
  funcParent_gui_Window_h_LAST
} funcParent_gui_Window_h;

// clang-format off
typedef std::tuple<uint64_t, std::string, std::string, bool, bool>                msgParent__IN_kodi_gui_CWindow_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_CWindow_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_CWindow_destroy;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_Show;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_CWindow_Show;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_Close;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_Close;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_DoModal;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_DoModal;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_CWindow_SetFocusId;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_CWindow_SetFocusId;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_GetFocusId;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_CWindow_GetFocusId;
typedef std::tuple<uint64_t, int, std::string>                                    msgParent__IN_kodi_gui_CWindow_SetControlLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetControlLabel;
typedef std::tuple<uint64_t, int, bool>                                           msgParent__IN_kodi_gui_CWindow_SetControlVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetControlVisible;
typedef std::tuple<uint64_t, int, bool>                                           msgParent__IN_kodi_gui_CWindow_SetControlSelected;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetControlSelected;
typedef std::tuple<uint64_t, std::string, std::string>                            msgParent__IN_kodi_gui_CWindow_SetProperty;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetProperty;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CWindow_GetProperty;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_CWindow_GetProperty;
typedef std::tuple<uint64_t, std::string, int>                                    msgParent__IN_kodi_gui_CWindow_SetPropertyInt;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetPropertyInt;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CWindow_GetPropertyInt;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_CWindow_GetPropertyInt;
typedef std::tuple<uint64_t, std::string, bool>                                   msgParent__IN_kodi_gui_CWindow_SetPropertyBool;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetPropertyBool;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CWindow_GetPropertyBool;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_CWindow_GetPropertyBool;
typedef std::tuple<uint64_t, std::string, double>                                 msgParent__IN_kodi_gui_CWindow_SetPropertyDouble;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetPropertyDouble;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CWindow_GetPropertyDouble;
typedef std::tuple<double>                                                        msgParent_OUT_kodi_gui_CWindow_GetPropertyDouble;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_ClearProperties;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_ClearProperties;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CWindow_ClearProperty;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_ClearProperty;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_ClearList;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_ClearList;
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_CWindow_AddListItem;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_AddListItem;
typedef std::tuple<uint64_t, std::string, int>                                    msgParent__IN_kodi_gui_CWindow_AddListItem2;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_AddListItem2;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_CWindow_RemoveListItem;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_RemoveListItem;
typedef std::tuple<uint64_t, uint64_t>                                            msgParent__IN_kodi_gui_CWindow_RemoveListItem2;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_RemoveListItem2;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_CWindow_GetListItem;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_CWindow_GetListItem;
typedef std::tuple<uint64_t, int>                                                 msgParent__IN_kodi_gui_CWindow_SetCurrentListPosition;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetCurrentListPosition;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_GetCurrentListPosition;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_CWindow_GetCurrentListPosition;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_GetListSize;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_CWindow_GetListSize;
typedef std::tuple<uint64_t, std::string, std::string>                            msgParent__IN_kodi_gui_CWindow_SetContainerProperty;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetContainerProperty;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CWindow_SetContainerContent;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_SetContainerContent;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_GetCurrentContainerId;
typedef std::tuple<int>                                                           msgParent_OUT_kodi_gui_CWindow_GetCurrentContainerId;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CWindow_MarkDirtyRegion;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CWindow_MarkDirtyRegion;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_Window_h : int
{
  kodi_gui_CWindow_OnInit,
  kodi_gui_CWindow_OnFocus,
  kodi_gui_CWindow_OnClick,
  kodi_gui_CWindow_OnAction,
  kodi_gui_CWindow_GetContextButtons,
  kodi_gui_CWindow_OnContextButton,
  funcChild_gui_Window_h_LAST
} funcChild_gui_Window_h;

// clang-format off
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_gui_CWindow_OnInit;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_gui_CWindow_OnInit;
typedef std::tuple<uint64_t, int>                                                 msgChild__IN_kodi_gui_CWindow_OnFocus;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_gui_CWindow_OnFocus;
typedef std::tuple<uint64_t, int>                                                 msgChild__IN_kodi_gui_CWindow_OnClick;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_gui_CWindow_OnClick;
typedef std::tuple<uint64_t, gui_input_CAction>                                   msgChild__IN_kodi_gui_CWindow_OnAction;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_gui_CWindow_OnAction;
typedef std::tuple<uint64_t, int, std::vector<std::pair<unsigned int, std::string>>>  msgChild__IN_kodi_gui_CWindow_GetContextButtons;
typedef std::tuple<std::vector<std::pair<unsigned int, std::string>>>             msgChild_OUT_kodi_gui_CWindow_GetContextButtons;
typedef std::tuple<uint64_t, int, unsigned int>                                   msgChild__IN_kodi_gui_CWindow_OnContextButton;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_gui_CWindow_OnContextButton;
// clang-format on

} /* namespace sandbox */
} /* namespace kodi */

