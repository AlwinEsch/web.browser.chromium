/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/Window.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/ExtendedProgress.h"

// CEF
#include "include/cef_download_handler.h"

// Global
#include <ctime>
#include <map>
#include <memory>
#include <mutex>

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

class CDownloadItem
{
public:
  CDownloadItem(const std::string& url, CefRefPtr<CefDownloadItemCallback> callback);
  CDownloadItem(const std::string& name,
                const std::string& url,
                const std::string& path,
                std::time_t time);
  ~CDownloadItem();

  void SetActive(const std::string& name, const std::string& path);
  void SetInProgress(bool inProgress);
  void SetPaused(bool paused);
  void SetCanceled(bool canceled);
  void SetComplete();
  void SetProgressText(long totalMBytes, long receivedMBytes, float percentage);
  bool IsActive() const { return m_active; }
  bool IsComplete() const { return m_complete; }
  bool IsCanceled() const { return m_canceled; }
  bool IsPaused() const { return m_paused; }
  std::time_t GetDownloadTime() const { return m_time; }

  void Cancel();
  void Pause();
  void Resume();

  const std::string& GetURL() const { return m_url; }
  const std::string& GetName() const { return m_name; }
  const std::string& GetPath() const { return m_path; }
  const std::string& GetProcessText() const { return m_processText; }

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
  std::shared_ptr<kodi::gui::dialogs::CExtendedProgress> m_progressDialog;
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
  bool OnAction(const kodi::gui::input::CAction& action) override;
  void GetContextButtons(int itemNumber,
                         std::vector<std::pair<unsigned int, std::string>>& buttons) override;
  bool OnContextButton(int itemNumber, unsigned int button) override;

  // CEF's related download handle childs
  void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefDownloadItem> download_item,
                        const CefString& suggested_name,
                        CefRefPtr<CefBeforeDownloadCallback> callback) override;

  void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefDownloadItem> download_item,
                         CefRefPtr<CefDownloadItemCallback> callback) override;

  bool HasDownloads() const
  {
    return !m_activeDownloads.empty() || !m_finishedDownloads.empty() ? true : false;
  }
  bool HasActiveDownloads() const { return !m_activeDownloads.empty() ? true : false; }
  bool HasFinishedDownloads() const { return !m_finishedDownloads.empty() ? true : false; }
  void RemovedFinishedDownload(std::shared_ptr<CDownloadItem> download);
  void ResetHistory();

  const std::map<std::string, std::shared_ptr<CDownloadItem>>& GetActiveDownloads() const
  {
    return m_activeDownloads;
  }
  const std::map<std::string, std::shared_ptr<CDownloadItem>>& GetFinishedDownloads() const
  {
    return m_finishedDownloads;
  }

  // override CEF's CefRefPtr handling
  void AddRef() const override {}
  bool Release() const override { return false; }
  bool HasOneRef() const override { return false; }
  bool HasAtLeastOneRef() const override { return false; }

private:
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserDownloadHandler);

  static std::mutex m_mutex;

  bool LoadDownloadHistory(bool initial);
  bool SaveDownloadHistory();

  std::vector<std::shared_ptr<CDownloadItem>> m_items;
  std::map<std::string, std::shared_ptr<CDownloadItem>> m_activeDownloads;
  std::map<std::string, std::shared_ptr<CDownloadItem>> m_finishedDownloads;
};

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
