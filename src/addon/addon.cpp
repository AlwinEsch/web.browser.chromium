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

#include "addon.h"

#include "DOMVisitor.h"
#include "WebBrowserClient.h"
#include "Utils.h"
#include "JSInterface/V8Handler.h"

#include "include/cef_app.h"
#include "include/cef_version.h"

#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/gui/dialogs/Keyboard.h>
#include <kodi/gui/dialogs/FileBrowser.h>
#include <p8-platform/util/StringUtils.h>
#include "MessageIds.h"

int CWebBrowser::m_iUniqueClientId = 0;

CWebBrowser::CWebBrowser(KODI_HANDLE instance)
  : CInstanceWeb(instance),
    m_downloadHandler(new CWebBrowserDownloadHandler),
    m_uploadHandler(new CWebBrowserUploadHandler),
    m_geolocationPermission(new CWebBrowserGeolocationPermission),
    m_isActive(false)
{
  kodi::Log(ADDON_LOG_DEBUG, "%s - Creating the Google Chromium Internet Browser add-on", __FUNCTION__);

  CefMessageRouterConfig config;
  m_messageRouter = CefMessageRouterRendererSide::Create(config);

  DCHECK(m_threadChecker.CalledOnValidThread());
}

CWebBrowser::~CWebBrowser()
{
  StopInstance();
}

WEB_ADDON_ERROR CWebBrowser::GetCapabilities(WEB_ADDON_CAPABILITIES& capabilities)
{
  capabilities.bSupportsWeb       = true;
  capabilities.bSupportsMail      = false;
  capabilities.bSupportsMessenger = false;
  capabilities.bSupportsVarious   = false;
  return WEB_ADDON_ERROR_NO_ERROR;
}

WEB_ADDON_ERROR CWebBrowser::StartInstance()
{
  std::string path;

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Setup add-on CEF directories:");

  path = UserPath();
  m_strHTMLCachePath = path + "pchHTMLCache";
  m_strCookiePath = path + "pchCookies";

  path = AddonLibPath();
  m_strLibPath = path + "kodichromium";
  m_strSandboxBinary = path + "chrome-sandbox";

  path = AddonSharePath();
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

  m_cefSettings.single_process                      = 1;
  m_cefSettings.no_sandbox                          = 0;
  CefString(&m_cefSettings.browser_subprocess_path) = m_strLibPath;
  CefString(&m_cefSettings.framework_dir_path)      = "";
  m_cefSettings.multi_threaded_message_loop         = 0;
  m_cefSettings.external_message_pump               = 0;
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
  m_cefSettings.remote_debugging_port               = 0;
  m_cefSettings.uncaught_exception_stack_size       = 0;
  m_cefSettings.ignore_certificate_errors           = 0;
  m_cefSettings.enable_net_security_expiration      = 0;
  m_cefSettings.background_color                    = 0;
  CefString(&m_cefSettings.accept_language_list)    = language;
  CefString(&m_cefSettings.kodi_addon_dir_path)     = m_strLibPath;

  CreateThread();

  LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Started web browser add-on process", __FUNCTION__);

  return WEB_ADDON_ERROR_NO_ERROR;
}

void CWebBrowser::StopInstance()
{
  StopThread();
}

bool CWebBrowser::SetLanguage(const char *language)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser language set to '%s'", __FUNCTION__, language);
  return true;
}

