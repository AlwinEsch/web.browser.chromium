/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// CEF
#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"

namespace chromium
{
namespace app
{
namespace renderer
{

// Client app implementation for other process types.
class CWebAppRenderer : public CefApp,
                        public CefRenderProcessHandler,
                        public CefResourceBundleHandler
{
public:
  CWebAppRenderer();
  ~CWebAppRenderer();

  CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }
  bool CurrentSiteInterfaceAllowed() { return m_interfaceAllowed; }

  /// CefApp
  //@{
  void OnBeforeCommandLineProcessing(const CefString& process_type,
                                     CefRefPtr<CefCommandLine> command_line) override;
  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;
  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }
  CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override { return this; }
  //@}

  /// CefRenderProcessHandler
  //@{
  void OnWebKitInitialized() override;
  void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefDictionaryValue> extra_info) override;
  void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
  CefRefPtr<CefLoadHandler> GetLoadHandler() override;
  void OnContextCreated(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefFrame> frame,
                        CefRefPtr<CefV8Context> context) override;
  void OnContextReleased(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         CefRefPtr<CefV8Context> context) override;
  void OnUncaughtException(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefV8Context> context,
                           CefRefPtr<CefV8Exception> exception,
                           CefRefPtr<CefV8StackTrace> stackTrace) override;
  void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefDOMNode> node) override;
  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;
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

private:
  static std::vector<std::string> m_allowedInterfaceURLs;

  CefRefPtr<CefBrowser> m_browser;
  CefRefPtr<CefMessageRouterRendererSide> m_messageRouter;

  bool m_lastNodeIsEditable{false};
  int m_securityWebaddonAccess{0}; // by addon settings to set rights for Kodi's interface access
  bool m_interfaceAllowed{false};

  IMPLEMENT_REFCOUNTING(CWebAppRenderer);
  DISALLOW_COPY_AND_ASSIGN(CWebAppRenderer);
};

} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
