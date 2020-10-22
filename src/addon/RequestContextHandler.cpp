/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "RequestContextHandler.h"

#include "ExtensionUtils.h"
#include "WebBrowserClient.h"
#include "include/cef_command_line.h"

#include <kodi/Filesystem.h>
#include <kodi/General.h>

/**
 * WARNING Currently a memleak!!!
 *
 * This class becomes created but never destroyed.
 * Problem already known by CEF:
 * - https://bitbucket.org/chromiumembedded/cef/issues/2505/cefrequestcontext-inherited-from-global
 * - https://bitbucket.org/chromiumembedded/cef/issues/2622/implement-support-for-networkservice
 *
 * TODO after CEF is fixed, fix it here!!!
 */

void CRequestContextHandler::OnRequestContextInitialized(
    CefRefPtr<CefRequestContext> request_context)
{
  CEF_REQUIRE_UI_THREAD();

  if (kodi::GetSettingBoolean("main.enable_extensions"))
  {
    // Load one or more extension paths specified on the command-line and
    // delimited with semicolon.
    std::string extensionsPath = kodi::GetBaseUserPath("extensions");
    std::vector<kodi::vfs::CDirEntry> items;
    kodi::vfs::GetDirectory(extensionsPath, "", items);
    kodi::Log(ADDON_LOG_DEBUG, "Scanning directory '%s' with %lu entries for browser extensions",
              extensionsPath.c_str(), items.size());

    for (const auto& item : items)
    {
      if (item.IsFolder())
      {
        ExtensionUtils::LoadExtension(request_context, item.Path(), this);
      }
    }
  }
}


bool CRequestContextHandler::OnBeforePluginLoad(const CefString& mime_type,
                                                const CefString& plugin_url,
                                                bool is_main_frame,
                                                const CefString& top_origin_url,
                                                CefRefPtr<CefWebPluginInfo> plugin_info,
                                                PluginPolicy* plugin_policy)
{
  // Always allow the PDF plugin to load.
  if (*plugin_policy != PLUGIN_POLICY_ALLOW && mime_type == "application/pdf")
  {
    *plugin_policy = PLUGIN_POLICY_ALLOW;
    return true;
  }

  return false;
}

void CRequestContextHandler::OnExtensionLoaded(CefRefPtr<CefExtension> extension)
{
  CEF_REQUIRE_UI_THREAD();

  m_browserClient->AddExtension(extension);
}

void CRequestContextHandler::OnExtensionUnloaded(CefRefPtr<CefExtension> extension)
{
}

bool CRequestContextHandler::OnBeforeBackgroundBrowser(CefRefPtr<CefExtension> extension,
                                                       const CefString& url,
                                                       CefRefPtr<CefClient>& client,
                                                       CefBrowserSettings& settings)
{
  client = m_browserClient;
  return false;
}

bool CRequestContextHandler::OnBeforeBrowser(CefRefPtr<CefExtension> extension,
                                             CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefBrowser> active_browser,
                                             int index,
                                             const CefString& url,
                                             bool active,
                                             CefWindowInfo& windowInfo,
                                             CefRefPtr<CefClient>& client,
                                             CefBrowserSettings& settings)
{
  client = m_browserClient;
  return false;
}

CefRefPtr<CefBrowser> CRequestContextHandler::GetActiveBrowser(CefRefPtr<CefExtension> extension,
                                                               CefRefPtr<CefBrowser> browser,
                                                               bool include_incognito)
{
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefBrowser> active_browser = m_browserClient->GetBrowser();
  if (!active_browser)
  {
    kodi::Log(ADDON_LOG_WARNING, "No active browser available for extension %s",
              browser->GetHost()->GetExtension()->GetIdentifier().ToString().c_str());
  }
  else
  {
    // The active browser should not be hosting an extension.
    DCHECK(!active_browser->GetHost()->GetExtension());
  }

  return active_browser;
}

bool CRequestContextHandler::CanAccessBrowser(CefRefPtr<CefExtension> extension,
                                              CefRefPtr<CefBrowser> browser,
                                              bool include_incognito,
                                              CefRefPtr<CefBrowser> target_browser)
{
  return true;
}

bool CRequestContextHandler::GetExtensionResource(
    CefRefPtr<CefExtension> extension,
    CefRefPtr<CefBrowser> browser,
    const CefString& file,
    CefRefPtr<CefGetExtensionResourceCallback> callback)
{
  return false;
}
