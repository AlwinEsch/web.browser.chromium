/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppBrowser.h"
#include "SchemeKodi.h"

#include "MessageIds.h"

#include <kodi/General.h>

CClientAppBrowser::CClientAppBrowser(CWebBrowser* addonMain) : m_addonMain(addonMain)
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

bool CClientAppBrowser::GetDataResourceForScale(int resource_id, ScaleFactor scale_factor, void*& data, size_t& data_size)
{
  //TODO is useful?
  return false;
}
//@}

/// CefBrowserProcessHandler
//@{
void CClientAppBrowser::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
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
}

void CClientAppBrowser::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  registrar->AddCustomScheme("kodi", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
}

void CClientAppBrowser::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
  // Give needed Kodi settings to render process application
  CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
  dict->SetInt(SettingValues::security_webaddon_access, kodi::GetSettingInt("security.webaddon.access"));
  extra_info->SetDictionary(0, dict);
}

CefRefPtr<CefPrintHandler> CClientAppBrowser::GetPrintHandler()
{
  return nullptr;
}

void CClientAppBrowser::OnScheduleMessagePumpWork(int64 delay_ms)
{
}
//@}
