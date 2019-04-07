/*
 *      Copyright (C) 2015-2019 Team KODI
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

#include "addon.h"

#include "AppBrowser.h"
#include "MessageIds.h"
#include "RequestContextHandler.h"
#include "SandboxControl.h"
#include "WebBrowserClient.h"
#include "WidevineControl.h"
#include "utils/Utils.h"
#include "utils/StringUtils.h"

#include "include/cef_app.h"
#include "include/cef_version.h"
#include "include/wrapper/cef_library_loader.h"

#include <kodi/Filesystem.h>
#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/FileBrowser.h>

int CWebBrowser::m_iUniqueClientId = 0;

CWebBrowser::CWebBrowser()
  : m_guiManager(this),
    m_isActive(false)
{
}

WEB_ADDON_ERROR CWebBrowser::StartInstance()
{
  kodi::Log(ADDON_LOG_DEBUG, "%s - Creating the Google Chromium Internet Browser add-on", __FUNCTION__);

  // Load CEF library by self
  std::string cefLib = kodi::GetAddonPath(LIBRARY_PREFIX "cef" LIBRARY_SUFFIX);
  if (!cef_load_library(cefLib.c_str()))
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Failed to load CEF library '%s'", __FUNCTION__, cefLib.c_str());
    return WEB_ADDON_ERROR_FAILED;
  }

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Setup add-on CEF directories:");

  WidevineControl::InitializeWidevine();

  std::string path;

  path = kodi::GetBaseUserPath();
  m_strHTMLCachePath = path + "pchHTMLCache";
  m_strCookiePath = path + "pchCookies";

  path = kodi::GetAddonPath();
  m_strLibPath = path + "kodichromium";

  path = AddonSharePath();
  fprintf(stderr, "AddonSharePath %s\n", path.c_str());
  m_strLocalesPath = path + "resources/cef/locales";
  m_strResourcesPath = path + "resources/cef/";

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strHTMLCacheDir  %s", m_strHTMLCachePath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strCookiePath    %s", m_strCookiePath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strLocalesPath   %s", m_strLocalesPath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strResourcesPath %s", m_strResourcesPath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strLibPath       %s", m_strLibPath.c_str());

  if (!SandboxControl::SetSandbox())
    return WEB_ADDON_ERROR_FAILED;

  std::string language = kodi::GetLanguage(LANG_FMT_ISO_639_1, true);

  // Create and delete CefSettings itself, otherwise comes seqfault during
  // "CefSettingsTraits::clear" call, on
  m_cefSettings = new CefSettings;
  m_cefSettings->no_sandbox                          = 0;
  CefString(&m_cefSettings->browser_subprocess_path) = m_strLibPath;
  CefString(&m_cefSettings->framework_dir_path)      = "";
  m_cefSettings->multi_threaded_message_loop         = 0;
  m_cefSettings->external_message_pump               = 1;
  m_cefSettings->windowless_rendering_enabled        = 1;
  m_cefSettings->command_line_args_disabled          = 0;
  CefString(&m_cefSettings->cache_path)              = m_strHTMLCachePath;
  CefString(&m_cefSettings->user_data_path)          = "";
  m_cefSettings->persist_session_cookies             = 0;
  m_cefSettings->persist_user_preferences            = 0;
  CefString(&m_cefSettings->user_agent)              = StringUtils::Format("Chrome/%d.%d.%d.%d",
                                                                          CHROME_VERSION_MAJOR, CHROME_VERSION_MINOR,
                                                                          CHROME_VERSION_BUILD, CHROME_VERSION_PATCH);
  CefString(&m_cefSettings->product_version)         = "KODI";
  CefString(&m_cefSettings->locale)                  = language;
  CefString(&m_cefSettings->log_file)                = "";
  m_cefSettings->log_severity                        = static_cast<cef_log_severity_t>(kodi::GetSettingInt("system.loglevelcef"));
  CefString(&m_cefSettings->javascript_flags)        = "";
  CefString(&m_cefSettings->resources_dir_path)      = m_strResourcesPath;
  CefString(&m_cefSettings->locales_dir_path)        = m_strLocalesPath;
  m_cefSettings->pack_loading_disabled               = 0;
  m_cefSettings->remote_debugging_port               = 8457;
  m_cefSettings->uncaught_exception_stack_size       = 0;
  m_cefSettings->ignore_certificate_errors           = 0;
  m_cefSettings->enable_net_security_expiration      = 0;
  m_cefSettings->background_color                    = 0;
  CefString(&m_cefSettings->accept_language_list)    = language;
  CefString(&m_cefSettings->kodi_addon_dir_path)     = path;

  LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Started web browser add-on process", __FUNCTION__);

  return WEB_ADDON_ERROR_NO_ERROR;
}

void CWebBrowser::StopInstance()
{
  // deleted the created settings class
  m_cefSettings->Reset();
  delete m_cefSettings;
  m_cefSettings = nullptr;

  // unload cef library
  if (!cef_unload_library())
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Failed to unload CEF library", __FUNCTION__);
    return;
  }
}

bool CWebBrowser::MainInitialize()
{
  if (kodi::GetSettingString("downloads.path").empty())
  {
    kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30080), kodi::GetLocalizedString(30081));

    std::string path;
    while (path.empty())
      kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory("local", kodi::GetLocalizedString(30081), path, true);

    kodi::SetSettingString("downloads.path", path);
  }

  const char* cmdLine[3];
  cmdLine[0] = "";
  cmdLine[1] = "--disable-gpu";
  cmdLine[2] = "--disable-software-rasterizer";
  CefMainArgs args(3, (char**)cmdLine);
  m_app = new CClientAppBrowser(this);
  if (!CefInitialize(args, *m_cefSettings, m_app, nullptr))
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser start failed", __FUNCTION__);
    return false;
  }

  return true;
}

void CWebBrowser::MainLoop()
{
  // Do CEF's message loop work
  CefDoMessageLoopWork();

  // Clear opened browser where not used anymore
  if (!m_browserClientsToDelete.empty())
    ClearClosedBrowsers();
}

void CWebBrowser::MainShutdown()
{
  ClearClosedBrowsers();

  // Clear app browser and shutdown CEF
  m_app = nullptr;
  CefShutdown();
}

void CWebBrowser::ClearClosedBrowsers()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  /* delete all clients outside of CefDoMessageLoopWork */
  for (const auto& entry : m_browserClientsToDelete)
  {
    if (entry->CloseComplete())
      CefDoMessageLoopWork();
  }
  m_browserClientsToDelete.clear();
}

