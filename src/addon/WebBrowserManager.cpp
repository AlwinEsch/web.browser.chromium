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

#include <vector>

#include <kodi/General.h>
#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/gui/dialogs/Keyboard.h>

#include "p8-platform/util/StringUtils.h"
#include "include/cef_app.h"
#include "include/cef_version.h"
#include "include/wrapper/cef_helpers.h"

#include "addon.h"
#include "WebBrowserManager.h"
#include "Utils.h"

using namespace std;

class CWebBrowserApp : public CefApp,
                       public CefBrowserProcessHandler,
                       public CefRenderProcessHandler,
                       public CefResourceBundleHandler
{
public:
  CWebBrowserApp();
  virtual ~CWebBrowserApp();

  /*!
   * @brief CefApp methods
   *
   * Implement this interface to provide handler implementations. Methods will be
   * called by the process and/or thread indicated.
   */
  //{
  virtual void OnBeforeCommandLineProcessing(                      ///<--
      const CefString& process_type,                                  ///
      CefRefPtr<CefCommandLine> command_line)                         ///
                        OVERRIDE;                                     ///

  virtual void OnRegisterCustomSchemes(                            ///<--
      CefRefPtr<CefSchemeRegistrar> registrar)                        ///
                        OVERRIDE;                                     ///
  //}

  /*!
   * @brief CefBrowserProcessHandler methods
   */
  //{
  virtual void OnContextInitialized()                              ///<--
                        OVERRIDE;                                     ///
  //}

  /*!
   * @brief CefRenderProcessHandler methods
   *
   * Class used to implement render process callbacks. The methods of this class
   * will be called on the render process main thread (TID_RENDERER) unless
   * otherwise indicated.
   */
  //{
  virtual void OnRenderThreadCreated(                              ///<--
      CefRefPtr<CefListValue> extra_info)                             ///
                        OVERRIDE;                                     ///

  virtual void OnWebKitInitialized()                               ///<--
                        OVERRIDE;                                     ///

  virtual void OnBrowserCreated(                                   ///<--
      CefRefPtr<CefBrowser> browser)                                  ///
                        OVERRIDE;                                     ///

  virtual void OnBrowserDestroyed(                                 ///<--
      CefRefPtr<CefBrowser> browser)                                  ///
                        OVERRIDE;                                     ///

  virtual CefRefPtr<CefLoadHandler> GetLoadHandler()               ///<--
                        OVERRIDE;                                     ///

  virtual bool OnBeforeNavigation(                                 ///<--
      CefRefPtr<CefBrowser> browser,                                  ///
      CefRefPtr<CefFrame> frame,                                      ///
      CefRefPtr<CefRequest> request,                                  ///
      NavigationType navigation_type,                                 ///
      bool is_redirect)                                               ///
                        OVERRIDE;                                     ///

  virtual void OnContextCreated(                                   ///<--
      CefRefPtr<CefBrowser> browser,                                  ///
      CefRefPtr<CefFrame> frame,                                      ///
      CefRefPtr<CefV8Context> context)                                ///
                        OVERRIDE;                                     ///

  virtual void OnContextReleased(                                  ///<--
      CefRefPtr<CefBrowser> browser,                                  ///
      CefRefPtr<CefFrame> frame,                                      ///
      CefRefPtr<CefV8Context> context)                                ///
                        OVERRIDE;                                     ///

  virtual void OnUncaughtException(                                ///<--
      CefRefPtr<CefBrowser> browser,                                  ///
      CefRefPtr<CefFrame> frame,                                      ///
      CefRefPtr<CefV8Context> context,                                ///
      CefRefPtr<CefV8Exception> exception,                            ///
      CefRefPtr<CefV8StackTrace> stackTrace)                          ///
                        OVERRIDE;                                     ///

  virtual void OnFocusedNodeChanged(                               ///<--
      CefRefPtr<CefBrowser> browser,                                  ///
      CefRefPtr<CefFrame> frame,                                      ///
      CefRefPtr<CefDOMNode> node)                                     ///
                        OVERRIDE;                                     ///

  virtual bool OnProcessMessageReceived(                           ///<--
      CefRefPtr<CefBrowser> browser,                                  ///
      CefProcessId source_process,                                    ///
      CefRefPtr<CefProcessMessage> message)                           ///
                        OVERRIDE;                                     ///
  //}

  /*!
   * @brief CefResourceBundleHandler methods
   *
   * Class used to implement a custom resource bundle interface. See CefSettings
   * for additional options related to resource bundle loading. The methods of
   * this class may be called on multiple threads.
   */
  //{
  virtual bool GetLocalizedString(                                 ///<--
      int                                   string_id,                ///
      CefString&                            string)                   ///
                        OVERRIDE;                                     ///

  virtual bool GetDataResource(                                    ///<--
      int                                   resource_id,              ///
      void*&                                data,                     ///
      size_t&                               data_size)                ///
                        OVERRIDE;                                     ///

  virtual bool GetDataResourceForScale(                            ///<--
      int                                   resource_id,              ///
      ScaleFactor                           scale_factor,             ///
      void*&                                data,                     ///
      size_t&                               data_size)                ///
                        OVERRIDE;                                     ///
  //}

  virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() OVERRIDE { return this; }
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this; }

