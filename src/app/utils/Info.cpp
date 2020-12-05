/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Info.h"

// CEF
#include "include/cef_api_hash.h"
#include "include/cef_path_util.h"
#include "include/cef_version.h"

// Dev-kit
#include "../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

using kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace utils
{

void PrintCEFInfos(int argc, char* argv[], bool isChild)
{
  bool ret;
  CefString path;

  if (isChild)
  {
    for (int i = 0; i < argc; ++i)
      fprintf(stderr, "--- Cmd: %s\n", argv[i]);
    return;
  }

  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF version:        %i.%i.%i-%i (%s)", cef_version_info(0),
            cef_version_info(1), cef_version_info(2), cef_version_info(3), cef_api_hash(2));

  kodi::Log(ADDON_LOG_DEBUG, "INFO: Chrome version:     %i.%i.%i-%i", cef_version_info(4),
            cef_version_info(5), cef_version_info(6), cef_version_info(7));

  ret = CefGetPath(PK_DIR_CURRENT, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF dir current:    %s", path.ToString().c_str(),
            ret ? "" : "not available");
  path.clear();

  ret = CefGetPath(PK_DIR_EXE, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF dir exe:        %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();

  ret = CefGetPath(PK_DIR_MODULE, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF dir module:     %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();

  ret = CefGetPath(PK_DIR_TEMP, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF dir temp:       %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();

  ret = CefGetPath(PK_FILE_EXE, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF file exe:       %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();

  ret = CefGetPath(PK_FILE_MODULE, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF file module:    %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();

  ret = CefGetPath(PK_LOCAL_APP_DATA, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF local app data: %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();

  ret = CefGetPath(PK_USER_DATA, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF user data:      %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();

  ret = CefGetPath(PK_DIR_RESOURCES, path);
  kodi::Log(ADDON_LOG_DEBUG, "INFO: CEF dir resources:  %s",
            ret ? path.ToString().c_str() : "not available");
  path.clear();
}

bool IsImageURL(std::string url)
{
  std::size_t found = url.find_last_of("?");
  if (found != std::string::npos)
  {
    url.erase(url.begin()+found, url.end());
  }

  static const std::vector<std::string> extList =
  {
    ".bmp",
    ".jpg",
    ".jpeg",
    ".ico",
    ".png",
    ".gif",
    ".tif",
    ".tiff",
    ".raw",
    ".cr2",
    ".nef",
    ".orf",
    ".sr2",
    ".eps",
    ".svg"
  };

  for (const auto& ext : extList)
  {
    if (StringUtils::EndsWithNoCase(url, ext))
      return true;
  }

  return false;
}

} /* namespace utils */
} /* namespace app */
} /* namespace chromium */
