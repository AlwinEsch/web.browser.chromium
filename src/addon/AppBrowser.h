/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_app.h"
#include "include/cef_client.h"

#include <atomic>
#include <kodi/General.h>
#include <map>

class CWebBrowser;

class ATTRIBUTE_HIDDEN CClientAppBrowser : public CefApp,
                                           public CefResourceBundleHandler,
                                           public CefBrowserProcessHandler,
                                           public CefPowerPolicyController
{
public:
  CClientAppBrowser(CWebBrowser& addonMain);

  /// CefApp
  //@{
  void OnBeforeCommandLineProcessing(const CefString& process_type,
                                     CefRefPtr<CefCommandLine> command_line) override;
  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;
  CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override { return this; }
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
  CefRefPtr<CefPowerPolicyController> GetPowerPolicyController() override { return this; }
  //@}

  /// CefResourceBundleHandler
  //@{
  bool GetLocalizedString(int string_id, CefString& string) override;
  bool GetDataResource(int resource_id, void*& data, size_t& data_size) override;
  bool GetDataResourceForScale(int resource_id,
                               ScaleFactor scale_factor,
                               void*& data,
                               size_t& data_size) override;
  //@}

  /// CefBrowserProcessHandler
  //@{
  void GetCookieableSchemes(std::vector<CefString>& schemes, bool& include_defaults) override;
  void OnContextInitialized() override;
  void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override {}
  CefRefPtr<CefPrintHandler> GetPrintHandler() override;
  void OnScheduleMessagePumpWork(int64 delay_ms) override;
  CefRefPtr<CefClient> GetDefaultClient() override { return nullptr; }
  //@}

  /// CefPowerPolicyController
  //@{
  int AddSystemWakeLock(WakeLockReason reason, const CefString& description) override;
  int AddScreenWakeLock(WakeLockReason reason, const CefString& description) override;
  int AddDimWakeLock(WakeLockReason reason, const CefString& description) override;
  void RemoveWakeLock(int block_id) override;
  //@}

protected:
  // Schemes that will be registered with the global cookie manager.
  std::vector<CefString> m_cookieableSchemes;

private:
  enum class WakeType
  {
    System,
    Screen,
    Dim
  };

  bool ContainsType(WakeType reason);

  CWebBrowser& m_addonMain;

  CefRefPtr<CefPrintHandler> m_printHandler;

  IMPLEMENT_REFCOUNTING(CClientAppBrowser);
  DISALLOW_COPY_AND_ASSIGN(CClientAppBrowser);

  std::map<int, std::pair<WakeType, WakeLockReason>> m_locks;
  std::atomic_int m_nextLockId{1};
};