protected:
  // Schemes that will be registered with the global cookie manager.
  std::vector<CefString>  m_cookieableSchemes;

private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(CWebBrowserApp);
};

CWebBrowserApp::CWebBrowserApp()
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

CWebBrowserApp::~CWebBrowserApp()
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

/*!
 * @brief CefApp methods
 */
//{
void CWebBrowserApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnRegisterCustomSchemes(
    CefRefPtr<CefSchemeRegistrar> registrar)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}
//}

/*!
 * @brief CefBrowserProcessHandler methods
 */
//{
void CWebBrowserApp::OnContextInitialized()
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);

  CEF_REQUIRE_UI_THREAD();

  // Register cookieable schemes with the global cookie manager.
  CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
  DCHECK(manager.get());
  manager->SetSupportedSchemes(m_cookieableSchemes, NULL);

}
//}

/*!
 * @brief CefRenderProcessHandler methods
 */
//{
void CWebBrowserApp::OnRenderThreadCreated(
    CefRefPtr<CefListValue> extra_info)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnWebKitInitialized()
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnBrowserCreated(
    CefRefPtr<CefBrowser> browser)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnBrowserDestroyed(
    CefRefPtr<CefBrowser> browser)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

CefRefPtr<CefLoadHandler> CWebBrowserApp::GetLoadHandler()
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  return nullptr;
}

bool CWebBrowserApp::OnBeforeNavigation(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    NavigationType navigation_type,
    bool is_redirect)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  return false;
}

void CWebBrowserApp::OnContextCreated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnContextReleased(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnUncaughtException(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context,
    CefRefPtr<CefV8Exception> exception,
    CefRefPtr<CefV8StackTrace> stackTrace)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnFocusedNodeChanged(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefDOMNode> node)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

bool CWebBrowserApp::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  return false;
}
//}


/*!
 * @brief CefResourceBundle methods
 */
//{
bool CWebBrowserApp::GetLocalizedString(
    int                                   string_id,
    CefString&                            string)
{
  fprintf(stderr, " -- %s - %i\n", __PRETTY_FUNCTION__, string_id);
  return false;
}

bool CWebBrowserApp::GetDataResource(
    int                                   resource_id,
    void*&                                data,
    size_t&                               data_size)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  return false;
}

bool CWebBrowserApp::GetDataResourceForScale(
    int                                   resource_id,
    ScaleFactor                           scale_factor,
    void*&                                data,
    size_t&                               data_size)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  return false;
}
//}



int CWebBrowserManager::m_iUniqueClientId = 0;

CWebBrowserManager::CWebBrowserManager(kodi::addon::CInstanceWeb* instance) :
  m_isActive(false),
  m_windowlessEnabled(true),
  m_instance(instance)
{
}

CWebBrowserManager::~CWebBrowserManager()
{
}

bool CWebBrowserManager::Create()
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser add-on process creation start", __FUNCTION__);

  SetCEFPaths();
  if (!SetSandbox())
    return false;

  m_setting.LoadSettings();

  /*!
   * List of all aavailable settings to check during development
   * Not used ones which stays empty need to remove on end
   */
  m_CefSettings.single_process                      = 0;
  m_CefSettings.no_sandbox                          = 0;
  m_CefSettings.multi_threaded_message_loop         = 0;
  CefString(&m_CefSettings.cache_path)              = m_strHTMLCacheDir;
  CefString(&m_CefSettings.resources_dir_path)      = m_strResourcesPath;
  CefString(&m_CefSettings.locales_dir_path)        = m_strLocalesPath;
  CefString(&m_CefSettings.browser_subprocess_path) = m_strLibPath;
  CefString(&m_CefSettings.kodi_addon_dir_path)     = m_strLibPath;
  CefString(&m_CefSettings.user_data_path)          = "";
  CefString(&m_CefSettings.product_version)         = "KODI";
  CefString(&m_CefSettings.user_agent)              = StringUtils::Format("Chrome/%d.%d.%d.%d", CHROME_VERSION_MAJOR, CHROME_VERSION_MINOR, CHROME_VERSION_BUILD, CHROME_VERSION_PATCH);
  m_CefSettings.windowless_rendering_enabled        = m_windowlessEnabled ? 1 : 0;
  m_CefSettings.log_severity                        = g_pWebManager->GetSettings()->LogLevelCEF();
  CefString(&m_CefSettings.log_file)                = "";
  m_CefSettings.command_line_args_disabled          = 0;
  m_CefSettings.ignore_certificate_errors           = 0;
  m_CefSettings.persist_session_cookies             = 0;
  m_CefSettings.persist_user_preferences            = 0;
  CefString(&m_CefSettings.locale)                  = "";
  CefString(&m_CefSettings.javascript_flags)        = "";
  m_CefSettings.pack_loading_disabled               = 0;
  m_CefSettings.remote_debugging_port               = 0;
  m_CefSettings.uncaught_exception_stack_size       = 0;
  m_CefSettings.context_safety_implementation       = 0;
  m_CefSettings.background_color                    = 0;
  CefString(&m_CefSettings.accept_language_list)    = "";

  DCHECK(m_threadChecker.CalledOnValidThread());

  CreateThread();

  LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Started web browser add-on process", __FUNCTION__);

  m_isActive = true;
  return true;
}

