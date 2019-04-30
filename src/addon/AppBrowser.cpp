/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppBrowser.h"
#include "SchemeKodi.h"

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

void CClientAppBrowser::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
  std::string commandLine = command_line->GetCommandLineString();
  fprintf(stderr, "--> %s '%s'\n", __FUNCTION__, commandLine.c_str());
}

void CClientAppBrowser::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
  fprintf(stderr, "--> %s\n", __FUNCTION__);
}

CefRefPtr<CefPrintHandler> CClientAppBrowser::GetPrintHandler()
{
  fprintf(stderr, "--> %s\n", __FUNCTION__);
  return NULL;
}

void CClientAppBrowser::OnScheduleMessagePumpWork(int64 delay_ms)
{
//   fprintf(stderr, "--> %s delay_ms %li\n", __FUNCTION__, delay_ms);
}
//@}
