#pragma once
/*
 *      Copyright (C) 2015-2017 Team KODI
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

#include "include/cef_app.h"
#include <p8-platform/threads/threads.h>

/// TODO move this to external app process after addon interface is reworked
class CRenderProcess
  : public P8PLATFORM::CThread,
    public CefApp,
    public CefRenderProcessHandler
{
public:
  CRenderProcess(const CefSettings& cefSettings);
  ~CRenderProcess() override;

  void AddRef() const override { }
  bool Release() const override { return false; }
  bool HasOneRef() const override { return false; }

protected:
  virtual void* Process(void) override;

private:
  // CefApp methods.
  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { fprintf(stderr, "--1-----------------------<> %s\n", __PRETTY_FUNCTION__); return this; }

  // CefRenderProcessHandler
  //@{
  void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) override;
  void OnWebKitInitialized() override;
  void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
  void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
  bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request,
                                  NavigationType navigation_type, bool is_redirect) override;
  void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
  void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
  void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context,
                                   CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) override;
  void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) override;
  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
  //@}

  DISALLOW_COPY_AND_ASSIGN(CRenderProcess);

  const CefSettings& m_cefSettings;
  CefRefPtr<CefMessageRouterRendererSide> m_messageRouter;
};
