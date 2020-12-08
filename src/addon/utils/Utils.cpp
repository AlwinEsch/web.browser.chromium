/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Utils.h"

#include <kodi/Filesystem.h>
#include <kodi/tools/StringUtils.h>

using kodi::tools::StringUtils;

static unsigned int logCount = 0;
static std::string logFile;
static const char* const levelNames[] =
{
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "FATAL",
  "NONE"
};

void LOG_MESSAGE(const AddonLog loglevel, const char* format, ...)
{
  char buffer[FORMAT_BLOCK_SIZE] = {};
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, FORMAT_BLOCK_SIZE, format, args);
  va_end(args);

#ifdef TEST_BUILD
  time_t rawtime;
  time(&rawtime);
  std::string time = ctime(&rawtime);
  StringUtils::RemoveCRLF(time);

  std::string logMessage = StringUtils::Format("%s - %05i - %7s: %s\n", time.c_str(), logCount++,
                                               levelNames[loglevel], buffer);
  fprintf(stderr, "KODIChromium - %s", logMessage.c_str());
#endif
  kodi::Log(loglevel, buffer);
}
