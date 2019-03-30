
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
#include "WebBrowserClient.h"
#include "Cookies/CookieHandler.h"
#include "gui/GUIManager.h"

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/base/cef_thread_checker.h"
#include "include/wrapper/cef_message_router.h"

#include <kodi/addon-instance/Web.h>
#include <p8-platform/threads/threads.h>
#include <queue>
#include <unordered_map>

class CWebBrowserClient;
class CRenderProcess;

class CWebBrowser
  : public kodi::addon::CInstanceWeb,
    public P8PLATFORM::CThread
{
public:
  CWebBrowser(KODI_HANDLE instance);
  virtual ~CWebBrowser();

  // ---------------------------------------------------------------------------
  // Internal interface parts

  bool IsActive() const { return m_isActive; }
  void OpenDownloadDialog();
  void OpenCookieHandler();

  // ---------------------------------------------------------------------------
  // Kodi interface parts

  WEB_ADDON_ERROR GetCapabilities(WEB_ADDON_CAPABILITIES& capabilities) override;

  bool MainInitialize();
  void MainShutdown();
  void MainLoop() override;

  WEB_ADDON_ERROR StartInstance() override;
  void StopInstance() override;
  bool SetLanguage(const char *language) override;
  kodi::addon::CWebControl* CreateControl(const std::string& sourceName, const std::string& startURL, KODI_HANDLE handle) override;
  bool DestroyControl(kodi::addon::CWebControl* control, bool complete) override;

  // ---------------------------------------------------------------------------
  // CEF interface parts
/*
  virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override { return this; }
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
  virtual CefRefPtr<CWebBrowserDownloadHandler> GetDownloadHandler() { return &m_downloadHandler; }
  virtual CefRefPtr<CWebBrowserUploadHandler> GetUploadHandler() { return m_uploadHandler; }

  const std::string& GetHTMLCachePath() { return m_strHTMLCachePath; }
  const std::string& GetCookiePath() { return m_strCookiePath; }
  const std::string& GetResourcePath() { return m_strResourcesPath; }

  void AddRef() const override { }
  bool Release() const override { return false; }
  bool HasOneRef() const override { return false; }
  bool HasAtLeastOneRef() const override { return false; }

  virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;

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
  //@}*/

  CBrowserGUIManager& GetGUIManager() { return m_guiManager; }

protected:
  virtual void* Process() override;

private:
  CBrowserGUIManager m_guiManager;
  void* m_sandboxInfo = nullptr;
  CefRefPtr<CefApp> m_app;
//   DISALLOW_COPY_AND_ASSIGN(CWebBrowser);
//
  static int m_iUniqueClientId;
//
//   CWebBrowserDownloadHandler m_downloadHandler;

  bool SetSandbox();

  bool m_isActive;
  CefSettings m_cefSettings;
  std::string m_strHTMLCachePath;
  std::string m_strCookiePath;
  std::string m_strResourcesPath;
  std::string m_strLocalesPath;
  std::string m_strLibPath;
  std::string m_strSandboxBinary;

  P8PLATFORM::CMutex m_mutex;

  std::unordered_map<int, CWebBrowserClient*> m_browserClients;
  std::unordered_map<std::string, CWebBrowserClient*> m_browserClientsInactive;
  std::vector<CWebBrowserClient*> m_browserClientsToDelete;
//   CCookieHandler m_cookieHandler;
//   CefRefPtr<CRenderProcess> m_renderProcess;
//   CefRefPtr<CefMessageRouterRendererSide> m_messageRouter;
  bool m_started = false;
};