void CWebBrowserManager::Destroy()
{
  m_isActive = false;

  StopThread();
  m_setting.SaveSettings();
  LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Stopped web browser add-on process", __FUNCTION__);
}

void *CWebBrowserManager::Process()
{
  DCHECK(m_threadChecker.CalledOnValidThread());

  CefMainArgs args;
  CefRefPtr<CefApp> app(new CWebBrowserApp);
  if (!CefInitialize(args, m_CefSettings, app, nullptr))
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser start failed", __FUNCTION__);
    return nullptr;
  }

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

      m_processQueueMutex.Lock();
      while (!m_processQueue.empty())
      {
        sMainThreadData *queueData = m_processQueue.front();
        m_processQueue.pop();
        queueData->function(queueData);
        queueData->event.Broadcast();
        CefDoMessageLoopWork();
      }
      m_processQueueMutex.Unlock();

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

  CefDoMessageLoopWork();
  CefShutdown();

  return nullptr;
}


WEB_ADDON_ERROR CWebBrowserManager::CreateControl(const WEB_ADDON_GUI_PROPS &props, const char *webType, ADDON_HANDLE handle)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  /*!
   * Paranoia ;-), prevent not wanted creation calls (normally not done)
   */
  if ((strcmp(webType, "browser") != 0) || handle == nullptr)
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Called for not supported web type %i", __FUNCTION__, webType);
    return WEB_ADDON_ERROR_INVALID_PARAMETERS;
  }

  CEF_REQUIRE_UI_THREAD();

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control creation started", __FUNCTION__);

  WEB_ADDON_ERROR addonError = WEB_ADDON_ERROR_FAILED;
  CWebBrowserClient *pBrowserClient;

  std::map<std::string, CWebBrowserClient*>::iterator itr = m_BrowserClientsInactive.find(props.strName);
  if (itr != m_BrowserClientsInactive.end())
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_INFO, "%s - Found control in inactive mode and setting active", __FUNCTION__);
    pBrowserClient = itr->second;
    pBrowserClient->SetActive();
    m_BrowserClientsInactive.erase(itr);
    addonError = WEB_ADDON_ERROR_NO_ERROR_REOPEN;
  }
  else
  {
    pBrowserClient = new CWebBrowserClient(m_iUniqueClientId++, &props, m_instance);

    CefWindowInfo info;
    info.SetAsWindowless(m_windowlessEnabled ? 1 : 0, false);

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
    settings.javascript                         = STATE_DEFAULT;
    settings.javascript_open_windows            = STATE_DEFAULT;
    settings.javascript_close_windows           = STATE_DEFAULT;
    settings.javascript_access_clipboard        = STATE_DEFAULT;
    settings.javascript_dom_paste               = STATE_DEFAULT;
    settings.caret_browsing                     = STATE_DEFAULT;
    settings.plugins                            = STATE_DISABLED;
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
      return addonError;
    }
    addonError = WEB_ADDON_ERROR_NO_ERROR;
  }

  int uniqueId = pBrowserClient->GetUniqueId();
  m_BrowserClients[uniqueId]  = pBrowserClient;
  handle->callerAddress       = pBrowserClient;
  handle->dataIdentifier      = uniqueId;
  pBrowserClient->SetAddonHandle(handle);

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control id '%i' created", __FUNCTION__, uniqueId);
  return addonError;
}

