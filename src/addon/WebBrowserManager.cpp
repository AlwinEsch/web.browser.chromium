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
#include "WebBrowserManager.h"
#include "Utils.h"

using namespace std;
using namespace ADDON;

int CWebBrowserManager::m_iUniqueClientId = 0;

CWebBrowserManager::CWebBrowserManager() :
  m_isActive(false)
{
}

CWebBrowserManager::~CWebBrowserManager()
{
}

bool CWebBrowserManager::Create()
{
  LOG_MESSAGE(LOG_DEBUG, "%s - Web browser add-on process creation start", __FUNCTION__);

  SetCEFPaths();
  m_setting.LoadSettings();

  CefString(&m_CefSettings.cache_path) = m_strHTMLCacheDir;
  CefString(&m_CefSettings.resources_dir_path) = m_strResourcesPath;
  CefString(&m_CefSettings.locales_dir_path) = m_strLocalesPath;
  CefString(&m_CefSettings.browser_subprocess_path) = m_strLibPath;
  CefString(&m_CefSettings.kodi_addon_dir_path) = m_strLibPath;
  CefString(&m_CefSettings.product_version) = "KODI";
  m_CefSettings.windowless_rendering_enabled = true;
//  m_CefSettings.single_process = 1;
//  m_CefSettings.no_sandbox = 0;
//  CefString(&m_CefSettings.log_file) =
//  m_CefSettings.pack_loading_disabled = true;
//  m_CefSettings.multi_threaded_message_loop = false;
//  m_CefSettings.command_line_args_disabled = true;

  CreateThread();

  LOG_MESSAGE(LOG_INFO, "%s - Started web browser add-on process", __FUNCTION__);

  m_isActive = true;
  return m_isActive;
}

void CWebBrowserManager::Destroy()
{
  m_isActive = false;

  StopThread();
  m_setting.SaveSettings();
  LOG_MESSAGE(LOG_INFO, "%s - Stopped web browser add-on process", __FUNCTION__);
}

void *CWebBrowserManager::Process()
{
  CefMainArgs args;
  CefRefPtr<CefApp> app;
  if (!CefInitialize(args, m_CefSettings, app, NULL))
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Web browser start failed", __FUNCTION__);
    return NULL;
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
      PLATFORM::CLockObject lock(m_Mutex);

      std::map<std::string, CWebBrowserClient*>::iterator itr;
      for (itr = m_BrowserClientsInactive.begin(); itr != m_BrowserClientsInactive.end(); ++itr)
      {
        if (itr->second->CurrentInactiveCountdown() < 0)
        {
          LOG_MESSAGE(LOG_INFO, "%s - Web browser control inactive countdown reached end and closed", __FUNCTION__);
          delete itr->second;
          m_BrowserClientsInactive.erase(itr);
        }
      }
    }
  }
  CefShutdown();
  return NULL;
}

WEB_ADDON_ERROR CWebBrowserManager::CreateControl(const WEB_ADDON_GUI_PROPS &props, unsigned int webType, ADDON_HANDLE handle)
{
  /*!
   * Paranoia ;-), prevent not wanted creation calls (normally not done)
   */
  if (webType != WEB_TYPE_ID_BROWSER || handle == nullptr)
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Called for not supported web type %i", __FUNCTION__, webType);
    return WEB_ADDON_ERROR_INVALID_PARAMETERS;
  }

  LOG_MESSAGE(LOG_DEBUG, "%s - Web browser control creation started", __FUNCTION__);

  WEB_ADDON_ERROR err = WEB_ADDON_ERROR_FAILED;
  CWebBrowserClient *pBrowserClient;

  PLATFORM::CLockObject lock(m_Mutex);

  std::map<std::string, CWebBrowserClient*>::iterator itr = m_BrowserClientsInactive.find(props.strName);
  if (itr != m_BrowserClientsInactive.end())
  {
    LOG_MESSAGE(LOG_INFO, "%s - Found control in inactive mode and setting active", __FUNCTION__);
    pBrowserClient = itr->second;
    pBrowserClient->SetActive();
    m_BrowserClientsInactive.erase(itr);
    err = WEB_ADDON_ERROR_NO_ERROR_REOPEN;
  }
  else
  {
    pBrowserClient = new CWebBrowserClient(m_iUniqueClientId++, &props);

    CefWindowInfo info;
    info.SetAsWindowless(0, true);

    CefBrowserSettings settings;
    settings.plugins = STATE_DISABLED;
    settings.webgl = STATE_DISABLED;
    settings.java = STATE_DISABLED;

    if (!CefBrowserHost::CreateBrowser(info, pBrowserClient, "", settings, nullptr))
    {
      LOG_MESSAGE(LOG_ERROR, "%s - Web browser creation failed", __FUNCTION__);
      if (pBrowserClient)
        delete pBrowserClient;
      return err;
    }
    err = WEB_ADDON_ERROR_NO_ERROR;
  }

  int uniqueId = pBrowserClient->GetUniqueId();
  m_BrowserClients[uniqueId] = pBrowserClient;
  handle->callerAddress      = pBrowserClient;
  handle->dataIdentifier     = uniqueId;

  LOG_MESSAGE(LOG_DEBUG, "%s - Web browser control id '%i' created", __FUNCTION__, uniqueId);
  return err;
}

