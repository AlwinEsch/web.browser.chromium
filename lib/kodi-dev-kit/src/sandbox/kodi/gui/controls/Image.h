/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/image.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Image_h : int
{
  kodi_gui_controls_CImage_create,
  kodi_gui_controls_CImage_destroy,
  kodi_gui_controls_CImage_SetVisible,
  kodi_gui_controls_CImage_SetFileName,
  kodi_gui_controls_CImage_SetColorDiffuse,
  funcParent_gui_controls_Image_h_LAST
} funcParent_gui_controls_Image_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CImage_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CImage_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CImage_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CImage_destroy;
typedef std::tuple<uint64_t, bool>                                                msgParent__IN_kodi_gui_controls_CImage_SetVisible;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CImage_SetVisible;
typedef std::tuple<uint64_t, std::string, bool>                                   msgParent__IN_kodi_gui_controls_CImage_SetFileName;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CImage_SetFileName;
typedef std::tuple<uint64_t, uint32_t>                                            msgParent__IN_kodi_gui_controls_CImage_SetColorDiffuse;
typedef std::tuple<DummyValue>                                                    msgParent_OUT_kodi_gui_controls_CImage_SetColorDiffuse;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Image_h
{
  funcChild_gui_controls_Image_h_LAST
} funcChild_gui_controls_Image_h;

} /* namespace sandbox */
} /* namespace kodi */