bool CWebBrowserManager::DestroyControl(const ADDON_HANDLE handle)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  //! Check for wrongly passed empty handle.
  if (handle == NULL)
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser control destroy called without handle!", __FUNCTION__);
    return false;
  }

  //! Find wanted control to destroy.
  std::map<int, CWebBrowserClient*>::iterator itr = m_BrowserClients.find(handle->dataIdentifier);
  if (itr == m_BrowserClients.end())
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser control destroy called for invalid id '%i'", __FUNCTION__, handle->dataIdentifier);
    return false;
  }

  //! Move to inactive list and remove it from active list
  CWebBrowserClient *client = itr->second;
  if (client->SetInactive())
    delete client;
  else
    m_BrowserClientsInactive[client->GetIdName()] = client;
  m_BrowserClients.erase(itr);

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control id '%i' destroyed", __FUNCTION__, handle->dataIdentifier);
  return true;
}

bool CWebBrowserManager::SetLanguage(const char *language)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser language set to '%s'", __FUNCTION__, language);
  return true;
}

bool CWebBrowserManager::OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Open URL '%s' called", __FUNCTION__, strURL);
  return ((CWebBrowserClient*)handle->callerAddress)->OpenWebsite(strURL, single, allowMenus);
}

void CWebBrowserManager::CallSingleCommand(const ADDON_HANDLE handle, WEB_ADDON_SINGLE_COMMANDS command)
{
  ((CWebBrowserClient*)handle->callerAddress)->CallSingleCommand(command);
}

void CWebBrowserManager::Render(const ADDON_HANDLE handle)
{
  ((CWebBrowserClient*)handle->callerAddress)->Render();
}

bool CWebBrowserManager::Dirty(const ADDON_HANDLE handle)
{
  return ((CWebBrowserClient*)handle->callerAddress)->Dirty();
}

void CWebBrowserManager::Stop(const ADDON_HANDLE handle)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control stopped", __FUNCTION__);
}

bool CWebBrowserManager::OnInit(const ADDON_HANDLE handle)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "%s - Web browser control initialize", __FUNCTION__);
  return true;
}

bool CWebBrowserManager::OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem)
{
  return ((CWebBrowserClient*)handle->callerAddress)->OnAction(actionId, nextItem);
}

bool CWebBrowserManager::OnMouseEvent(const ADDON_HANDLE handle, int mouseId, double x, double y, double offsetX, double offsetY, int state)
{
  return ((CWebBrowserClient*)handle->callerAddress)->OnMouseEvent(mouseId, x, y, offsetX, offsetY, state);
}

bool CWebBrowserManager::LoadUserSettings(void)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Load of user settings");
  return m_setting.LoadUserSettings();
}

bool CWebBrowserManager::SaveUserSettings(void)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Save of user settings");
  return m_setting.SaveUserSettings();
}

void CWebBrowserManager::SetCEFPaths()
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Setup add-on CEF directories:");
  std::string path = g_strUserPath;
  fprintf(stderr, "--------------<------------<: %s\n", path.c_str());
  if (path.at(path.size() - 1) != '\\' &&
      path.at(path.size() - 1) != '/')
    path.append("/");

  m_strHTMLCacheDir = path + "pchHTMLCache";
  m_strCookiePath = path + "pchCookies";

  path = g_strAddonLibPath;
  if (path.at(path.size() - 1) != '\\' &&
      path.at(path.size() - 1) != '/')
    path.append("/");

  m_strLibPath = path + "kodichromium";
  m_strSandboxBinary = path + "chrome-sandbox";

  path = g_strAddonSharePath;
  if (path.at(path.size() - 1) != '\\' &&
      path.at(path.size() - 1) != '/')
    path.append("/");

  m_strLocalesPath = path + "resources/cef/locales";
  m_strResourcesPath = path + "resources/cef/";

  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strHTMLCacheDir  %s", m_strHTMLCacheDir.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strCookiePath    %s", m_strCookiePath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strLocalesPath   %s", m_strLocalesPath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strResourcesPath %s", m_strResourcesPath.c_str());
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - m_strLibPath       %s", m_strLibPath.c_str());
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
bool CWebBrowserManager::SetSandbox()
{
  struct stat st;
  if (m_strSandboxBinary.empty() || stat(m_strSandboxBinary.c_str(), &st) != 0)
  {
    kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30000),
                                            kodi::GetLocalizedString(30001));
    LOG_MESSAGE(ADDON_LOG_ERROR, "Web browser sandbox binary missing, add-on not usable!");
    return false;
  }

  bool bCanceled;
  if (access(m_strSandboxBinary.c_str(), X_OK) != 0 || (st.st_uid != 0) ||
      ((st.st_mode & S_ISUID) == 0) || ((st.st_mode & S_IXOTH)) == 0)
  {
    if (!kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30000),
                                                    kodi::GetLocalizedString(30002),
                                                    bCanceled))
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
          command = StringUtils::Format("echo %s | sudo -S bash -c \"chown root:root %s; sudo -- chmod 4755 %s\"",
                                            strPassword.c_str(),
                                            m_strSandboxBinary.c_str(),
                                            m_strSandboxBinary.c_str());
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
