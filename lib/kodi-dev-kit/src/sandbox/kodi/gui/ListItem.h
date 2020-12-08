/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../shared_process_func_group.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_ListItem_h : int
{
  kodi_gui_CListItem_create,
  kodi_gui_CListItem_destroy,
  kodi_gui_CListItem_GetLabel,
  kodi_gui_CListItem_SetLabel,
  kodi_gui_CListItem_GetLabel2,
  kodi_gui_CListItem_SetLabel2,
  kodi_gui_CListItem_GetArt,
  kodi_gui_CListItem_SetArt,
  kodi_gui_CListItem_GetPath,
  kodi_gui_CListItem_SetPath,
  kodi_gui_CListItem_GetProperty,
  kodi_gui_CListItem_SetProperty,
  kodi_gui_CListItem_Select,
  kodi_gui_CListItem_IsSelected,
  funcParent_Network_h_LAST
} funcParent_gui_ListItem_h;

// clang-format off
typedef std::tuple<uint64_t, std::string, std::string, std::string>               msgParent__IN_kodi_gui_CListItem_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_CListItem_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CListItem_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_CListItem_destroy;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CListItem_GetLabel;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_CListItem_GetLabel;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CListItem_SetLabel;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CListItem_SetLabel;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CListItem_GetLabel2;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_CListItem_GetLabel2;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CListItem_SetLabel2;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CListItem_SetLabel2;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CListItem_GetArt;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_CListItem_GetArt;
typedef std::tuple<uint64_t, std::string, std::string>                            msgParent__IN_kodi_gui_CListItem_SetArt;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CListItem_SetArt;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CListItem_GetPath;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_CListItem_GetPath;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CListItem_SetPath;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CListItem_SetPath;
typedef std::tuple<uint64_t, std::string>                                         msgParent__IN_kodi_gui_CListItem_GetProperty;
typedef std::tuple<std::string>                                                   msgParent_OUT_kodi_gui_CListItem_GetProperty;
typedef std::tuple<uint64_t, std::string, std::string>                            msgParent__IN_kodi_gui_CListItem_SetProperty;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CListItem_SetProperty;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_CListItem_Select;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_CListItem_Select;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_CListItem_IsSelected;
typedef std::tuple<bool>                                                          msgParent_OUT_kodi_gui_CListItem_IsSelected;
// clang-format on

//==============================================================================

typedef enum funcChild_ListItem_h
{
  funcChild_ListItem_h_LAST
} funcChild_ListItem_h;

} /* namespace sandbox */
} /* namespace kodi */

