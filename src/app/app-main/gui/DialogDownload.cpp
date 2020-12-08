/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogDownload.h"

// Own
#include "../../utils/XMLUtils.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/Filesystem.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/ListItem.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/FileBrowser.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/OK.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/YesNo.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// Global
#include <iomanip>
#include <thread>

/*
TODO:
- Check after sandbox system reworked in multithread support this works without freezes on several loads
- Not sure about GUI show of progress bar in on browser window
  - Good to know, only shows not the best, also shown if download window opened
  - Maybe to make only a rotating image on bottom of Kodi's webbrowser window
- Look to make like on chromium as temporary file name and rename to correct on end
- Use another XML lib (tinyxml to old)
- Another ways possible to hold the download history?

This parts here are usable, only need observed to improve a bit more.
*/

#define DEBUG_LOGS

using kodi::tools::StringUtils;
using namespace chromium::app::utils;

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

std::mutex CWebBrowserDownloadHandler::m_mutex;

CDownloadItem::CDownloadItem(const std::string& url, CefRefPtr<CefDownloadItemCallback> callback)
  : m_url(url), m_callback(callback)
{
}

CDownloadItem::CDownloadItem(const std::string& name,
                             const std::string& url,
                             const std::string& path,
                             std::time_t time)
  : m_url(url), m_name(name), m_path(path), m_time(time), m_complete(true)
{
}

CDownloadItem::~CDownloadItem()
{
  m_progressDialog.reset();
}

void CDownloadItem::SetActive(const std::string& name, const std::string& path)
{
  m_active = true;
  m_name = name;
  m_path = path;
  m_callback->Resume();
}

void CDownloadItem::Cancel()
{
  kodi::Log(ADDON_LOG_INFO, "Download of '%s' canceled", m_url.c_str());
  m_callback->Cancel();
}

void CDownloadItem::Pause()
{
  if (m_paused)
    return;

  m_paused = true;
  m_progressDialog.reset();
  kodi::Log(ADDON_LOG_INFO, "Download of '%s' paused", m_url.c_str());
  m_callback->Pause();
}

void CDownloadItem::Resume()
{
  if (!m_paused)
    return;

  if (!m_progressDialog)
    m_progressDialog = std::make_shared<kodi::gui::dialogs::CExtendedProgress>(
        StringUtils::Format(kodi::GetLocalizedString(30082).c_str(), m_name.c_str()));
  m_paused = false;
  kodi::Log(ADDON_LOG_INFO, "Download of '%s' resumed", m_url.c_str());
  m_callback->Resume();
}

void CDownloadItem::SetInProgress(bool inProgress)
{
  if (m_inProgress != inProgress)
  {
    m_inProgress = inProgress;
    if (m_inProgress)
    {
      m_progressDialog = std::make_shared<kodi::gui::dialogs::CExtendedProgress>(
          StringUtils::Format(kodi::GetLocalizedString(30082).c_str(), m_name.c_str()));
    }
    else
    {
      m_progressDialog.reset();
    }
  }
}

void CDownloadItem::SetCanceled(bool canceled)
{
  if (m_canceled != canceled)
  {
    m_canceled = canceled;
    if (m_canceled)
    {
      m_progressDialog.reset();
    }
  }
}

void CDownloadItem::SetComplete()
{
  m_complete = true;
  m_progressDialog.reset();

  m_time = std::time(nullptr);
}

void CDownloadItem::SetProgressText(long totalMBytes, long receivedMBytes, float percentage)
{
  m_processText = StringUtils::Format(kodi::GetLocalizedString(30090).c_str(), percentage,
                                      totalMBytes, receivedMBytes);

  if (m_progressDialog)
  {
    m_progressDialog->SetText(
        StringUtils::Format(kodi::GetLocalizedString(30083).c_str(), totalMBytes, receivedMBytes));
    m_progressDialog->SetPercentage(percentage);
  }
}

//------------------------------------------------------------------------------

CWebBrowserDownloadHandler::CWebBrowserDownloadHandler()
  : CWindow("DialogDownloads.xml", "skin.estuary", true)
{
  LoadDownloadHistory(true);
}

void CWebBrowserDownloadHandler::Open()
{
  CWindow::Show();
}

void CWebBrowserDownloadHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                                                  CefRefPtr<CefDownloadItem> download_item,
                                                  const CefString& suggested_name,
                                                  CefRefPtr<CefBeforeDownloadCallback> callback)
{
  const std::string suggestedName = suggested_name.ToString();
  const std::string url = download_item->GetOriginalUrl().ToString();
  const int64 totalBytes = download_item->GetTotalBytes();

  std::thread([this, url, totalBytes, suggestedName, callback] {
    std::shared_ptr<CDownloadItem> downloadItem;
    {
      m_mutex.lock();
      auto it = m_activeDownloads.find(url);
      if (it != m_activeDownloads.end())
      {
        downloadItem = it->second;
        m_mutex.unlock();

        if (downloadItem == nullptr)
        {
          kodi::Log(ADDON_LOG_ERROR, "Download '%s' present on active downloads but no info class!",
                    url.c_str());
          return;
        }

        if (downloadItem->IsActive())
        {
          kodi::QueueFormattedNotification(QUEUE_INFO, kodi::GetLocalizedString(30084).c_str(),
                                           suggestedName.c_str());
          return;
        }
      }
      else
      {
        kodi::Log(ADDON_LOG_ERROR, "Download '%s' not found on active list", url.c_str());
        m_mutex.unlock();
      }
    }

    std::string path = kodi::GetSettingString("downloads.path");
    std::string header =
        StringUtils::Format(kodi::GetLocalizedString(30082).c_str(), suggestedName.c_str());
    if (kodi::GetSettingBoolean("downloads.askfolder"))
    {
      bool ret = kodi::gui::dialogs::FileBrowser::ShowAndGetDirectory("local", header, path, true);
      if (!ret)
        return;
    }
    else
    {
      kodi::QueueFormattedNotification(QUEUE_INFO, header.c_str());
    }

    path += suggestedName;

    if (kodi::vfs::FileExists(path))
    {
      bool canceled;
      bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(
          header, kodi::GetLocalizedString(30085), canceled, kodi::GetLocalizedString(30087),
          kodi::GetLocalizedString(30086));
      if (!ret || canceled)
      {
        kodi::Log(ADDON_LOG_INFO, "Download of '%s' was already present on '%s' and is canceled",
                  url.c_str(), path.c_str());
        return;
      }

      bool successed = kodi::vfs::DeleteFile(path);
      if (!successed)
      {
        kodi::gui::dialogs::OK::ShowAndGetInput(
            kodi::GetLocalizedString(30088),
            StringUtils::Format(kodi::GetLocalizedString(30089).c_str(), path.c_str()));
        return;
      }
    }

    kodi::Log(ADDON_LOG_INFO, "Download of '%s' with %li MBytes started", url.c_str(),
              totalBytes / 1024 / 1024);

    callback->Continue(path, false);
    downloadItem->SetActive(suggestedName, path);
  }).detach();
}

void CWebBrowserDownloadHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                                                   CefRefPtr<CefDownloadItem> download_item,
                                                   CefRefPtr<CefDownloadItemCallback> callback)
{
  const std::string url = download_item->GetOriginalUrl().ToString();

#ifdef DEBUG_LOGS
  kodi::Log(ADDON_LOG_DEBUG, "%s", __func__);
  kodi::Log(ADDON_LOG_DEBUG, "%s --- IsValid: '%i'", __func__, download_item->IsValid());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- IsInProgress: '%i'", __func__, download_item->IsInProgress());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- IsComplete: '%i'", __func__, download_item->IsComplete());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- IsCanceled: '%i'", __func__, download_item->IsCanceled());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetCurrentSpeed: '%li'", __func__,
            download_item->GetCurrentSpeed());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetPercentComplete: '%i'", __func__,
            download_item->GetPercentComplete());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetTotalBytes: '%li'", __func__,
            download_item->GetTotalBytes());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetReceivedBytes: '%li'", __func__,
            download_item->GetReceivedBytes());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetStartTime: '%f'", __func__,
            download_item->GetStartTime().GetDoubleT());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetEndTime: '%f'", __func__,
            download_item->GetEndTime().GetDoubleT());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetFullPath: '%s'", __func__,
            download_item->GetFullPath().ToString().c_str());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetId: '%i'", __func__, download_item->GetId());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetURL: '%s'", __func__,
            download_item->GetURL().ToString().c_str());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetOriginalUrl: '%s'", __func__,
            download_item->GetOriginalUrl().ToString().c_str());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetSuggestedFileName: '%s'", __func__,
            download_item->GetSuggestedFileName().ToString().c_str());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetContentDisposition: '%s'", __func__,
            download_item->GetContentDisposition().ToString().c_str());
  kodi::Log(ADDON_LOG_DEBUG, "%s --- GetMimeType: '%s'", __func__,
            download_item->GetMimeType().ToString().c_str());