bool CWebBrowser::SetLanguage(const char *language)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser language set to '%s'", __FUNCTION__, language);
  return true;
}

kodi::addon::CWebControl* CWebBrowser::CreateControl(const std::string& sourceName, const std::string& startURL, KODI_HANDLE handle)
{
  CEF_REQUIRE_UI_THREAD();

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control creation started", __FUNCTION__);

  CWebBrowserClient *pBrowserClient;

  std::lock_guard<std::mutex> lock(m_mutex);

  std::unordered_map<std::string, CWebBrowserClient*>::iterator itr = m_browserClientsInactive.find(sourceName);
  if (itr != m_browserClientsInactive.end())
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Found control in inactive mode and setting active", __FUNCTION__);
    pBrowserClient = itr->second;
    pBrowserClient->SetActive();
    m_browserClientsInactive.erase(itr);
  }
  else
  {
    for (const auto& entry : m_browserClients)
    {
      if (entry.second->GetName() == sourceName)
      {
        return entry.second;
      }
    }

    pBrowserClient = new CWebBrowserClient(handle, m_iUniqueClientId++, startURL, this);

    CefWindowInfo info;
    info.SetAsWindowless(kNullWindowHandle);
    CefBrowserSettings settings;
    settings.windowless_frame_rate              = static_cast<int>(pBrowserClient->GetFPS());
    CefString(&settings.standard_font_family)   = "";
    CefString(&settings.fixed_font_family)      = "";
    CefString(&settings.serif_font_family)      = "";
    CefString(&settings.sans_serif_font_family) = "";
    CefString(&settings.cursive_font_family)    = "";
    CefString(&settings.fantasy_font_family)    = "";
    settings.default_font_size                  = 0;
    settings.default_fixed_font_size            = 0;
    settings.minimum_font_size                  = 0;
    settings.minimum_logical_font_size          = 0;
    CefString(&settings.default_encoding)       = "";
    settings.remote_fonts                       = STATE_DEFAULT;
    settings.javascript                         = STATE_ENABLED;
    settings.javascript_close_windows           = STATE_DEFAULT;
    settings.javascript_access_clipboard        = STATE_DEFAULT;
    settings.javascript_dom_paste               = STATE_DEFAULT;
    settings.plugins                            = STATE_ENABLED;
    settings.universal_access_from_file_urls    = STATE_DEFAULT;
    settings.file_access_from_file_urls         = STATE_DEFAULT;
    settings.web_security                       = STATE_DEFAULT;
    settings.image_loading                      = STATE_DEFAULT;
    settings.image_shrink_standalone_to_fit     = STATE_DEFAULT;
    settings.text_area_resize                   = STATE_DEFAULT;
    settings.tab_to_links                       = STATE_DEFAULT;
    settings.local_storage                      = STATE_DEFAULT;
    settings.databases                          = STATE_DEFAULT;
    settings.application_cache                  = STATE_DEFAULT;
    settings.webgl                              = STATE_ENABLED;//STATE_DISABLED;//STATE_ENABLED
    settings.background_color                   = 0;
    CefString(&settings.accept_language_list)   = "";

    ///TODO Change CefRequestContext::GetGlobalContext() to a use of own CefRequestContextSettings?
    CefRefPtr<CefRequestContext> request_context = CefRequestContext::CreateContext(CefRequestContext::GetGlobalContext(),
                                                                                    new CRequestContextHandler);
    if (!CefBrowserHost::CreateBrowser(info, pBrowserClient, "", settings, request_context))
    {
      LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser creation failed", __FUNCTION__);
      if (pBrowserClient)
      {
        delete pBrowserClient;
      }
      return nullptr;
    }
  }

  int uniqueId = pBrowserClient->GetUniqueId();
  m_browserClients.emplace(std::pair<int, CWebBrowserClient*>(uniqueId, pBrowserClient));
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control created", __FUNCTION__);
  return pBrowserClient;
}

