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
#include "DOMVisitor.h"
#include "MessageIds.h"
#include "RequestContextHandler.h"
#include "WebBrowserClient.h"
#include "Utils.h"
#include "webApp/RenderProcess.h"

#include "include/cef_app.h"
#include "include/cef_version.h"
#include "include/wrapper/cef_library_loader.h"

#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/gui/dialogs/Keyboard.h>
#include <kodi/gui/dialogs/FileBrowser.h>
#include <p8-platform/util/StringUtils.h>

int CWebBrowser::m_iUniqueClientId = 0;

CWebBrowser::CWebBrowser(KODI_HANDLE instance)
  : CInstanceWeb(instance),
    m_guiManager(this),
//     m_uploadHandler(new CWebBrowserUploadHandler),
    m_isActive(false)
{
  kodi::Log(ADDON_LOG_DEBUG, "%s - Creating the Google Chromium Internet Browser add-on", __FUNCTION__);

  std::string cefLib = kodi::GetAddonPath(LIBRARY_PREFIX "cef" LIBRARY_SUFFIX);
  if (!cef_load_library(cefLib.c_str()))
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Failed to load CEF library '%s'", __FUNCTION__, cefLib.c_str());
    return;
  }

//
//   CefMessageRouterConfig config;
//   m_messageRouter = CefMessageRouterRendererSide::Create(config);
}

CWebBrowser::~CWebBrowser()
{
  StopInstance();

//   m_messageRouter = nullptr;
  if (!cef_unload_library())
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Failed to unload CEF library", __FUNCTION__);
    return;
  }
}

WEB_ADDON_ERROR CWebBrowser::GetCapabilities(WEB_ADDON_CAPABILITIES& capabilities)
{
  capabilities.bSupportsWeb       = true;
  capabilities.bSupportsMail      = false;
  capabilities.bSupportsMessenger = false;
  capabilities.bSupportsVarious   = false;
  return WEB_ADDON_ERROR_NO_ERROR;
}

bool CWebBrowser::MainInitialize()
{
  if (kodi::GetSettingString("downloads.path").empty())
  {
    kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30080), kodi::GetLocalizedString(30081));

    std::string path;
    while (path.empty() && !IsStopped())
      kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory("local", kodi::GetLocalizedString(30081), path, true);

    if (IsStopped())
      return false;

    kodi::SetSettingString("downloads.path", path);
  }

  const char* cmdLine[3];
  cmdLine[0] = std::string("--kodi-addon-path=" + kodi::GetAddonPath()).c_str();
  cmdLine[1] = "--disable-gpu";
  cmdLine[2] = "--disable-software-rasterizer";
  CefMainArgs args(3, (char**)cmdLine);
  m_app = new CClientAppBrowser(this);
  if (!CefInitialize(args, m_cefSettings, m_app, nullptr))
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser start failed", __FUNCTION__);
    return false;
  }

  return true;
}

void CWebBrowser::MainShutdown()
{

  P8PLATFORM::CLockObject lock(m_mutex);

  /* delete all clients outside of CefDoMessageLoopWork */
  for (const auto& entry : m_browserClientsToDelete)
  {
    if (entry->CloseComplete())
      CefDoMessageLoopWork();
  }
  m_browserClientsToDelete.clear();
}

void CWebBrowser::MainLoop()
{

  if (!m_isActive)
  {
    MainInitialize();
    m_isActive = true;
  }

  CefDoMessageLoopWork();

  MainShutdown();
}

