/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
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

#ifdef _WIN32                   // windows
#include "third_party/dlfcn-win32/dlfcn.h"
#else
#include <dlfcn.h>              // linux+osx
#endif

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
    void OnCdmRegistrationComplete(cef_cdm_registration_error_t result, const CefString& error_message) override
    {
      if (result == CEF_CDM_REGISTRATION_ERROR_NONE)
        kodi::Log(ADDON_LOG_INFO, "Chromium widevine registration successfully");
      else
        kodi::Log(ADDON_LOG_ERROR, "Chromium widevine registration failed with '%s'", error_message.ToString().c_str());
    }

  private:
    IMPLEMENT_REFCOUNTING(CRegisterCdmCallback);
  };

  inline bool ends_with(std::string const & value, std::string const & ending)
  {
    if (ending.size() > value.size()) 
      return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
  }

  bool CreateManifestAndInstall()
  {
    std::string addonPath = kodi::GetBaseUserPath("widevine/");

    if (kodi::vfs::FileExists(kodi::GetBaseUserPath("widevine/" WIDEVINE_LIB)) &&
        kodi::vfs::FileExists(kodi::GetBaseUserPath("widevine/manifest.json")))
    {
      kodi::Log(ADDON_LOG_INFO, "Chromium widevine already present under '%s'", addonPath.c_str());
      return true;
    }

    std::string path;
    if (kodi::GetSettingBoolean("system.widevine-path-asked"))
    {
      kodi::Log(ADDON_LOG_NOTICE, "Chromium widevine not present and ignored (Some streams are not supported!)");
      return false;
    }
    else
    {
      bool canceled = true;
      // Retry if Yes/No is canceled
      while (canceled)
      {
        kodi::gui::dialogs::FileBrowser::ShowAndGetFile("local", WIDEVINE_LIB,
                                                        StringUtils::Format(kodi::GetLocalizedString(30196).c_str(), WIDEVINE_LIB),
                                                        path, true);
        if (!path.empty())
          break;

        if (!kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30197), kodi::GetLocalizedString(30198), canceled))
        {
          if (!canceled)
            kodi::SetSettingBoolean("system.widevine-path-asked", true);
        }
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

    // Get cdm version from selected library
    void *handle;
    const char *(*get_cdm_version)();

    handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle)
    {
      kodi::Log(ADDON_LOG_ERROR, "Widevine library failed to load '%s'", dlerror());
      return false;
    }

    get_cdm_version = reinterpret_cast<const char *(*)()>(dlsym(handle, "GetCdmVersion"));
    if (!get_cdm_version)
    {
      kodi::Log(ADDON_LOG_ERROR, "Widevine library failed to get version call '%s'", dlerror());
      return false;
    }

    std::string manifest = StringUtils::Format("{\n"
    "  \"manifest_version\": 2,\n"
    "  \"name\": \"WidevineCdm\",\n"
    "  \"description\": \"Widevine Content Decryption Module Stub\",\n"
    "  \"offline_enabled\": false,\n"
    "  \"version\": \"%s\",\n"
    "  \"minimum_chrome_version\": \"%i.%i.%i.%i\",\n"
    "  \"os\": \"%s\",\n"
    "  \"arch\": \"%s\",\n"
    "  \"x-cdm-module-versions\": \"4\",\n"
    "  \"x-cdm-interface-versions\": \"9\",\n"
    "  \"x-cdm-host-versions\": \"9\",\n"
    "  \"x-cdm-codecs\": \"vp8,vp9.0,avc1\"\n"
    "}\n",
      get_cdm_version(),
      CHROME_VERSION_MAJOR,
      CHROME_VERSION_MINOR,
      CHROME_VERSION_BUILD,
      CHROME_VERSION_PATCH,
      WIDEVINE_OS,
      WIDEVINE_CPU);

    dlclose(handle);

    // Create directory if not exists
    kodi::vfs::CreateDirectory(addonPath);

    kodi::vfs::CFile manifestFile;
    std::string file = addonPath + "/manifest.json";
    manifestFile.OpenFileForWrite(file, true);
    manifestFile.Write(manifest.c_str(), manifest.size());

    kodi::Log(ADDON_LOG_NOTICE, "Written chromium widevine manifest to %s", file.c_str());

    // Copy selected library to Kodi's user addon folder
    if (!kodi::vfs::CopyFile(path, addonPath + "/" + WIDEVINE_LIB))
    {
      kodi::Log(ADDON_LOG_ERROR, "Failed to copy '%s' widevine library to '%s'", path.c_str(), addonPath.c_str());
      return false;
    }

    kodi::Log(ADDON_LOG_NOTICE, "Copied chromium widevine library to %s", addonPath.c_str());

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
