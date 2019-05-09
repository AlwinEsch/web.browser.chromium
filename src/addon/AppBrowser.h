/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
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

  /// CefApp
  //@{
  void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;
  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;
  CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override { return this; }
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
  //@}

  /// CefResourceBundleHandler
  //@{
  bool GetLocalizedString(int string_id, CefString& string) override;
  bool GetDataResource(int resource_id, void*& data, size_t& data_size) override;
  bool GetDataResourceForScale(int resource_id, ScaleFactor scale_factor, void*& data, size_t& data_size) override;
  //@}

  /// CefBrowserProcessHandler
  //@{
  void OnContextInitialized() override;
  void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override { }
  void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) override;
  CefRefPtr<CefPrintHandler> GetPrintHandler() override;
  void OnScheduleMessagePumpWork(int64 delay_ms) override;
  //@}*/

private:
  CWebBrowser* m_addonMain;

  IMPLEMENT_REFCOUNTING(CClientAppBrowser);
  DISALLOW_COPY_AND_ASSIGN(CClientAppBrowser);
};
