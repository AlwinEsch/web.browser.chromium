/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <EGL/egl.h>

namespace chromium
{
namespace app
{
namespace main
{
namespace renderer
{

const char* GetEGLErrorString(uint32_t error);

// Returns the last EGL error as a string.
const char* GetLastEGLErrorString();

} /* renderer */
} /* main */
} /* app */
} /* chromium */
