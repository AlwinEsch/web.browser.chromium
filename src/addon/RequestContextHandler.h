/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_request_context_handler.h"

class CRequestContextHandler : public CefRequestContextHandler
{
public:
  CRequestContextHandler() = default;

  bool OnBeforePluginLoad(const CefString& mime_type, const CefString& plugin_url, bool is_main_frame, const CefString& top_origin_url,
                          CefRefPtr<CefWebPluginInfo> plugin_info, PluginPolicy* plugin_policy) override;

private:
  IMPLEMENT_REFCOUNTING(CRequestContextHandler);
};
