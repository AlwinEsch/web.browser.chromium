/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "RequestContextHandler.h"

bool CRequestContextHandler::OnBeforePluginLoad(const CefString& mime_type, const CefString& plugin_url, bool is_main_frame,
                                                const CefString& top_origin_url, CefRefPtr<CefWebPluginInfo> plugin_info,
                                                PluginPolicy* plugin_policy)
{
  // Always allow the PDF plugin to load.
  if (*plugin_policy != PLUGIN_POLICY_ALLOW &&
      mime_type == "application/pdf")
  {
    *plugin_policy = PLUGIN_POLICY_ALLOW;
    return true;
  }

  return false;
}