kodi::addon::CWebControl* CWebBrowser::CreateControl(const std::string& sourceName, const std::string& webType, KODI_HANDLE handle)
{
  /*!
   * Paranoia ;-), prevent not wanted creation calls (normally not done)
   */
  if (webType != "browser" || handle == nullptr)
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Called for not supported web type %i", __FUNCTION__, webType.c_str());
    return nullptr;
  }

  CEF_REQUIRE_UI_THREAD();

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control creation started", __FUNCTION__);

  CWebBrowserClient *pBrowserClient;

  P8PLATFORM::CLockObject lock(m_Mutex);

  std::map<std::string, CWebBrowserClient*>::iterator itr = m_BrowserClientsInactive.find(sourceName);
  if (itr != m_BrowserClientsInactive.end())
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Found control in inactive mode and setting active", __FUNCTION__);
    pBrowserClient = itr->second;
    pBrowserClient->SetActive();
    m_BrowserClientsInactive.erase(itr);
  }
  else
  {
    pBrowserClient = new CWebBrowserClient(handle, m_iUniqueClientId++, this);

    CefWindowInfo info;
    info.SetAsWindowless(kNullWindowHandle);

    CefBrowserSettings settings;
    settings.windowless_frame_rate              = 0;
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
    settings.webgl                              = STATE_DISABLED;//STATE_ENABLED
    settings.background_color                   = 0;
    CefString(&settings.accept_language_list)   = "";

    if (!CefBrowserHost::CreateBrowser(info, pBrowserClient, "", settings, nullptr))
    {
      LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser creation failed", __FUNCTION__);
      if (pBrowserClient)
        delete pBrowserClient;
      return nullptr;
    }
  }

  int uniqueId = pBrowserClient->GetUniqueId();
  m_BrowserClients[uniqueId] = pBrowserClient;

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control created", __FUNCTION__);
  return pBrowserClient;
}

bool CWebBrowser::DestroyControl(kodi::addon::CWebControl* control)
{
  //! Check for wrongly passed empty handle.
  CWebBrowserClient* browserClient = static_cast<CWebBrowserClient*>(control);
  if (browserClient == nullptr)
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser control destroy called without handle!", __FUNCTION__);
    return false;
  }

  P8PLATFORM::CLockObject lock(m_Mutex);

  //! Find wanted control to destroy.
  std::map<int, CWebBrowserClient*>::iterator itr = m_BrowserClients.find(browserClient->GetDataIdentifier());
  if (itr == m_BrowserClients.end())
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser control destroy called for invalid id '%i'",
                                            __FUNCTION__, browserClient->GetDataIdentifier());
    return false;
  }

  m_BrowserClientsInactive[browserClient->GetName()] = browserClient;
  m_BrowserClients.erase(itr);

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control destroyed", __FUNCTION__);
  return true;
}

void* CWebBrowser::Process(void)
{
  DCHECK(m_threadChecker.CalledOnValidThread());

  if (kodi::GetSettingString("downloads.path").empty())
  {
    kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30080), kodi::GetLocalizedString(30081));

    std::string path;
    while (path.empty() && !IsStopped())
      kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory("local", kodi::GetLocalizedString(30081), path, true);
    if (IsStopped())
      return nullptr;

    kodi::SetSettingString("downloads.path", path);
  }

  CefMainArgs args;
  if (!CefInitialize(args, m_cefSettings, this, nullptr))
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser start failed", __FUNCTION__);
    return nullptr;
  }

  m_isActive = true;

  while (!IsStopped())
  {
    /*!
     * Do Chromium related works, also CefRunMessageLoop() can be used and the
     * thread is complete moved then there.
     */
    CefDoMessageLoopWork();

    /*!
     * Handle currently inactive controls and if timeout is reached delete
     * them.
     */
    {
      P8PLATFORM::CLockObject lock(m_Mutex);

      std::map<std::string, CWebBrowserClient*>::iterator itr;
      for (itr = m_BrowserClientsInactive.begin(); itr != m_BrowserClientsInactive.end(); ++itr)
      {
        if (itr->second->CurrentInactiveCountdown() < 0)
        {
          LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Web browser control inactive countdown reached end and closed", __FUNCTION__);
          delete itr->second;
          m_BrowserClientsInactive.erase(itr);
        }
      }
    }
    usleep(100);
  }

  m_isActive = false;

  CefDoMessageLoopWork();
  CefShutdown();

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

/// CefResourceBundleHandler
//@{
bool CWebBrowser::GetLocalizedString(int string_id, CefString& string)
{
  return false;
}

bool CWebBrowser::GetDataResource(int resource_id, void*& data, size_t& data_size)
{
  return false;
}

bool CWebBrowser::GetDataResourceForScale(int resource_id, ScaleFactor scale_factor, void*& data, size_t& data_size)
{
  return false;
}
//@}

/// CefBrowserProcessHandler
//@{
void CWebBrowser::OnContextInitialized()
{
  //m_cookieHandler.Open();
  // Register cookieable schemes with the global cookie manager.
//   CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
//   manager->SetSupportedSchemes(m_cookieable_schemes, nullptr);
//   manager->


  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowser::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowser::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
}

