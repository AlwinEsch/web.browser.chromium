/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_extension_handler.h"
#include "include/cef_request_context_handler.h"

namespace chromium
{
namespace app
{
namespace main
{

class CWebBrowserClient;

class CRequestContextHandler : public CefRequestContextHandler, public CefExtensionHandler
{
public:
  CRequestContextHandler() = default;
  ~CRequestContextHandler() = default;

  // CefRequestContextHandler
  //@{
  void OnRequestContextInitialized(CefRefPtr<CefRequestContext> request_context) override;
  bool OnBeforePluginLoad(const CefString& mime_type,
                          const CefString& plugin_url,
                          bool is_main_frame,
                          const CefString& top_origin_url,
                          CefRefPtr<CefWebPluginInfo> plugin_info,
                          PluginPolicy* plugin_policy) override;
  //@}

  // CefExtensionHandler
  //@{
  void OnExtensionLoadFailed(cef_errorcode_t result) override {}
  void OnExtensionLoaded(CefRefPtr<CefExtension> extension) override;
  void OnExtensionUnloaded(CefRefPtr<CefExtension> extension) override;
  bool OnBeforeBackgroundBrowser(CefRefPtr<CefExtension> extension,
                                 const CefString& url,
                                 CefRefPtr<CefClient>& client,
                                 CefBrowserSettings& settings) override;
  bool OnBeforeBrowser(CefRefPtr<CefExtension> extension,
                       CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefBrowser> active_browser,
                       int index,
                       const CefString& url,
                       bool active,
                       CefWindowInfo& windowInfo,
                       CefRefPtr<CefClient>& client,
                       CefBrowserSettings& settings) override;
  CefRefPtr<CefBrowser> GetActiveBrowser(CefRefPtr<CefExtension> extension,
                                         CefRefPtr<CefBrowser> browser,
                                         bool include_incognito) override;
  bool CanAccessBrowser(CefRefPtr<CefExtension> extension,
                        CefRefPtr<CefBrowser> browser,
                        bool include_incognito,
                        CefRefPtr<CefBrowser> target_browser) override;
  bool GetExtensionResource(CefRefPtr<CefExtension> extension,
                            CefRefPtr<CefBrowser> browser,
                            const CefString& file,
                            CefRefPtr<CefGetExtensionResourceCallback> callback) override;
  //@}

  void Init(CefRefPtr<CWebBrowserClient> browserClient) { m_browserClient = browserClient; }
  void Clear() { m_browserClient = nullptr; }

private:
  IMPLEMENT_REFCOUNTING(CRequestContextHandler);
  DISALLOW_COPY_AND_ASSIGN(CRequestContextHandler);

  CefRefPtr<CWebBrowserClient> m_browserClient;
};

} /* namespace main */
} /* namespace app */
} /* namespace chromium */
