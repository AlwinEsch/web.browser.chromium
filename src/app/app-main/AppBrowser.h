/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_app.h"
#include "include/cef_client.h"

// Global
#include <atomic>
#include <map>
#include <memory>

namespace chromium
{
namespace app
{
namespace main
{

class CMainCEFProcess;

class CClientAppBrowser : public CefApp,
                          public CefResourceBundleHandler,
                          public CefBrowserProcessHandler
{
public:
  CClientAppBrowser(std::shared_ptr<CMainCEFProcess> addonMain);
  ~CClientAppBrowser() override;

  /// CefApp
  //@{
  void OnBeforeCommandLineProcessing(const CefString& process_type,
                                     CefRefPtr<CefCommandLine> command_line) override;
  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;
  CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override { return this; }
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
  CefRefPtr<CefPowerPolicyController> GetPowerPolicyController() override { return m_powerPolicyController; }
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
  void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;
  CefRefPtr<CefPrintHandler> GetPrintHandler() override;
  void OnScheduleMessagePumpWork(int64 delay_ms) override;
  CefRefPtr<CefClient> GetDefaultClient() override { return nullptr; }
  //@}

protected:
  // Schemes that will be registered with the global cookie manager.
  std::vector<CefString> m_cookieableSchemes;

private:
  std::shared_ptr<CMainCEFProcess> m_addonMain;

  CefRefPtr<CefPrintHandler> m_printHandler;
  CefRefPtr<CefPowerPolicyController> m_powerPolicyController;

  IMPLEMENT_REFCOUNTING(CClientAppBrowser);
  DISALLOW_COPY_AND_ASSIGN(CClientAppBrowser);

  uint32_t m_nextChildProcessIdentifier{0};
};

} /* namespace main */
} /* namespace app */
} /* namespace chromium */
