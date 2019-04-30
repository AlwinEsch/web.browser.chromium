/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>
#include <kodi/General.h>

#define TEST_BUILD 1

inline void LOG_MESSAGE(const AddonLog loglevel, const char* format, ...)
{
  char buffer[16384];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

#ifdef TEST_BUILD
  fprintf(stderr, "KODIChromium - %04i - %s\n", loglevel, buffer);
#else
  kodi::Log(loglevel, buffer);
#endif
}

void LOG_INTERNAL_MESSAGE(const AddonLog loglevel, const char *format, ...);