#endif

  std::shared_ptr<CDownloadItem> downloadItem;

  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_activeDownloads.find(url);
    if (it == m_activeDownloads.end())
    {
      downloadItem = std::make_shared<CDownloadItem>(url, callback);
      m_activeDownloads[url] = downloadItem;

      // Pause until the path is set
      callback->Pause();
    }
    else
      downloadItem = it->second;
  }

  if (!downloadItem->IsActive())
    return;

  const bool isComplete = download_item->IsComplete();
  const bool isCanceled = download_item->IsCanceled();
  const bool isInProgress = download_item->IsInProgress();
  const int64 totalBytes = download_item->GetTotalBytes();
  const int64 receivedBytes = download_item->GetReceivedBytes();
  const int percentComplete = download_item->GetPercentComplete();

  if (isComplete)
  {
    kodi::Log(ADDON_LOG_INFO, "Download of '%s' finished",
              download_item->GetOriginalUrl().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Is valid: '%i'", download_item->IsValid());
    kodi::Log(ADDON_LOG_INFO, " - Is complete: '%i'", isComplete);
    kodi::Log(ADDON_LOG_INFO, " - Is canceled: '%i'", isCanceled);
    kodi::Log(ADDON_LOG_INFO, " - Total bytes: '%li'", totalBytes);
    kodi::Log(ADDON_LOG_INFO, " - Received bytes: '%li'", receivedBytes);
    kodi::Log(ADDON_LOG_INFO, " - Start time: '%f'", download_item->GetStartTime().GetDoubleT());
    kodi::Log(ADDON_LOG_INFO, " - End time: '%f'", download_item->GetEndTime().GetDoubleT());
    kodi::Log(ADDON_LOG_INFO, " - Full path: '%s'",
              download_item->GetFullPath().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - URL: '%s'", download_item->GetURL().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Original Url: '%s'",
              download_item->GetOriginalUrl().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Suggested file name: '%s'",
              download_item->GetSuggestedFileName().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Content disposition: '%s'",
              download_item->GetContentDisposition().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Mime type: '%s'",
              download_item->GetMimeType().ToString().c_str());
  }

  std::thread([this, url, isComplete, isCanceled, isInProgress, totalBytes, receivedBytes,
               percentComplete, downloadItem, callback] {
    downloadItem->SetCanceled(isCanceled);
    downloadItem->SetInProgress(isInProgress);
    downloadItem->SetProgressText(static_cast<long>(totalBytes / 1024 / 1024),
                                  static_cast<long>(receivedBytes / 1024 / 1024),
                                  static_cast<float>(percentComplete));

    if (isComplete)
    {
      downloadItem->SetComplete();

      std::lock_guard<std::mutex> lock(m_mutex);
      m_finishedDownloads[url] = downloadItem;
      m_activeDownloads.erase(url);
      SaveDownloadHistory();
    }

    UpdateEntry(downloadItem, isComplete);
  }).detach();
}

void CWebBrowserDownloadHandler::RemovedFinishedDownload(std::shared_ptr<CDownloadItem> download)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  auto it = m_finishedDownloads.find(download->GetURL());
  m_finishedDownloads.erase(it);
  SaveDownloadHistory();
}

