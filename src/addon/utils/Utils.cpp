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

#include "Utils.h"
#include "platform/util/StringUtils.h"

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

void LOG_INTERNAL_MESSAGE(const ADDON::addon_log_t loglevel, const char *format, ...)
{
  if (logCount == 0)
  {
    logFile = g_strLogPath + "kodi-chromium.log";
    if (KODI->FileExists(logFile.c_str(), false))
    {
      char buffer[16384];
      void* oldFile = KODI->OpenFile(logFile.c_str(), 0);
      void* backupFile = KODI->OpenFileForWrite(std::string(g_strLogPath + "kodi-chromium.old.log").c_str(), true);
      if (oldFile)
      {
        ssize_t fileSize = KODI->ReadFile(oldFile, &buffer, sizeof(buffer));
        KODI->CloseFile(oldFile);
        KODI->DeleteFile(logFile.c_str());

        if (backupFile)
        {
          if (fileSize > 0)
            KODI->WriteFile(backupFile, &buffer, fileSize);
          KODI->CloseFile(backupFile);
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
  va_end(args);

  void* file = KODI->OpenFileForWrite(logFile.c_str(), false);
  if (file)
  {
    KODI->SeekFile(file, KODI->GetFileLength(file), 0);
    KODI->WriteFile(file, logMessage.c_str(), logMessage.size());
    KODI->CloseFile(file);
  }
}
