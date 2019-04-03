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

#include "DialogDownload.h"
#include "utils/Utils.h"

#include <kodi/General.h>
#include <kodi/Filesystem.h>
#include <kodi/gui/dialogs/FileBrowser.h>
#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/util/XMLUtils.h>
#include <p8-platform/util/StringUtils.h>
#include <iomanip>
#include <thread>

std::mutex CWebBrowserDownloadHandler::m_mutex;

CDownloadItem::CDownloadItem(const std::string& url, CefRefPtr<CefDownloadItemCallback> callback)
  : m_url(url),
    m_callback(callback)
{
}

CDownloadItem::CDownloadItem(const std::string& name, const std::string& url, const std::string& path, std::time_t time)
  : m_url(url),
    m_name(name),
    m_path(path),
    m_time(time),
    m_complete(true)
{
}

CDownloadItem::~CDownloadItem()
{
  if (m_progressDialog)
  {
    delete m_progressDialog;
    m_progressDialog = nullptr;
  }
}

void CDownloadItem::SetActive(const std::string& name, const std::string& path)
{
  m_active = true;
  m_name = name;
  m_path = path;
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
  delete m_progressDialog;
  m_progressDialog = nullptr;
  kodi::Log(ADDON_LOG_INFO, "Download of '%s' paused", m_url.c_str());
  m_callback->Pause();
}

void CDownloadItem::Resume()
{
  if (!m_paused)
    return;

  if (!m_progressDialog)
    m_progressDialog = new kodi::gui::dialogs::CExtendedProgress(StringUtils::Format(kodi::GetLocalizedString(30082).c_str(),
                                                                                     m_name.c_str()));
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
      m_progressDialog = new kodi::gui::dialogs::CExtendedProgress(StringUtils::Format(kodi::GetLocalizedString(30082).c_str(),
                                                                                       m_name.c_str()));
    }
    else
    {
      delete m_progressDialog;
      m_progressDialog = nullptr;
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
      delete m_progressDialog;
      m_progressDialog = nullptr;
    }
  }
}

void CDownloadItem::SetComplete()
{
  m_complete = true;
  delete m_progressDialog;
  m_progressDialog = nullptr;

  m_time = std::time(nullptr);
}

