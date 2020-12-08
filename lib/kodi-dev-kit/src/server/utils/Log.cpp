/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Log.h"

#include <kodi/General.h>
#include <kodi/tools/StringUtils.h>

using kodi::tools::StringUtils;

#define TEST_BUILD

namespace kodi
{
namespace utils
{

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

void LOG_MESSAGE(const int loglevel, const char* format, ...)
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
//   kodi::Log(loglevel, buffer);
}

} /* namespace utils */
} /* namespace kodi */
