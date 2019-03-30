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

#pragma once

#include <kodi/General.h>

#include "include/cef_app.h"
#include "include/cef_client.h"

class CWebBrowser;

class ATTRIBUTE_HIDDEN CClientAppBrowser
  : public CefApp,
    public CefResourceBundleHandler,
    public CefBrowserProcessHandler
{
public:
  CClientAppBrowser(CWebBrowser* addonMain);

  void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;
  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;
  CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override { return this; }
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

  /// CefResourceBundleHandler
  //@{
  bool GetLocalizedString(int string_id, CefString& string) override;
  bool GetDataResource(int resource_id, void*& data, size_t& data_size) override;
  bool GetDataResourceForScale(int resource_id, ScaleFactor scale_factor, void*& data, size_t& data_size) override;
  //@}

  /// CefBrowserProcessHandler
  //@{
  void OnContextInitialized() override;
  void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;
  void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) override;
  CefRefPtr<CefPrintHandler> GetPrintHandler() override;
  void OnScheduleMessagePumpWork(int64 delay_ms) override;
  //@}*/

private:
  CWebBrowser* m_addonMain;

  IMPLEMENT_REFCOUNTING(CClientAppBrowser);
  DISALLOW_COPY_AND_ASSIGN(CClientAppBrowser);
};