bool CWebBrowserManager::DestroyControl(const ADDON_HANDLE handle)
{
  //! Check for wrongly passed empty handle.
  if (handle == NULL)
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Web browser control destroy called without handle!", __FUNCTION__);
    return false;
  }

  PLATFORM::CLockObject lock(m_Mutex);

  //! Find wanted control to destroy.
  int identifier = handle->dataIdentifier;
  std::map<int, CWebBrowserClient*>::iterator itr = m_BrowserClients.find(identifier);
  if (itr == m_BrowserClients.end())
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Web browser control destroy called for invalid id '%i'", __FUNCTION__, identifier);
    return false;
  }

  //! Move to inactive list and remove it from active list
  CWebBrowserClient *client = itr->second;
  if (client->SetInactive())
    delete client;
  else
    m_BrowserClientsInactive[client->GetIdName()] = client;
  m_BrowserClients.erase(itr);

  LOG_MESSAGE(LOG_DEBUG, "%s - Web browser control id '%i' destroyed", __FUNCTION__, identifier);
  return true;
}

bool CWebBrowserManager::SetLanguage(const char *language)
{
  LOG_MESSAGE(LOG_DEBUG, "%s - Web browser language set to '%s'", __FUNCTION__, language);
  return true;
}

bool CWebBrowserManager::OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus)
{
  return ((CWebBrowserClient*)handle->callerAddress)->OpenWebsite(strURL, single, allowMenus);
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
  LOG_MESSAGE(LOG_DEBUG, "%s - Web browser control stopped", __FUNCTION__);
}

bool CWebBrowserManager::OnInit(const ADDON_HANDLE handle)
{
  LOG_MESSAGE(LOG_DEBUG, "%s - Web browser control initialize", __FUNCTION__);
  return true;
}

bool CWebBrowserManager::OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem)
{
  return false;
}

bool CWebBrowserManager::OnMouseEvent(const ADDON_HANDLE handle, int mouseId, double x, double y, double offsetX, double offsetY, int state)
{
  return true;
}

bool CWebBrowserManager::LoadUserSettings(void)
{
  return m_setting.LoadUserSettings();
}

bool CWebBrowserManager::SaveUserSettings(void)
{
  return m_setting.SaveUserSettings();
}

void CWebBrowserManager::SetCEFPaths()
{
  std::string path = g_strUserPath;
  if (path.at(path.size() - 1) != '\\' &&
      path.at(path.size() - 1) != '/')
    path.append("/");
  path.append("cef/");

  m_strHTMLCacheDir = path + "pchHTMLCache";
  m_strCookiePath = path + "pchCookies";

  path = g_strAddonPath;
  if (path.at(path.size() - 1) != '\\' &&
      path.at(path.size() - 1) != '/')
    path.append("/");
  path.append("cef/");

  m_strLibPath = path + "kodichromium";
  m_strLocalesPath = path + "locales";
  m_strResourcesPath = path;
}
