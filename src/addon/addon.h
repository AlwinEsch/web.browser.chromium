#pragma once
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

#include "DownloadHandler.h"
#include "GeolocationPermission.h"
#include "UploadHandler.h"
#include "WebBrowserClient.h"
#include "Cookies/CookieHandler.h"

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/base/cef_thread_checker.h"
#include "include/wrapper/cef_message_router.h"

#include <kodi/addon-instance/Web.h>
#include <p8-platform/threads/threads.h>
#include <queue>

class CWebBrowserClient;

class CWebBrowser
  : public kodi::addon::CInstanceWeb,
    public P8PLATFORM::CThread,
    public CefApp,
    public CefResourceBundleHandler,
    public CefBrowserProcessHandler,
    public CefRenderProcessHandler
{
public:
  CWebBrowser(KODI_HANDLE instance);
  virtual ~CWebBrowser();

  bool IsActive() const { return m_isActive; }

  virtual WEB_ADDON_ERROR GetCapabilities(WEB_ADDON_CAPABILITIES& capabilities) override;
  virtual WEB_ADDON_ERROR StartInstance() override;
  virtual void StopInstance() override;

  virtual bool SetLanguage(const char *language) override;

  virtual kodi::addon::CWebControl* CreateControl(const std::string& sourceName, const std::string& webType, KODI_HANDLE handle) override;
  virtual bool DestroyControl(kodi::addon::CWebControl* control) override;

  virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override { return this; }
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }
  virtual CefRefPtr<CWebBrowserDownloadHandler> GetDownloadHandler() { return m_downloadHandler; }
  virtual CefRefPtr<CWebBrowserUploadHandler> GetUploadHandler() { return m_uploadHandler; }
  virtual CefRefPtr<CWebBrowserGeolocationPermission> GetGeolocationPermission() { return m_geolocationPermission; }

  const std::string& GetHTMLCachePath() { return m_strHTMLCachePath; }
  const std::string& GetCookiePath() { return m_strCookiePath; }
  const std::string& GetResourcePath() { return m_strResourcesPath; }

  /// CefResourceBundleHandler
  //@{
  virtual bool GetLocalizedString(int string_id, CefString& string) override;
  virtual bool GetDataResource(int resource_id, void*& data, size_t& data_size) override;
  virtual bool GetDataResourceForScale(int resource_id, ScaleFactor scale_factor, void*& data, size_t& data_size) override;
  //@}

  /// CefBrowserProcessHandler
  //@{
  virtual void OnContextInitialized() override;
  virtual void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;
  virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) override;
  virtual CefRefPtr<CefPrintHandler> GetPrintHandler() override;
  virtual void OnScheduleMessagePumpWork(int64 delay_ms) override;
  //@}

  /// CefRenderProcessHandler
  //@{
  virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) override;
  virtual void OnWebKitInitialized() override;
  virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
  virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request,
                                  NavigationType navigation_type, bool is_redirect) override;
  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
  virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
  virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context,
                                   CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) override;
  virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) override;
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
  //@}

  void OpenDownloadDialog();
  void OpenCookieHandler();

protected:
  virtual void* Process(void) override;

private:
  IMPLEMENT_REFCOUNTING(CWebBrowser);
  DISALLOW_COPY_AND_ASSIGN(CWebBrowser);

  CefRefPtr<CWebBrowserDownloadHandler> m_downloadHandler;
  CefRefPtr<CWebBrowserUploadHandler> m_uploadHandler;
  CefRefPtr<CWebBrowserGeolocationPermission> m_geolocationPermission;
  CefRefPtr<CefMessageRouterRendererSide> m_messageRouter;

  bool SetSandbox();

  bool m_isActive;
  CefSettings m_cefSettings;
  std::string m_strHTMLCachePath;
  std::string m_strCookiePath;
  std::string m_strResourcesPath;
  std::string m_strLocalesPath;
  std::string m_strLibPath;
  std::string m_strSandboxBinary;
  base::ThreadChecker m_threadChecker;   // Used to verify that methods are called on the correct thread.








  static int m_iUniqueClientId;

  CefRefPtr<CefApp> m_CefApp;

  P8PLATFORM::CMutex  m_Mutex;
  std::map<int, CWebBrowserClient*> m_BrowserClients;
  std::map<std::string, CWebBrowserClient*> m_BrowserClientsInactive;

  // Schemes that will be registered with the global cookie manager.
  std::vector<CefString> m_cookieable_schemes;
  CCookieHandler m_cookieHandler;
};
