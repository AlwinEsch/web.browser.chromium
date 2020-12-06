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
class CWebAppRenderer : public CefApp, public CefRenderProcessHandler
{
public:
  CWebAppRenderer();
  ~CWebAppRenderer();

  void OnBeforeCommandLineProcessing(const CefString& process_type,
                                     CefRefPtr<CefCommandLine> command_line) override;
  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

//   void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) override;
  void OnWebKitInitialized() override;
  void OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info) override;
  void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
  CefRefPtr<CefLoadHandler> GetLoadHandler() override;
  void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
  void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
  void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,CefRefPtr<CefV8Context> context,
                           CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) override;
  void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) override;
  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;

  CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }

  bool CurrentSiteInterfaceAllowed() { return m_interfaceAllowed; }

private:
  static std::vector<std::string> m_allowedInterfaceURLs;

  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

  CefRefPtr<CefMessageRouterRendererSide> m_messageRouter;
  bool m_lastNodeIsEditable = false;
  CefRefPtr<CefBrowser> m_browser;
  int m_securityWebaddonAccess = 0; // controlled by addon settings to set rights for Kodi's interface access
  bool m_interfaceAllowed = false;
  bool m_v8Inited = false;

  std::string m_mainShared;

  IMPLEMENT_REFCOUNTING(CWebAppRenderer);
  DISALLOW_COPY_AND_ASSIGN(CWebAppRenderer);
};

} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