void CDownloadItem::SetProgressText(long totalMBytes, long receivedMBytes, float percentage)
{
  m_processText = StringUtils::Format(kodi::GetLocalizedString(30090).c_str(), percentage,
                                      totalMBytes, receivedMBytes);
  if (m_progressDialog)
  {
    m_progressDialog->SetText(StringUtils::Format(kodi::GetLocalizedString(30083).c_str(),
                                                  totalMBytes,
                                                  receivedMBytes));
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

void CWebBrowserDownloadHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
                                                  const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback)
{
  std::string suggestedName = suggested_name.ToString();
  std::string url = download_item->GetOriginalUrl().ToString();
  int64 totalBytes = download_item->GetTotalBytes();
  std::thread(OnBeforeDownloadProcess, this, url, totalBytes, suggestedName, callback).detach();
}

void CWebBrowserDownloadHandler::OnBeforeDownloadProcess(CWebBrowserDownloadHandler* thisClass,
                                                         std::string url, int64 totalBytes, std::string suggested_name,
                                                         CefRefPtr<CefBeforeDownloadCallback> callback)
{
  std::shared_ptr<CDownloadItem> downloadItem;
  {
    m_mutex.lock();
    auto it = thisClass->m_activeDownloads.find(url);
    if (it != thisClass->m_activeDownloads.end())
    {
      downloadItem = it->second;
      m_mutex.unlock();

      if (downloadItem == nullptr)
      {
        kodi::Log(ADDON_LOG_ERROR, "Download '%s' present on active downloads but no info class!", url.c_str());
        return;
      }

      if (downloadItem->IsActive())
      {
        kodi::QueueFormattedNotification(QUEUE_INFO, kodi::GetLocalizedString(30084).c_str(), suggested_name.c_str());
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
  std::string header = StringUtils::Format(kodi::GetLocalizedString(30082).c_str(), suggested_name.c_str());
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

  path += suggested_name;

  if (kodi::vfs::FileExists(path))
  {
    bool canceled;
    bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(header, kodi::GetLocalizedString(30085),
                                                          canceled, kodi::GetLocalizedString(30087),
                                                          kodi::GetLocalizedString(30086));
    if (!ret || canceled)
    {
      kodi::Log(ADDON_LOG_INFO, "Download of '%s' was already present on '%s' and is canceled", url.c_str(), path.c_str());
      return;
    }

    bool successed = kodi::vfs::DeleteFile(path);
    if (!successed)
    {
      kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30088),
                                              StringUtils::Format(kodi::GetLocalizedString(30089).c_str(), path.c_str()));
      return;
    }
  }

  kodi::Log(ADDON_LOG_INFO, "Download of '%s' with %li MBytes started", url.c_str(), totalBytes / 1024 / 1024);

  callback->Continue(path, false);
  downloadItem->SetActive(suggested_name, path);
}

void CWebBrowserDownloadHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item,
                                                   CefRefPtr<CefDownloadItemCallback> callback)
{
  std::string url = download_item->GetOriginalUrl().ToString();
  std::shared_ptr<CDownloadItem> downloadItem;

  {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_activeDownloads.find(url);
    if (it == m_activeDownloads.end())
    {
      downloadItem = std::make_shared<CDownloadItem>(url, callback);
      m_activeDownloads[url] = downloadItem;
    }
    else
      downloadItem = it->second;
  }

  if (!downloadItem->IsActive())
    return;

#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- IsValid: '%i'", __FUNCTION__, download_item->IsValid());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- IsInProgress: '%i'", __FUNCTION__, download_item->IsInProgress());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- IsComplete: '%i'", __FUNCTION__, download_item->IsComplete());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- IsCanceled: '%i'", __FUNCTION__, download_item->IsCanceled());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetCurrentSpeed: '%li'", __FUNCTION__, download_item->GetCurrentSpeed());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetPercentComplete: '%i'", __FUNCTION__, download_item->GetPercentComplete());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetTotalBytes: '%li'", __FUNCTION__, download_item->GetTotalBytes());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetReceivedBytes: '%li'", __FUNCTION__, download_item->GetReceivedBytes());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetStartTime: '%f'", __FUNCTION__, download_item->GetStartTime().GetDoubleT());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetEndTime: '%f'", __FUNCTION__, download_item->GetEndTime().GetDoubleT());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetFullPath: '%s'", __FUNCTION__, download_item->GetFullPath().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetId: '%i'", __FUNCTION__, download_item->GetId());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetURL: '%s'", __FUNCTION__, download_item->GetURL().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetOriginalUrl: '%s'", __FUNCTION__, download_item->GetOriginalUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetSuggestedFileName: '%s'", __FUNCTION__, download_item->GetSuggestedFileName().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetContentDisposition: '%s'", __FUNCTION__, download_item->GetContentDisposition().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s --- GetMimeType: '%s'", __FUNCTION__, download_item->GetMimeType().ToString().c_str());
#endif

  downloadItem->SetCanceled(download_item->IsCanceled());
  downloadItem->SetInProgress(download_item->IsInProgress());
  downloadItem->SetProgressText(download_item->GetTotalBytes() / 1024 / 1024,
                                download_item->GetReceivedBytes() / 1024 / 1024,
                                download_item->GetPercentComplete());

  if (download_item->IsComplete())
  {
    downloadItem->SetComplete();

    std::lock_guard<std::mutex> lock(m_mutex);
    m_finishedDownloads[url] = downloadItem;
    m_activeDownloads.erase(url);
    SaveDownloadHistory();

    kodi::Log(ADDON_LOG_INFO, "Download of '%s' finished", download_item->GetOriginalUrl().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Is valid: '%i'", download_item->IsValid());
    kodi::Log(ADDON_LOG_INFO, " - Is complete: '%i'", download_item->IsComplete());
    kodi::Log(ADDON_LOG_INFO, " - Is canceled: '%i'", download_item->IsCanceled());
    kodi::Log(ADDON_LOG_INFO, " - Total bytes: '%li'", download_item->GetTotalBytes());
    kodi::Log(ADDON_LOG_INFO, " - Received bytes: '%li'", download_item->GetReceivedBytes());
    kodi::Log(ADDON_LOG_INFO, " - Start time: '%f'", download_item->GetStartTime().GetDoubleT());
    kodi::Log(ADDON_LOG_INFO, " - End time: '%f'", download_item->GetEndTime().GetDoubleT());
    kodi::Log(ADDON_LOG_INFO, " - Full path: '%s'", download_item->GetFullPath().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - URL: '%s'", download_item->GetURL().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Original Url: '%s'", download_item->GetOriginalUrl().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Suggested file name: '%s'", download_item->GetSuggestedFileName().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Content disposition: '%s'", download_item->GetContentDisposition().ToString().c_str());
    kodi::Log(ADDON_LOG_INFO, " - Mime type: '%s'", download_item->GetMimeType().ToString().c_str());
  }

  UpdateEntry(downloadItem, download_item->IsComplete());
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
        kodi::Log(ADDON_LOG_ERROR, "failed to create initial settings data file at '%s'", strSettingsFile.c_str());
        return false;
      }
      return true;
    }
    else
      kodi::Log(ADDON_LOG_ERROR, "invalid settings data (no/invalid data file found at '%s')", strSettingsFile.c_str());
    return false;
  }

  TiXmlElement *pRootElement = xmlDoc.RootElement();
  if (strcmp(pRootElement->Value(), "downloadhistory") != 0)
  {
    if (!initial)
      kodi::Log(ADDON_LOG_ERROR, "invalid download history data (no <downloadhistory> tag found)");
    return false;
  }

  /* load history */
  TiXmlElement *pElement = pRootElement->FirstChildElement("histories");
  if (pElement)
  {
    TiXmlNode *pChannelNode = nullptr;
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

      std::shared_ptr<CDownloadItem> downloadItem = std::make_shared<CDownloadItem>(name, url, path, time);
      m_finishedDownloads[url] = downloadItem;
    }
  }

  return true;
}

