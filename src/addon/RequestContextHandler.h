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

#include "include/cef_request_context.h"

class CRequestContextHandler : public CefRequestContextHandler
{
public:
  CRequestContextHandler() = default;

  bool OnBeforePluginLoad(const CefString& mime_type, const CefString& plugin_url, bool is_main_frame, const CefString& top_origin_url,
                          CefRefPtr<CefWebPluginInfo> plugin_info, PluginPolicy* plugin_policy) override;

private:
  IMPLEMENT_REFCOUNTING(CRequestContextHandler);
};
