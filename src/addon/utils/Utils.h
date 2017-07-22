#pragma once
/*
 *      Copyright (C) 2015 Team KODI
 *      http:/kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <kodi/General.h>

#define TEST_BUILD 1
#ifdef TEST_BUILD
#define LOG_MESSAGE(loglevel, a...) \
do { \
  fprintf(stderr, "KODIChromium - " a); \
  fprintf(stderr, "\n"); \
} while(0)
#else
#define LOG_MESSAGE(loglevel, a...) \
do { \
  kodi::Log(loglevel, "KODIChromium - " a); \
} while(0)
#endif

void LOG_INTERNAL_MESSAGE(const AddonLog loglevel, const char *format, ...);
