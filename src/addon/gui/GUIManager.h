/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "DialogCookie.h"
#include "DialogDownload.h"
#include "DialogFile.h"
#include "DialogKeyboard.h"

class CWebBrowser;

class ATTRIBUTE_HIDDEN CBrowserGUIManager
{
public:
  CBrowserGUIManager(CWebBrowser* instance);

  bool Create();
  void Destroy();

  CBrowserDialogCookie& GetCookieDialog() { return m_cookie; }
  CefRefPtr<CWebBrowserDownloadHandler> GetDownloadDialog() { return &m_download; }
  CefRefPtr<CBrowserDialogFile> GetFileDialog() { return m_file; }
  CBrowserDialogKeyboard& GetKeyboard() { return m_keyboard; }

private:
  CWebBrowser* m_instance;

  CBrowserDialogCookie m_cookie;
  CWebBrowserDownloadHandler m_download;
  CefRefPtr<CBrowserDialogFile> m_file;
  CBrowserDialogKeyboard m_keyboard;
};