bool CWebBrowserDownloadHandler::SaveDownloadHistory()
{
  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("downloadhistory");
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
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
        XMLUtils::SetLong(pHistoryNode, "time", entry.second->GetDownloadTime());
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
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
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

void CWebBrowserDownloadHandler::UpdateEntry(std::shared_ptr<CDownloadItem> downloadItem, bool complete)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  for (unsigned int i = 0; i < m_items.size(); ++i)
  {
    if (m_items[i]->GetPath() != downloadItem->GetPath())
      continue;

    kodi::gui::ListItemPtr item = GetListItem(i);
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
      item->SetLabel2(StringUtils::Format(kodi::GetLocalizedString(30095).c_str(), downloadItem->GetProcessText().c_str()));
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
    kodi::gui::ListItemPtr item(new kodi::gui::CListItem(file->GetName()));

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
      info = StringUtils::Format(kodi::GetLocalizedString(30095).c_str(), file->GetProcessText().c_str());
    else
      info = file->GetProcessText();

    item->SetLabel2(info);
    item->SetPath(file->GetPath());
    AddListItem(item);
  }

  return true;
}

bool CWebBrowserDownloadHandler::OnAction(int actionId, uint32_t buttoncode, wchar_t unicode)
{
  if (actionId == ACTION_PREVIOUS_MENU || actionId ==ACTION_NAV_BACK)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    ClearList();
    m_items.clear();
  }
  return CWindow::OnAction(actionId, buttoncode, unicode);
}

void CWebBrowserDownloadHandler::GetContextButtons(int itemNumber, std::vector< std::pair<unsigned int, std::string> > &buttons)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (m_items[itemNumber]->IsComplete())
    buttons.push_back(std::pair<unsigned int, std::string>(30091, kodi::GetLocalizedString(30091)));
  else
  {
    if (!m_items[itemNumber]->IsCanceled())
    {
      buttons.push_back(std::pair<unsigned int, std::string>(30092, kodi::GetLocalizedString(30092)));
      if (!m_items[itemNumber]->IsPaused())
        buttons.push_back(std::pair<unsigned int, std::string>(30093, kodi::GetLocalizedString(30093)));
      else
        buttons.push_back(std::pair<unsigned int, std::string>(30094, kodi::GetLocalizedString(30094)));
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
    bool canceled = false;
    std::string text = StringUtils::Format(kodi::GetLocalizedString(30098).c_str(), m_items[itemNumber]->GetName().c_str());
    bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30016), text, canceled,
                                                          kodi::GetLocalizedString(30018), kodi::GetLocalizedString(30017));
    if (canceled)
      return false;

    if (ret)
      kodi::vfs::DeleteFile(m_items[itemNumber]->GetPath());

    RemovedFinishedDownload(m_items[itemNumber]);
    OnInit();
  }
  return true;
}
