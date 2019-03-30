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

#include "AppBrowser.h"

CClientAppBrowser::CClientAppBrowser(CWebBrowser* addonMain) : m_addonMain(addonMain)
{
  fprintf(stderr, "--> %s\n", __PRETTY_FUNCTION__);
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
  std::string commandLine = command_line->GetCommandLineString();
  std::string processType = process_type;
  fprintf(stderr, "--> %s '%s' '%s'\n", __PRETTY_FUNCTION__, processType.c_str(), commandLine.c_str());
}

void CClientAppBrowser::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  fprintf(stderr, "--> %s\n", __PRETTY_FUNCTION__);
}

void CClientAppBrowser::OnContextInitialized()
{
  fprintf(stderr, "--> %s\n", __PRETTY_FUNCTION__);
}

void CClientAppBrowser::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
  std::string commandLine = command_line->GetCommandLineString();
  fprintf(stderr, "--> %s '%s'\n", __PRETTY_FUNCTION__, commandLine.c_str());
}

void CClientAppBrowser::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
  fprintf(stderr, "--> %s\n", __PRETTY_FUNCTION__);
}

CefRefPtr<CefPrintHandler> CClientAppBrowser::GetPrintHandler()
{
  fprintf(stderr, "--> %s\n", __PRETTY_FUNCTION__);
  return NULL;
}

void CClientAppBrowser::OnScheduleMessagePumpWork(int64 delay_ms)
{
//   fprintf(stderr, "--> %s\n", __PRETTY_FUNCTION__);
}
//@}
