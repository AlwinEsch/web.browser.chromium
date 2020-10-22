/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppBrowser.h"

#include "MessageIds.h"
#include "PrintHandler.h"
#include "SchemeKodi.h"
#include "addon.h"

#include <kodi/General.h>

CClientAppBrowser::CClientAppBrowser(CWebBrowser& addonMain) : m_addonMain(addonMain)
{
}

/// CefResourceBundleHandler
//@{
bool CClientAppBrowser::GetLocalizedString(int string_id, CefString& string)
{
  //TODO maybe change chromium strings to them from Kodi?
  return false;
}

bool CClientAppBrowser::GetDataResource(int resource_id, void*& data, size_t& data_size)
{
  //TODO is useful?
  return false;
}

bool CClientAppBrowser::GetDataResourceForScale(int resource_id,
                                                ScaleFactor scale_factor,
                                                void*& data,
                                                size_t& data_size)
{
  //TODO is useful?
  return false;
}
//@}

/// CefBrowserProcessHandler
//@{
void CClientAppBrowser::GetCookieableSchemes(std::vector<CefString>& schemes,
                                             bool& include_defaults)
{
  // Defaults are "http", "https", "ws" and "wss"
  include_defaults = kodi::GetSettingBoolean("security.allow_cookies", true);
  if (include_defaults)
  {
    schemes.emplace_back("kodi");
  }
}

void CClientAppBrowser::OnBeforeCommandLineProcessing(const CefString& process_type,
                                                      CefRefPtr<CefCommandLine> command_line)
{
  command_line->AppendSwitch("kodi-addon-path=" + kodi::GetAddonPath());
#ifdef WIN32
  command_line->AppendSwitch("disable-gpu-shader-disk-cache");
  command_line->AppendSwitchWithValue("use-angle", "d3d11");
#else
  command_line->AppendSwitch("disable-gpu-compositing");
  command_line->AppendSwitch("disable-software-rasterizer");
#endif // WIN32
}

void CClientAppBrowser::OnContextInitialized()
{
  // Register kodi:// scheme's
  CefRegisterSchemeHandlerFactory("kodi", "home", new CSchemeKodiFactory());

  // Register cookieable schemes with the global cookie manager.
  CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
  if (manager.get())
    manager->SetSupportedSchemes(m_cookieableSchemes, true, nullptr);
  else
    kodi::Log(ADDON_LOG_ERROR, "Failed to get cookie manager");

  m_printHandler = CPrintHandler::CreatePrintHandler();
}

void CClientAppBrowser::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  registrar->AddCustomScheme("kodi", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
}

CefRefPtr<CefPrintHandler> CClientAppBrowser::GetPrintHandler()
{
  return m_printHandler;
}

void CClientAppBrowser::OnScheduleMessagePumpWork(int64 delay_ms)
{
}

//@}

//------------------------------------------------------------------------------
/// CefPowerPolicyController
///
/// To inihibit screensavers and system sleep in case of e.g. playback via
/// browser.
///
//@{

int CClientAppBrowser::AddSystemWakeLock(WakeLockReason reason, const CefString& description)
{
  const int block_id = m_nextLockId++;
  m_locks.emplace(block_id, std::make_pair(WakeType::System, reason));
  m_addonMain.InhibitShutdown(true);
  kodi::Log(ADDON_LOG_DEBUG, "CClientAppBrowser::%s: System sleep inhibit started, why: '%s'",
            __func__, description.ToString().c_str());
  return block_id;
}

int CClientAppBrowser::AddScreenWakeLock(WakeLockReason reason, const CefString& description)
{
  const int block_id = m_nextLockId++;
  m_locks.emplace(block_id, std::make_pair(WakeType::Screen, reason));
  m_addonMain.InhibitScreensaver(true);
  kodi::Log(ADDON_LOG_DEBUG, "CClientAppBrowser::%s: System screensaver inhibit started, why: '%s'",
            __func__, description.ToString().c_str());
  return block_id;
}

int CClientAppBrowser::AddDimWakeLock(WakeLockReason reason, const CefString& description)
{
  const int block_id = m_nextLockId++;
  m_locks.emplace(block_id, std::make_pair(WakeType::Dim, reason));
  kodi::Log(ADDON_LOG_DEBUG,
            "CClientAppBrowser::%s: System dim inhibit started, why: '%s' (currently unsupported "
            "in Kodi)",
            __func__, description.ToString().c_str());
  return block_id;
}

void CClientAppBrowser::RemoveWakeLock(int block_id)
{
  auto entry =
      std::find_if(m_locks.begin(), m_locks.end(),
                   [block_id](const std::pair<int, std::pair<WakeType, WakeLockReason>> entry) {
                     return entry.first == block_id;
                   });
  if (entry != m_locks.end())
  {
    const WakeType type = entry->second.first;
    m_locks.erase(entry);

    if (!ContainsType(type))
    {
      if (entry->second.first == WakeType::System)
      {
        m_addonMain.InhibitShutdown(false);
        kodi::Log(ADDON_LOG_DEBUG, "CClientAppBrowser::%s: System sleep inhibit ended", __func__);
      }
      else if (entry->second.first == WakeType::Screen)
      {
        m_addonMain.InhibitScreensaver(false);
        kodi::Log(ADDON_LOG_DEBUG, "CClientAppBrowser::%s: System screensaver inhibit ended",
                  __func__);
      }
      else if (entry->second.first == WakeType::Dim)
      {
        kodi::Log(ADDON_LOG_DEBUG, "CClientAppBrowser::%s: System dim inhibit ended", __func__);
      }
    }
  }
}

bool CClientAppBrowser::ContainsType(WakeType reason)
{
  auto entry =
      std::find_if(m_locks.begin(), m_locks.end(),
                   [reason](const std::pair<int, std::pair<WakeType, WakeLockReason>> entry) {
                     return entry.second.first == reason;
                   });
  return entry != m_locks.end();
}

//@}