WEB_ADDON_ERROR CWebBrowser::StartInstance()
{
  std::string path;

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Setup add-on CEF directories:");

  path = UserPath();
  m_strHTMLCachePath = path + "pchHTMLCache";
  m_strCookiePath = path + "pchCookies";

  path = kodi::GetAddonPath();
  m_strLibPath = path + "kodichromium";
  m_strSandboxBinary = path + "chrome-sandbox";

  path = AddonSharePath();
  fprintf(stderr, "AddonSharePath %s\n", path.c_str());
  m_strLocalesPath = path + "resources/cef/locales";
  m_strResourcesPath = path + "resources/cef/";

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strHTMLCacheDir  %s", m_strHTMLCachePath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strCookiePath    %s", m_strCookiePath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strLocalesPath   %s", m_strLocalesPath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strResourcesPath %s", m_strResourcesPath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strLibPath       %s", m_strLibPath.c_str());

  if (!SetSandbox())
    return WEB_ADDON_ERROR_FAILED;

  std::string language = kodi::GetLanguage(LANG_FMT_ISO_639_1, true);

  m_cefSettings.no_sandbox                          = 0;
  CefString(&m_cefSettings.browser_subprocess_path) = m_strLibPath;
  CefString(&m_cefSettings.framework_dir_path)      = "";
  m_cefSettings.multi_threaded_message_loop         = 0;
  m_cefSettings.external_message_pump               = 1;
  m_cefSettings.windowless_rendering_enabled        = 1;
  m_cefSettings.command_line_args_disabled          = 0;
  CefString(&m_cefSettings.cache_path)              = m_strHTMLCachePath;
  CefString(&m_cefSettings.user_data_path)          = "";
  m_cefSettings.persist_session_cookies             = 0;
  m_cefSettings.persist_user_preferences            = 0;
  CefString(&m_cefSettings.user_agent)              = StringUtils::Format("Chrome/%d.%d.%d.%d",
                                                                          CHROME_VERSION_MAJOR, CHROME_VERSION_MINOR,
                                                                          CHROME_VERSION_BUILD, CHROME_VERSION_PATCH);
  CefString(&m_cefSettings.product_version)         = "KODI";
  CefString(&m_cefSettings.locale)                  = language;
  CefString(&m_cefSettings.log_file)                = "";
  m_cefSettings.log_severity                        = static_cast<cef_log_severity_t>(kodi::GetSettingInt("system.loglevelcef"));
  CefString(&m_cefSettings.javascript_flags)        = "";
  CefString(&m_cefSettings.resources_dir_path)      = m_strResourcesPath;
  CefString(&m_cefSettings.locales_dir_path)        = m_strLocalesPath;
  m_cefSettings.pack_loading_disabled               = 0;
  m_cefSettings.remote_debugging_port               = 8457;
  m_cefSettings.uncaught_exception_stack_size       = 0;
  m_cefSettings.ignore_certificate_errors           = 0;
  m_cefSettings.enable_net_security_expiration      = 0;
  m_cefSettings.background_color                    = 0;
  CefString(&m_cefSettings.accept_language_list)    = language;
  CefString(&m_cefSettings.kodi_addon_dir_path)     = path;

//   m_renderProcess = new CRenderProcess(m_cefSettings);
//   CreateThread();

  LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Started web browser add-on process", __FUNCTION__);

  return WEB_ADDON_ERROR_NO_ERROR;
}

void CWebBrowser::StopInstance()
{
//   StopThread();
}

bool CWebBrowser::SetLanguage(const char *language)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser language set to '%s'", __FUNCTION__, language);
  return true;
}