CefRefPtr<CefPrintHandler> CWebBrowser::GetPrintHandler()
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
  return nullptr;
}

void CWebBrowser::OnScheduleMessagePumpWork(int64 delay_ms)
{
  fprintf(stderr, "--- %s resource_id %li\n", __PRETTY_FUNCTION__, delay_ms);
}
//@}

/// CefRenderProcessHandler
//@{
void CWebBrowser::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowser::OnWebKitInitialized()
{
  CV8Handler::OnWebKitInitialized(this);
}

void CWebBrowser::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowser::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
}

bool CWebBrowser::OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefRequest> request,
                                  NavigationType navigation_type,
                                  bool is_redirect)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
  return false;
}

void CWebBrowser::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
  m_messageRouter->OnContextCreated(browser, frame, context);
}

void CWebBrowser::OnContextReleased(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
  m_messageRouter->OnContextReleased(browser, frame, context);
}

void CWebBrowser::OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefV8Context> context,
                                   CefRefPtr<CefV8Exception> exception,
                                   CefRefPtr<CefV8StackTrace> stackTrace)
{
  fprintf(stderr, "--- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowser::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefDOMNode> node)
{

  browser->GetFocusedFrame()->VisitDOM(new CDOMVisitor(browser));

  CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(RendererMessage::FocusedNodeChanged);
  if (node.get())
  {
    CefRect bounds = node->GetElementBounds();
    std::string name = node->GetName();
    bool editableForm = node->GetType() == 1 && (name == "SELECT" || name == "INPUT" || name == "TEXTAREA");

    message->GetArgumentList()->SetBool(0, editableForm);
    message->GetArgumentList()->SetBool(1, node->IsEditable());
    message->GetArgumentList()->SetInt(2, bounds.x);
    message->GetArgumentList()->SetInt(3, bounds.y);
    message->GetArgumentList()->SetInt(4, bounds.width);
    message->GetArgumentList()->SetInt(5, bounds.height);
    message->GetArgumentList()->SetString(6, node->GetName());
    message->GetArgumentList()->SetString(7, node->GetValue());
//     message->GetArgumentList()->SetString(3, node->GetFormControlElementType());
//     message->GetArgumentList()->SetInt(4, node->GetType());
//     message->GetArgumentList()->SetBool(5, node->IsEditable());
//     browser->SendProcessMessage(PID_BROWSER, message);
fprintf(stderr, "-------------------aaaaaaaaaaa- %s editableForm %i %i %i %i %i\n", __PRETTY_FUNCTION__, editableForm, bounds.x, bounds.y, bounds.width, bounds.height);
  }
  else
  {
    message->GetArgumentList()->SetBool(0, false);
    message->GetArgumentList()->SetBool(1, false);
    message->GetArgumentList()->SetInt(2, 0);
    message->GetArgumentList()->SetInt(3, 0);
    message->GetArgumentList()->SetInt(4, 0);
    message->GetArgumentList()->SetInt(5, 0);
    message->GetArgumentList()->SetString(6, "");
    message->GetArgumentList()->SetString(7, "");
    fprintf(stderr, "-------------------bbbbbbbbbbbb- %s\n", __PRETTY_FUNCTION__);
  }
  browser->SendProcessMessage(PID_BROWSER, message);
}

bool CWebBrowser::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message)
{
  fprintf(stderr, "--- %s message->GetName() %s\n", __PRETTY_FUNCTION__, message->GetName().ToString().c_str());
  if (m_messageRouter->OnProcessMessageReceived(browser, source_process, message))
    return true;

  std::string message_name = message->GetName();
  if (message_name == AddonClientMessage::FocusedSelected)
  {
    browser->GetMainFrame()->VisitDOM(new CDOMVisitor(browser));
    return true;
  }
  return false;
}
//@}

void CWebBrowser::OpenDownloadDialog()
{
  m_downloadHandler->Show();
}

void CWebBrowser::OpenCookieHandler()
{
  m_cookieHandler.Open();
}

//------------------------------------------------------------------------------

class CMyAddon : public kodi::addon::CAddonBase
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
