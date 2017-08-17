#pragma once
/*
 *      Copyright (C) 2015 Team KODI
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

#include <kodi/gui/dialogs/ExtendedProgress.h>
#include <kodi/gui/Window.h>
#include <p8-platform/threads/threads.h>

#include "include/cef_download_handler.h"

#include <ctime>
#include <map>
#include <memory>

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
  std::time_t m_time;

  bool m_active;
  bool m_paused;
  bool m_inProgress;
  bool m_canceled;
  bool m_complete;
  CefRefPtr<CefDownloadItemCallback> m_callback;
  kodi::gui::dialogs::CExtendedProgress* m_progressDialog;
};

class CWebBrowserDownloadHandler : public CefDownloadHandler, public kodi::gui::CWindow
{
public:
  CWebBrowserDownloadHandler();
  virtual ~CWebBrowserDownloadHandler() = default;

  void Open();

  void UpdateEntry(std::shared_ptr<CDownloadItem> downloadItem, bool complete);

  virtual bool OnInit() override;
  virtual bool OnClick(int controlId) override;
  virtual bool OnAction(int actionId) override;
  virtual void GetContextButtons(int itemNumber, std::vector< std::pair<unsigned int, std::string> > &buttons) override;
  virtual bool OnContextButton(int itemNumber, unsigned int button) override;


  virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
                                const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) override;

  virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
                                 CefRefPtr<CefDownloadItemCallback> callback) override;

  bool HasDownloads() { return !m_activeDownloads.empty() || !m_finishedDownloads.empty() ? true : false; }
  bool HasActiveDownloads() { return !m_activeDownloads.empty() ? true : false; }
  bool HasFinishedDownloads() { return !m_finishedDownloads.empty() ? true : false; }
  void RemovedFinishedDownload(std::shared_ptr<CDownloadItem> download);
  void ResetHistory();

  const std::map<std::string, std::shared_ptr<CDownloadItem>>& GetActiveDownloads() { return m_activeDownloads; }
  const std::map<std::string, std::shared_ptr<CDownloadItem>>& GetFinishedDownloads() { return m_finishedDownloads; }

  void AddRef() const override { }
  bool Release() const override { return false; }
  bool HasOneRef() const override { return false; }

private:
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserDownloadHandler);

  bool LoadDownloadHistory(bool initial);
  bool SaveDownloadHistory();


  std::vector<std::shared_ptr<CDownloadItem>> m_items;

  P8PLATFORM::CMutex m_mutex;
  std::map<std::string, std::shared_ptr<CDownloadItem>> m_activeDownloads;
  std::map<std::string, std::shared_ptr<CDownloadItem>> m_finishedDownloads;
};
