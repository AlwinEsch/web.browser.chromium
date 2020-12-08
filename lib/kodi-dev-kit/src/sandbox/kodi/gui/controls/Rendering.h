/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../../../include/kodi/c-api/gui/controls/rendering.h"

#include <string>
#include <tuple>

namespace kodi
{
namespace sandbox
{

typedef enum funcParent_gui_controls_Rendering_h : int
{
  kodi_gui_controls_CRendering_create,
  kodi_gui_controls_CRendering_destroy,
  funcParent_gui_controls_Rendering_h_LAST
} funcParent_gui_controls_Rendering_h;

// clang-format off
typedef std::tuple<uint64_t, uint64_t, int>                                       msgParent__IN_kodi_gui_controls_CRendering_create;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CRendering_create;
typedef std::tuple<uint64_t>                                                      msgParent__IN_kodi_gui_controls_CRendering_destroy;
typedef std::tuple<uint64_t>                                                      msgParent_OUT_kodi_gui_controls_CRendering_destroy;
// clang-format on

//==============================================================================

typedef enum funcChild_gui_controls_Rendering_h
{
  kodi_gui_controls_CRendering_Create,
  kodi_gui_controls_CRendering_Render,
  kodi_gui_controls_CRendering_Stop,
  kodi_gui_controls_CRendering_Dirty,
  funcChild_gui_controls_Rendering_h_LAST
} funcChild_gui_controls_Rendering_h;

// clang-format off
typedef std::tuple<uint64_t, int, int, int, int, uint64_t>                        msgChild__IN_kodi_gui_controls_CRendering_Create;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_gui_controls_CRendering_Create;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_gui_controls_CRendering_Render;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_gui_controls_CRendering_Render;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_gui_controls_CRendering_Stop;
typedef std::tuple<DummyValue>                                                    msgChild_OUT_kodi_gui_controls_CRendering_Stop;
typedef std::tuple<uint64_t>                                                      msgChild__IN_kodi_gui_controls_CRendering_Dirty;
typedef std::tuple<bool>                                                          msgChild_OUT_kodi_gui_controls_CRendering_Dirty;
// clang-format on

} /* namespace sandbox */
} /* namespace kodi */

