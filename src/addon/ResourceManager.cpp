/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "ResourceManager.h"

namespace ResourceManager
{

std::string KodiURLFilter(const std::string& url)
{
  return url;
}

void SetupResourceManager(CefRefPtr<CefResourceManager> resource_manager)
{
  if (!CefCurrentlyOn(TID_IO))
  {
    // Execute on the browser IO thread.
    CefPostTask(TID_IO, base::Bind(SetupResourceManager, resource_manager));
    return;
  }

  // Add the URL filter.
  if (resource_manager)
    resource_manager->SetUrlFilter(base::Bind(KodiURLFilter));
}

} // namespace ResourceManager