bool CWebBrowserDownloadHandler::LoadDownloadHistory(bool initial)
{
  TiXmlDocument xmlDoc;
  std::string strSettingsFile = kodi::GetBaseUserPath("download_history.xml");

  if (!xmlDoc.LoadFile(strSettingsFile))
  {
    if (initial)
    {
      if (!SaveDownloadHistory())
      {
        kodi::Log(ADDON_LOG_ERROR, "failed to create initial settings data file at '%s'",
                  strSettingsFile.c_str());
        return false;
      }
      return true;
    }
    else
      kodi::Log(ADDON_LOG_ERROR, "invalid settings data (no/invalid data file found at '%s')",
                strSettingsFile.c_str());
    return false;
  }

  TiXmlElement* pRootElement = xmlDoc.RootElement();
  if (strcmp(pRootElement->Value(), "downloadhistory") != 0)
  {
    if (!initial)
      kodi::Log(ADDON_LOG_ERROR, "invalid download history data (no <downloadhistory> tag found)");
    return false;
  }

  /* load history */
  TiXmlElement* pElement = pRootElement->FirstChildElement("histories");
  if (pElement)
  {
    TiXmlNode* pChannelNode = nullptr;
    while ((pChannelNode = pElement->IterateChildren(pChannelNode)) != nullptr)
    {
      std::string name;
      if (!XMLUtils::GetString(pChannelNode, "name", name))
      {
        kodi::Log(ADDON_LOG_ERROR, "Download defined in history without name");
        continue;
      }

      std::string url;
      if (!XMLUtils::GetString(pChannelNode, "url", url))
      {
        kodi::Log(ADDON_LOG_ERROR, "Download defined in history without url (%s)", name.c_str());
        continue;
      }

      std::string path;
      if (!XMLUtils::GetString(pChannelNode, "path", path))
      {
        kodi::Log(ADDON_LOG_ERROR, "Download defined in history without path (%s)", name.c_str());
        continue;
      }

      long time;
      if (!XMLUtils::GetLong(pChannelNode, "time", time))
      {
        kodi::Log(ADDON_LOG_ERROR, "Download defined in history without time (%s)", name.c_str());
        continue;
      }

      std::shared_ptr<CDownloadItem> downloadItem =
          std::make_shared<CDownloadItem>(name, url, path, time);
      m_finishedDownloads[url] = downloadItem;
    }
  }

  return true;
}

bool CWebBrowserDownloadHandler::SaveDownloadHistory()
{
  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("downloadhistory");
  TiXmlNode* pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == nullptr)
    return false;

  TiXmlElement xmlDownloadHistory("histories");
  TiXmlNode* pHistoriesNode = pRoot->InsertEndChild(xmlDownloadHistory);
  if (pHistoriesNode)
  {
    for (const auto& entry : m_finishedDownloads)
    {
      TiXmlElement xmlSetting("history");
      TiXmlNode* pHistoryNode = pHistoriesNode->InsertEndChild(xmlSetting);
      if (pHistoryNode)
      {
        XMLUtils::SetString(pHistoryNode, "name", entry.second->GetName().c_str());
        XMLUtils::SetString(pHistoryNode, "path", entry.second->GetPath().c_str());
        XMLUtils::SetString(pHistoryNode, "url", entry.second->GetURL().c_str());
        XMLUtils::SetLong(pHistoryNode, "time", static_cast<long>(entry.second->GetDownloadTime()));
      }
    }
  }

  std::string strSettingsFile = kodi::GetBaseUserPath("download_history.xml");
  if (!xmlDoc.SaveFile(strSettingsFile))
  {
    kodi::Log(ADDON_LOG_ERROR, "failed to write download history data");
    return false;
  }

  return true;
}

void CWebBrowserDownloadHandler::ResetHistory()
{
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_finishedDownloads.clear();
  }

  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("downloadhistory");
  TiXmlNode* pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == nullptr)
    return;

  TiXmlElement xmlDownloadHistory("histories");
  pRoot->InsertEndChild(xmlDownloadHistory);

  std::string strSettingsFile = kodi::GetBaseUserPath("download_history.xml");
  if (!xmlDoc.SaveFile(strSettingsFile))
  {
    kodi::Log(ADDON_LOG_ERROR, "failed to write download history data");
    return;
  }
}

void CWebBrowserDownloadHandler::UpdateEntry(std::shared_ptr<CDownloadItem> downloadItem,
                                             bool complete)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  for (unsigned int i = 0; i < m_items.size(); ++i)
  {
    if (m_items[i]->GetPath() != downloadItem->GetPath())
      continue;

    std::shared_ptr<kodi::gui::CListItem> item = GetListItem(i);
    std::string info;
    if (complete)
    {
      item->SetLabel2(kodi::GetLocalizedString(30015));
      std::time_t time = downloadItem->GetDownloadTime();
      auto tm = *std::localtime(&time);

      std::string format = kodi::GetRegion("datelong") + " - " + kodi::GetRegion("time");
      std::ostringstream oss;
      oss << std::put_time(&tm, format.c_str());
      item->SetProperty("downloadtime", oss.str());
    }
    else if (downloadItem->IsCanceled())
      item->SetLabel2(kodi::GetLocalizedString(30096));
    else if (downloadItem->IsPaused())
      item->SetLabel2(StringUtils::Format(kodi::GetLocalizedString(30095).c_str(),
                                          downloadItem->GetProcessText().c_str()));
    else
      item->SetLabel2(downloadItem->GetProcessText());
    break;
  }
}

