/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <string>

namespace GLExtCheck
{

bool CheckExtensionSupported(const std::string& name);

void* GetEGLFunc(const std::string& name);

const char* GetEGLErrorString(uint32_t error);

const char* GetLastEGLErrorString();

} /* namespace OpenGLExtensionCheck */
