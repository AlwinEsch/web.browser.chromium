/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/wrapper/cef_message_router.h"

class CWebBrowserClient;

class CV8Kodi : public virtual CefBaseRefCounted
{
public:
  CV8Kodi(CefRefPtr<CWebBrowserClient> client);

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message);

private:
  IMPLEMENT_REFCOUNTING(CV8Kodi);
  DISALLOW_COPY_AND_ASSIGN(CV8Kodi);

  CefRefPtr<CWebBrowserClient> m_client;
};