bool CWebBrowserDownloadHandler::OnClick(int controlId)
{
  return false;
}

bool CWebBrowserDownloadHandler::OnInit()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  ClearList();
  m_items.clear();

  for (const auto& file : GetActiveDownloads())
    m_items.push_back(file.second);
  for (const auto& file : GetFinishedDownloads())
  {
    if (file.second->GetName().empty())
      continue;
    m_items.push_back(file.second);
  }

  for (const auto& file : m_items)
  {
    std::shared_ptr<kodi::gui::CListItem> item(new kodi::gui::CListItem(file->GetName()));

    std::string info;
    if (file->IsComplete())
    {
      info = kodi::GetLocalizedString(30015);

      std::time_t time = file->GetDownloadTime();
      auto tm = *std::localtime(&time);

      std::string format = kodi::GetRegion("datelong") + " - " + kodi::GetRegion("time");
      std::ostringstream oss;
      oss << std::put_time(&tm, format.c_str());
      item->SetProperty("downloadtime", oss.str());
    }
    else if (file->IsCanceled())
      info = kodi::GetLocalizedString(30096);
    else if (file->IsPaused())
      info = StringUtils::Format(kodi::GetLocalizedString(30095).c_str(),
                                 file->GetProcessText().c_str());
    else
      info = file->GetProcessText();

    item->SetLabel2(info);
    item->SetPath(file->GetPath());
    AddListItem(item);
  }

  return true;
}

bool CWebBrowserDownloadHandler::OnAction(const kodi::gui::input::CAction& action)
{
  if (action.GetID() == ADDON_ACTION_PREVIOUS_MENU || action.GetID() == ADDON_ACTION_NAV_BACK)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    ClearList();
    m_items.clear();
  }
  return CWindow::OnAction(action);
}

void CWebBrowserDownloadHandler::GetContextButtons(
    int itemNumber, std::vector<std::pair<unsigned int, std::string>>& buttons)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (m_items[itemNumber]->IsComplete())
    buttons.push_back(std::pair<unsigned int, std::string>(30091, kodi::GetLocalizedString(30091)));
  else
  {
    if (!m_items[itemNumber]->IsCanceled())
    {
      buttons.push_back(
          std::pair<unsigned int, std::string>(30092, kodi::GetLocalizedString(30092)));
      if (!m_items[itemNumber]->IsPaused())
        buttons.push_back(
            std::pair<unsigned int, std::string>(30093, kodi::GetLocalizedString(30093)));
      else
        buttons.push_back(
            std::pair<unsigned int, std::string>(30094, kodi::GetLocalizedString(30094)));
    }
  }
  if (HasFinishedDownloads())
    buttons.push_back(std::pair<unsigned int, std::string>(30097, kodi::GetLocalizedString(30097)));
}

bool CWebBrowserDownloadHandler::OnContextButton(int itemNumber, unsigned int button)
{
  if (button == 30092)
    m_items[itemNumber]->Cancel();
  else if (button == 30093)
    m_items[itemNumber]->Pause();
  else if (button == 30094)
    m_items[itemNumber]->Resume();
  else if (button == 30097)
  {
    ResetHistory();
    OnInit();
  }
  else if (button == 30091)
  {
    std::thread([this, itemNumber] {
      bool canceled = false;
      const std::string text = StringUtils::Format(kodi::GetLocalizedString(30098).c_str(),
                                                   m_items[itemNumber]->GetName().c_str());
      const bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(
          kodi::GetLocalizedString(30016), text, canceled, kodi::GetLocalizedString(30018),
          kodi::GetLocalizedString(30017));
      if (canceled)
        return false;

      if (ret)
        kodi::vfs::DeleteFile(m_items[itemNumber]->GetPath());

      RemovedFinishedDownload(m_items[itemNumber]);
      OnInit();
      return true;
    }).detach();
  }
  return true;
}

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
