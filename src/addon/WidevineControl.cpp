/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "WidevineControl.h"

#include "utils/StringUtils.h"

#include <kodi/Filesystem.h>
#include <kodi/General.h>
#include <kodi/gui/dialogs/FileBrowser.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/tools/DllHelper.h>

#include "include/cef_app.h"
#include "include/cef_version.h"
#include "include/cef_web_plugin.h"

// prevent the use of Windows Macros for file edit (are in conflict with Kodi's one)
#ifdef WIN32
#undef CreateDirectory
#undef CopyFile
#endif

namespace WidevineControl
{

class CRegisterCdmCallback : public CefRegisterCdmCallback
{
public:
  void OnCdmRegistrationComplete(cef_cdm_registration_error_t result,
                                 const CefString& error_message) override
  {
    if (result == CEF_CDM_REGISTRATION_ERROR_NONE)
      kodi::Log(ADDON_LOG_INFO, "Chromium widevine registration successfully");
    else
      kodi::Log(ADDON_LOG_ERROR, "Chromium widevine registration failed with '%s'",
                error_message.ToString().c_str());
  }

private:
  IMPLEMENT_REFCOUNTING(CRegisterCdmCallback);
};

inline bool ends_with(std::string const& value, std::string const& ending)
{
  if (ending.size() > value.size())
    return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool CreateManifestAndInstall()
{
  if (!kodi::GetSettingBoolean("system.usewidevine"))
  {
    kodi::Log(
        ADDON_LOG_INFO,
        "Chromium widevine not present or disabled and ignored (Some streams are not supported!)");
    return false;
  }

  /*
   * Check widevine already present for use on browser.
   */
  std::string addonPath = kodi::GetBaseUserPath("widevine/");
  if (kodi::vfs::FileExists(addonPath + WIDEVINE_LIB) &&
      kodi::vfs::FileExists(addonPath + "widevine/manifest.json"))
  {
    kodi::Log(ADDON_LOG_INFO, "Chromium widevine already present under '%s'", addonPath.c_str());
    return true;
  }

  /*
   * If not present for this addon check is present by inputstream.adaptive and
   * script.module.inputstreamhelper.
   *
   * If yes copy it to browser (independent copy to prevent change problems).
   *
   * If it was not found becomes user asked to get a path too.
   */
  std::string path;
  if (kodi::vfs::FileExists("special://home/cdm/" WIDEVINE_LIB))
  {
    kodi::Log(ADDON_LOG_INFO, "Found widevine from inputstream.adaptive and becomes used");
    path = "special://home/cdm/" WIDEVINE_LIB;
  }
  else
  {
    bool canceled = true;
    // Retry if Yes/No is canceled
    while (canceled)
    {
      kodi::gui::dialogs::FileBrowser::ShowAndGetFile(
          "local", WIDEVINE_LIB,
          StringUtils::Format(kodi::GetLocalizedString(30196).c_str(), WIDEVINE_LIB), path, true);
      if (!path.empty())
        break;

      if (!kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30197),
                                                      kodi::GetLocalizedString(30198), canceled))
      {
        if (!canceled)
          kodi::SetSettingBoolean("system.usewidevine", false);
      }
    }

    if (!ends_with(path, WIDEVINE_LIB))
    {
      kodi::Log(ADDON_LOG_ERROR, "Selected Widevine library '%s' seems not correct", path.c_str());
      return false;
    }

    if (!kodi::vfs::FileExists(path))
    {
      kodi::Log(ADDON_LOG_ERROR, "Widevine file path '%s' does not exists", path.c_str());
      return false;
    }
  }

  // Get cdm version from selected library
  kodi::tools::CDllHelper dllHelper;

  if (!dllHelper.LoadDll(kodi::vfs::TranslateSpecialProtocol(path)))
  {
    kodi::Log(ADDON_LOG_ERROR, "Widevine library failed to load '%s'", dlerror());
    return false;
  }

  const char* (*get_cdm_version)();
  if (!dllHelper.RegisterSymbol(get_cdm_version, "GetCdmVersion"))
  {
    kodi::Log(ADDON_LOG_ERROR, "Widevine library failed to get version call '%s'", dlerror());
    return false;
  }

  std::string manifest =
      StringUtils::Format("{\n"
                          "  \"manifest_version\": 2,\n"
                          "  \"name\": \"WidevineCdm\",\n"
                          "  \"description\": \"Widevine Content Decryption Module Stub\",\n"
                          "  \"offline_enabled\": false,\n"
                          "  \"version\": \"%s\",\n"
                          "  \"minimum_chrome_version\": \"%i.%i.%i.%i\",\n"
                          "  \"os\": \"%s\",\n"
                          "  \"arch\": \"%s\",\n"
                          "  \"x-cdm-module-versions\": \"4\",\n"
                          "  \"x-cdm-interface-versions\": \"10\",\n"
                          "  \"x-cdm-host-versions\": \"10\",\n"
                          "  \"x-cdm-codecs\": \"vp8,vp9.0,avc1,av01\"\n"
                          "}\n",
                          get_cdm_version(), CHROME_VERSION_MAJOR, CHROME_VERSION_MINOR,
                          CHROME_VERSION_BUILD, CHROME_VERSION_PATCH, WIDEVINE_OS, WIDEVINE_CPU);

  // Create directory if not exists
  kodi::vfs::CreateDirectory(addonPath);

  kodi::vfs::CFile manifestFile;
  std::string file = addonPath + "/manifest.json";
  manifestFile.OpenFileForWrite(file, true);
  manifestFile.Write(manifest.c_str(), manifest.size());

  kodi::Log(ADDON_LOG_INFO, "Written chromium widevine manifest to %s", file.c_str());

  // Copy selected library to Kodi's user addon folder
  if (!kodi::vfs::CopyFile(path, addonPath + "/" + WIDEVINE_LIB))
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to copy '%s' widevine library to '%s'", path.c_str(),
              addonPath.c_str());
    return false;
  }

  kodi::Log(ADDON_LOG_INFO, "Copied chromium widevine library to %s", addonPath.c_str());

  return true;
}

bool InitializeWidevine()
{
  if (!CreateManifestAndInstall())
    return false;

  CefRegisterWidevineCdm(kodi::GetBaseUserPath("widevine"), new CRegisterCdmCallback);
  return true;
}

} /* namespace WidevineControl */
