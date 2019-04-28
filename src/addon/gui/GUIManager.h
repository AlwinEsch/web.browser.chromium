/*
 *      Copyright (C) 2015-2019 Team KODI
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

#pragma once

#include "DialogCookie.h"
#include "DialogDownload.h"
#include "DialogFile.h"
#include "DialogKeyboard.h"

#include "utils/Thread.h"

class CWebBrowser;

class CBrowserGUIManager : public CThread
{
public:
  CBrowserGUIManager(CWebBrowser* instance);

  bool Create();
  void Destroy();

  CBrowserDialogCookie& GetCookieDialog() { return m_cookie; }
  CefRefPtr<CWebBrowserDownloadHandler> GetDownloadDialog() { return &m_download; }
  CefRefPtr<CBrowserDialogFile> GetFileDialog() { return m_file; }
  CBrowserDialogKeyboard& GetKeyboard() { return m_keyboard; }

protected:
  void Process() override;

private:
  CWebBrowser* m_instance;

  CBrowserDialogCookie m_cookie;
  CWebBrowserDownloadHandler m_download;
  CefRefPtr<CBrowserDialogFile> m_file;
  CBrowserDialogKeyboard m_keyboard;
};

