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

#include <kodi/Filesystem.h>

#include "addon.h"
#include "Utils.h"
#include "p8-platform/util/StringUtils.h"

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

void LOG_INTERNAL_MESSAGE(const AddonLog loglevel, const char *format, ...)
{
  if (logCount == 0)
  {
    logFile = kodi::GetBaseUserPath("kodi-chromium.log");
    LOG_MESSAGE(ADDON_LOG_NOTICE, "Own add-on log file becomes stored at '%s'", logFile.c_str());
    if (kodi::vfs::FileExists(logFile, false))
    {
      char buffer[16384];
      kodi::vfs::CFile file;

      bool ret = file.OpenFile(logFile, 0);
      if (ret)
      {
        ssize_t fileSize = file.Read(&buffer, sizeof(buffer));
        file.Close();
        kodi::vfs::DeleteFile(logFile);

        ret = file.OpenFileForWrite(kodi::GetBaseUserPath("kodi-chromium.old.log"), true);
        if (ret)
        {
          if (fileSize > 0)
            file.Write(&buffer, fileSize);
        }
      }
    }
  }

  time_t rawtime;
  time(&rawtime);
  std::string time = ctime(&rawtime);
  StringUtils::RemoveCRLF(time);

  va_list args;
  va_start(args, format);
  std::string logMessage = StringUtils::Format("%s - %05i - %7s: %s\n", time.c_str(), logCount++, levelNames[loglevel], StringUtils::FormatV(format, args).c_str());
#ifdef TEST_BUILD
  fprintf(stderr, "%s", logMessage.c_str());
#endif // TEST_BUILD
  va_end(args);

  kodi::vfs::CFile file;
  if (file.OpenFileForWrite(logFile, false))
  {
    file.Seek(file.GetLength(), 0);
    file.Write(logMessage.c_str(), logMessage.size());
  }
}