kodi::addon::CWebControl* CWebBrowser::CreateControl(const std::string& sourceName, const std::string& startURL, KODI_HANDLE handle)
{
//   CEF_REQUIRE_UI_THREAD();

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control creation started", __FUNCTION__);

  CWebBrowserClient *pBrowserClient;

  P8PLATFORM::CLockObject lock(m_mutex);

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
    settings.webgl                              = STATE_DISABLED;//STATE_DISABLED;//STATE_ENABLED
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

  P8PLATFORM::CLockObject lock(m_mutex);

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

void* CWebBrowser::Process(void)
{
//   DCHECK(m_threadChecker.CalledOnValidThread());
//   if (kodi::GetSettingString("downloads.path").empty())
//   {
//     kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30080), kodi::GetLocalizedString(30081));
//
//     std::string path;
//     while (path.empty() && !IsStopped())
//       kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory("local", kodi::GetLocalizedString(30081), path, true);
//
//     if (IsStopped())
//       return nullptr;
//
//     kodi::SetSettingString("downloads.path", path);
//   }
//
//
//   m_app = new CClientAppBrowser(this);
//
//   std::string commandLine1 = "--kodi-addon-path=" + kodi::GetAddonPath();
//   std::string commandLine2 = "--disable-gpu";
//   std::string commandLine3 = "--disable-software-rasterizer";
//   const char* cmdLine[3];
//   cmdLine[0] = std::string("--kodi-addon-path=" + kodi::GetAddonPath()).c_str();
//   cmdLine[1] = "--disable-gpu";
//   cmdLine[2] = "--disable-software-rasterizer";
//   CefMainArgs args(3, (char**)cmdLine);
//   if (!CefInitialize(args, m_cefSettings, m_app, nullptr))
//   {
//     LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser start failed", __FUNCTION__);
//     return nullptr;
//   }
//
//   m_isActive = true;
//
//   while (!IsStopped())
//   {
//     /*!
//      * Do Chromium related works, also CefRunMessageLoop() can be used and the
//      * thread is complete moved then there.
//      */
//     CefDoMessageLoopWork();
//
//     {
//       P8PLATFORM::CLockObject lock(m_mutex);
//
//       /* delete all clients outside of CefDoMessageLoopWork */
//       for (const auto& entry : m_browserClientsToDelete)
//       {
//         if (entry->CloseComplete())
//           CefDoMessageLoopWork();
//       }
//       m_browserClientsToDelete.clear();
//     }
//     usleep(100);
//   }
//
//   m_isActive = false;
//
//   CefDoMessageLoopWork();
//   CefShutdown();

  return nullptr;
}

/*!
 * @note and @todo
 *
 * Function is initial version and support currently only 'sudo' which present on ubuntu.
 * The super user way must be added with a better solution. Primary Idea is to add
 * related support to Kodi itself to allow also for other add-ons used with Linux and to
 * support it on all distribution types.
 *
 * Also need this easy sudo way to be tested on Mac OS X where it is also present.
 */
bool CWebBrowser::SetSandbox()
{
  struct stat st;
  if (m_strSandboxBinary.empty() || stat(m_strSandboxBinary.c_str(), &st) != 0)
  {
    kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30000), kodi::GetLocalizedString(30001));
    LOG_MESSAGE(ADDON_LOG_ERROR, "Web browser sandbox binary missing, add-on not usable!");
    return false;
  }

  bool bCanceled;
  if (access(m_strSandboxBinary.c_str(), X_OK) != 0 || (st.st_uid != 0) ||
      ((st.st_mode & S_ISUID) == 0) || ((st.st_mode & S_IXOTH)) == 0)
  {
    if (!kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30000), kodi::GetLocalizedString(30002), bCanceled))
    {
      return false;
    }

    std::string command;
    std::string strPassword;
    for (int i = 0; i < 3; i++)
    {
      if (kodi::gui::dialogs::Keyboard::ShowAndGetNewPassword(strPassword, kodi::GetLocalizedString(30003), true))
      {
        if (stat("/usr/bin/sudo", &st) == 0)
        {
          command = StringUtils::Format("echo %s | sudo -S bash -c \"chown root:root %s; sudo -- chmod 4755 %s\"",
                                            strPassword.c_str(),
                                            m_strSandboxBinary.c_str(),
                                            m_strSandboxBinary.c_str());
        }
        else
        {
          LOG_MESSAGE(ADDON_LOG_ERROR, "No super user application found to change chrome-sandbox rights!");
          break;
        }
        if (system(command.c_str()) == 0)
          return true;
      }
      else
        break;
    }
    return false;
  }
  return true;
}

//
// void CWebBrowser::OpenDownloadDialog()
// {
//   m_downloadHandler.Show();
// }
//
// void CWebBrowser::OpenCookieHandler()
// {
//   m_cookieHandler.Open();
// }

//------------------------------------------------------------------------------

class ATTRIBUTE_HIDDEN CMyAddon : public kodi::addon::CAddonBase
{
public:
  CMyAddon() { }
  virtual ADDON_STATUS CreateInstance(int instanceType, std::string instanceID, KODI_HANDLE instance, KODI_HANDLE& addonInstance) override
  {
    addonInstance = new CWebBrowser(instance);
    return ADDON_STATUS_OK;
  }
};

ADDONCREATOR(CMyAddon)