bool CWebBrowser::DestroyControl(kodi::addon::CWebControl* control, bool complete)
{
  //! Check for wrongly passed empty handle.
  CWebBrowserClient* browserClient = static_cast<CWebBrowserClient*>(control);
  if (browserClient == nullptr)
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser control destroy called without handle!", __FUNCTION__);
    return false;
  }

  std::lock_guard<std::mutex> lock(m_mutex);

  const auto& itr = m_browserClients.find(browserClient->GetDataIdentifier());
  if (itr != m_browserClients.end())
    m_browserClients.erase(itr);
  browserClient->SetInactive();

  if (complete)
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control destroy complete", __FUNCTION__);
    const auto& inactiveClient = m_browserClientsInactive.find(browserClient->GetName());
    if (inactiveClient != m_browserClientsInactive.end())
      m_browserClientsInactive.erase(inactiveClient);
    m_browserClientsToDelete.push_back(browserClient);
    browserClient->DestroyRenderer();
  }
  else
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control destroy to set inactive", __FUNCTION__);
    if (itr == m_browserClients.end())
    {
      LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser control destroy called for invalid id '%i'",
                                              __FUNCTION__, browserClient->GetDataIdentifier());
      return false;
    }
    m_browserClientsInactive[browserClient->GetName()] = browserClient;
  }

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control destroy done", __FUNCTION__);
  return true;
}

ADDONCREATOR(CWebBrowser)
