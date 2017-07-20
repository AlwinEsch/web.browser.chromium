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

#define NDEBUG 1

#include <map>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/base/cef_thread_checker.h"
#include "p8-platform/threads/threads.h"

#include "SettingsMain.h"
#include "WebBrowserClient.h"

class CWebBrowserManager : public P8PLATFORM::CThread
{
public:
  CWebBrowserManager();
  virtual ~CWebBrowserManager();

  bool Create();
  void Destroy();

  bool IsActive() const { return m_isActive; }

  WEB_ADDON_ERROR CreateControl(const WEB_ADDON_GUI_PROPS &props, const char *webType, ADDON_HANDLE handle);
  bool DestroyControl(const ADDON_HANDLE handle);
  bool SetLanguage(const char *language);
  void Render(const ADDON_HANDLE handle);
  void Stop(const ADDON_HANDLE handle);
  bool Dirty(const ADDON_HANDLE handle);
  bool OnInit(const ADDON_HANDLE handle);
  bool OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem);
  bool OnMouseEvent(const ADDON_HANDLE handle, int id, double x, double y, double offsetX, double offsetY, int state);

  bool OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus);
  void CallSingleCommand(const ADDON_HANDLE handle, WEB_ADDON_SINGLE_COMMANDS command);

  bool LoadUserSettings(void);
  bool SaveUserSettings(void);
  CSettingsMain* GetSettings() { return &m_setting; }

  const std::string& GetResourceDir() { return m_strResourcesPath; }

protected:
  virtual void *Process(void);

private:
  void SetCEFPaths();
  bool SetSandbox();

  static int        m_iUniqueClientId;

  CefSettings       m_CefSettings;
  CefRefPtr<CefApp> m_CefApp;
  std::string       m_strHTMLCacheDir;
  std::string       m_strCookiePath;
  std::string       m_strResourcesPath;
  std::string       m_strLocalesPath;
  std::string       m_strLibPath;
  std::string       m_strSandboxBinary;

  struct sMainThreadData;
  typedef void (*MainThreadFunction)(struct sMainThreadData *data);
  typedef struct sMainThreadData
  {
    CWebBrowserManager *manager;
    const ADDON_HANDLE *handle;
    MainThreadFunction  function;
    union data_t
    {
      struct
      {
        const WEB_ADDON_GUI_PROPS *props;
        unsigned int webType;
        ADDON_HANDLE *handle;
      } CreateControl;
      struct
      {
        const char* strURL;
        bool single;
        bool allowMenus;
      } OpenWebsite;
      struct
      {
        int actionId;
        int *nextItem;
      } OnAction;
      struct
      {
        WEB_ADDON_SINGLE_COMMANDS command;
      } CallSingleCommand;
      struct
      {
        int mouseId;
        double x;
        double y;
        double offsetX;
        double offsetY;
        int state;
      } OnMouseEvent;
    } data;
    union ret_t
    {
      bool booleanError;
      WEB_ADDON_ERROR addonError;
    } ret;
    P8PLATFORM::CEvent event;
  } sMainThreadData;

  static void CreateControl_Main(sMainThreadData *data);
  static void DestroyControl_Main(sMainThreadData *data);
  static void OpenWebsite_Main(sMainThreadData *data);
  static void OnAction_Main(sMainThreadData *data);
  static void OnMouseEvent_Main(sMainThreadData *data);
  static void CallSingleCommand_Main(sMainThreadData *data);

  std::queue <sMainThreadData*> m_processQueue;
  P8PLATFORM::CMutex m_processQueueMutex;
  P8PLATFORM::CMutex  m_Mutex;
  std::map<int, CWebBrowserClient*> m_BrowserClients;
  std::map<std::string, CWebBrowserClient*> m_BrowserClientsInactive;

  CSettingsMain     m_setting;
  bool              m_isActive;
  bool              m_windowlessEnabled;

  // Used to verify that methods are called on the correct thread.
  base::ThreadChecker     m_threadChecker;

  CefRefPtr<CefBrowser>         m_pBrowser;
  CefRefPtr<CWebBrowserClient>  m_pBrowserClient;
};
