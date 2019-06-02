/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include <kodi/Filesystem.h>

#include "addon.h"
#include "Utils.h"
#include "StringUtils.h"

static unsigned int logCount = 0;
static std::string logFile;
static const char* const levelNames[] =
{
  "DEBUG",
  "INFO",
  "NOTICE",
  "WARNING",
  "ERROR",
  "SEVERE",
  "FATAL",
  "NONE"
};

void LOG_MESSAGE(const AddonLog loglevel, const char* format, ...)
{
  char buffer[16384];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

#ifdef TEST_BUILD
  time_t rawtime;
  time(&rawtime);
  std::string time = ctime(&rawtime);
  StringUtils::RemoveCRLF(time);

  std::string logMessage = StringUtils::Format("%s - %05i - %7s: %s\n", time.c_str(), logCount++,
                                               levelNames[loglevel], buffer);
  fprintf(stderr, "KODIChromium - %s", logMessage.c_str());
#else
  kodi::Log(loglevel, buffer);
#endif
}
