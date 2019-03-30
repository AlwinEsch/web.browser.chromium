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

#include <kodi/gui/dialogs/ExtendedProgress.h>
#include <kodi/gui/Window.h>

#include "include/cef_download_handler.h"

#include <ctime>
#include <map>
#include <memory>
#include <mutex>

class CDownloadItem
{
public:
  CDownloadItem(const std::string& url, CefRefPtr<CefDownloadItemCallback> callback);
  CDownloadItem(const std::string& name, const std::string& url, const std::string& path, std::time_t time);
  ~CDownloadItem();

  void SetActive(const std::string& name, const std::string& path);
  void SetInProgress(bool inProgress);
  void SetPaused(bool paused);
  void SetCanceled(bool canceled);
  void SetComplete();
  void SetProgressText(long totalMBytes, long receivedMBytes, float percentage);
  bool IsActive() { return m_active; }
  bool IsComplete() { return m_complete; }
  bool IsCanceled() { return m_canceled; }
  bool IsPaused() { return m_paused; }
  std::time_t GetDownloadTime() { return m_time; }

  void Cancel();
  void Pause();
  void Resume();

  const std::string& GetURL() { return m_url; }
  const std::string& GetName() { return m_name; }
  const std::string& GetPath() { return m_path; }
  const std::string& GetProcessText() { return m_processText; }

private:
  const std::string m_url;

  std::string m_name;
  std::string m_path;
  std::string m_processText;
  std::time_t m_time = 0;

  bool m_active = false;
  bool m_paused = false;
  bool m_inProgress = false;
  bool m_canceled = false;
  bool m_complete = false;
  CefRefPtr<CefDownloadItemCallback> m_callback = nullptr;
  kodi::gui::dialogs::CExtendedProgress* m_progressDialog = nullptr;
};

class CWebBrowserDownloadHandler : public CefDownloadHandler, public kodi::gui::CWindow
{
public:
  CWebBrowserDownloadHandler();
  ~CWebBrowserDownloadHandler() = default;

  void Open();

  void UpdateEntry(std::shared_ptr<CDownloadItem> downloadItem, bool complete);

  // Kodi's GUI related childs
  bool OnInit() override;
  bool OnClick(int controlId) override;
  bool OnAction(int actionId, uint32_t buttoncode, wchar_t unicode) override;
  void GetContextButtons(int itemNumber, std::vector< std::pair<unsigned int, std::string> > &buttons) override;
  bool OnContextButton(int itemNumber, unsigned int button) override;

  // CEF's related download handle childs
  void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
                        const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) override;

  void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
                         CefRefPtr<CefDownloadItemCallback> callback) override;

  bool HasDownloads() { return !m_activeDownloads.empty() || !m_finishedDownloads.empty() ? true : false; }
  bool HasActiveDownloads() { return !m_activeDownloads.empty() ? true : false; }
  bool HasFinishedDownloads() { return !m_finishedDownloads.empty() ? true : false; }
  void RemovedFinishedDownload(std::shared_ptr<CDownloadItem> download);
  void ResetHistory();

  const std::map<std::string, std::shared_ptr<CDownloadItem>>& GetActiveDownloads() { return m_activeDownloads; }
  const std::map<std::string, std::shared_ptr<CDownloadItem>>& GetFinishedDownloads() { return m_finishedDownloads; }

  // override CEF's CefRefPtr handling
  void AddRef() const override { }
  bool Release() const override { return false; }
  bool HasOneRef() const override { return false; }
  bool HasAtLeastOneRef() const override { return false; }

private:
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserDownloadHandler);

  static void OnBeforeDownloadProcess(CWebBrowserDownloadHandler* thisClass,
                                      std::string url, int64 totalBytes, std::string suggested_name,
                                      CefRefPtr<CefBeforeDownloadCallback> callback);

  static std::mutex m_mutex;

  bool LoadDownloadHistory(bool initial);
  bool SaveDownloadHistory();

  std::vector<std::shared_ptr<CDownloadItem>> m_items;
  std::map<std::string, std::shared_ptr<CDownloadItem>> m_activeDownloads;
  std::map<std::string, std::shared_ptr<CDownloadItem>> m_finishedDownloads;
};
