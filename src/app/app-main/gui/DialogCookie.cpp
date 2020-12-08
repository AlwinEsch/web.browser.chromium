/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogCookie.h"

// CEF
#include "include/cef_waitable_event.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/ListItem.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/ExtendedProgress.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/Keyboard.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/OK.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// Global
#include <iomanip>
#include <thread>

/*
TODO:
Seems final here.
*/

using kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

class CDeleteCookiesCallback : public CefDeleteCookiesCallback
{
public:
  CDeleteCookiesCallback(CBrowserDialogCookie* cookieHandler)
    : m_handler(cookieHandler), m_progress(kodi::GetLocalizedString(30320))
  {
    m_progress.SetText(kodi::GetLocalizedString(30321));
  }

  ~CDeleteCookiesCallback() override = default;

  void OnComplete(int num_deleted) override
  {
    m_handler->OnInit();
    m_progress.MarkFinished();
    std::string dialogText =
        StringUtils::Format(kodi::GetLocalizedString(30322).c_str(), num_deleted);
    kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30300), dialogText);
  }

private:
  CBrowserDialogCookie* m_handler;
  kodi::gui::dialogs::CExtendedProgress m_progress;
  IMPLEMENT_REFCOUNTING(CDeleteCookiesCallback);
};

//------------------------------------------------------------------------------

class CCookieVisitor : public CefCookieVisitor
{
public:
  CCookieVisitor(CBrowserDialogCookie* cookieHandler, CefCookie* deleteThis = nullptr)
    : m_handler(cookieHandler), m_delete(deleteThis != nullptr)
  {
    if (m_delete)
    {
      m_deleteName = CefString(&deleteThis->name).ToString();
      m_deleteDomain = CefString(&deleteThis->domain).ToString();
      m_deleteContent = CefString(&deleteThis->value).ToString();
    }
  }

  CCookieVisitor(CBrowserDialogCookie* cookieHandler, const std::string& domain)
    : m_handler(cookieHandler), m_delete(true), m_deleteDomain(domain)
  {
  }

  ~CCookieVisitor() override = default;

  bool Visit(const CefCookie& cookie, int count, int total, bool& deleteCookie) override
  {
    if (m_delete &&
        (!m_deleteName.empty() ? m_deleteName == CefString(&cookie.name).ToString() : true) &&
        (!m_deleteDomain.empty() ? m_deleteDomain == CefString(&cookie.domain).ToString() : true) &&
        (!m_deleteContent.empty() ? m_deleteContent == CefString(&cookie.value).ToString() : true))
    {
      deleteCookie = true;
    }
    else
    {
      deleteCookie = false;
      m_handler->AddCookie(cookie);
    }
    return true;
  }

private:
  CBrowserDialogCookie* m_handler;
  bool m_delete;
  std::string m_deleteName;
  std::string m_deleteDomain;
  std::string m_deleteContent;
  IMPLEMENT_REFCOUNTING(CCookieVisitor);
};

//------------------------------------------------------------------------------

CBrowserDialogCookie::CBrowserDialogCookie()
  : CWindow("DialogCookies.xml", "skin.estuary", true), m_inited(false), m_findPosition(-1)
{
}

void CBrowserDialogCookie::AddCookie(const CefCookie& cookie)
{
  m_mutex.lock();
  m_items.emplace_back(cookie);
  AddGUIEntry(cookie);
  m_mutex.unlock();
  MarkDirtyRegion();
}

void CBrowserDialogCookie::AddGUIEntry(const CefCookie& cookie)
{
  if (!m_inited)
    return;

  std::shared_ptr<kodi::gui::CListItem> item(
      new kodi::gui::CListItem(CefString(&cookie.name).ToString()));

  item->SetProperty("content", CefString(&cookie.value).ToString());
  item->SetProperty("domain", CefString(&cookie.domain).ToString());
  item->SetProperty("path", CefString(&cookie.path).ToString());
  item->SetProperty("secure", cookie.secure ? kodi::GetLocalizedString(30311)
                                            : kodi::GetLocalizedString(30312));
  item->SetProperty("forscripts", !cookie.httponly ? kodi::GetLocalizedString(30311)
                                                   : kodi::GetLocalizedString(30312));
  item->SetProperty("hasexpires", cookie.has_expires ? kodi::GetLocalizedString(30311)
                                                     : kodi::GetLocalizedString(30312));

  std::time_t time;
  cef_time_to_timet(&cookie.creation, &time);
  auto tm = *std::localtime(&time);
  std::string format = kodi::GetRegion("datelong") + " - " + kodi::GetRegion("time");
  std::ostringstream oss;
  oss << std::put_time(&tm, format.c_str());
  item->SetProperty("creation", oss.str());

  cef_time_to_timet(&cookie.last_access, &time);
  tm = *std::localtime(&time);
  format = kodi::GetRegion("datelong") + " - " + kodi::GetRegion("time");
  std::ostringstream oss2;
  oss2 << std::put_time(&tm, format.c_str());
  item->SetProperty("lastaccess", oss2.str());

  cef_time_to_timet(&cookie.expires, &time);
  tm = *std::localtime(&time);
  format = kodi::GetRegion("datelong") + " - " + kodi::GetRegion("time");
  std::ostringstream oss3;
  oss3 << std::put_time(&tm, format.c_str());
  item->SetProperty("expires", oss3.str());

  AddListItem(item);
}

bool CBrowserDialogCookie::OnInit()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_inited = true;
  ClearList();
  for (const auto& cookie : m_items)
  {
    AddGUIEntry(cookie);
  }

  return false;
}

void CBrowserDialogCookie::Open()
{
  CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
  bool ret = manager->VisitAllCookies(new CCookieVisitor(this));
  if (!ret)
  {
    kodi::Log(ADDON_LOG_ERROR, "Cookies can't be accessed");
    return;
  }
  Show();
}

bool CBrowserDialogCookie::OnClick(int controlId)
{

  return false;
}

#define COOKIE_CONTEXT_MENU__DELETE_SELECTED 0
#define COOKIE_CONTEXT_MENU__DELETE_DOMAIN 1
#define COOKIE_CONTEXT_MENU__DELETE_ALL 2
#define COOKIE_CONTEXT_MENU__SEARCH 3
#define COOKIE_CONTEXT_MENU__SEARCH_CONTNUE 4
#define COOKIE_CONTEXT_MENU__OPEN_SETTINGS 5

void CBrowserDialogCookie::GetContextButtons(
    int itemNumber, std::vector<std::pair<unsigned int, std::string>>& buttons)
{
  if (itemNumber >= 0)
  {
    buttons.emplace_back(std::pair<unsigned int, std::string>(COOKIE_CONTEXT_MENU__DELETE_SELECTED,
                                                              kodi::GetLocalizedString(30313)));
    buttons.emplace_back(std::pair<unsigned int, std::string>(COOKIE_CONTEXT_MENU__DELETE_DOMAIN,
                                                              kodi::GetLocalizedString(30319)));
    buttons.emplace_back(std::pair<unsigned int, std::string>(COOKIE_CONTEXT_MENU__DELETE_ALL,
                                                              kodi::GetLocalizedString(30314)));
    buttons.emplace_back(std::pair<unsigned int, std::string>(COOKIE_CONTEXT_MENU__SEARCH,
                                                              kodi::GetLocalizedString(30315)));
    if (m_findPosition >= 0)
      buttons.emplace_back(std::pair<unsigned int, std::string>(COOKIE_CONTEXT_MENU__SEARCH_CONTNUE,
                                                                kodi::GetLocalizedString(30318)));
    buttons.emplace_back(std::pair<unsigned int, std::string>(COOKIE_CONTEXT_MENU__OPEN_SETTINGS,
                                                              kodi::GetLocalizedString(30316)));
  }
}

bool CBrowserDialogCookie::OnContextButton(int itemNumber, unsigned int button)
{
  CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);

  switch (button)
  {
    case COOKIE_CONTEXT_MENU__DELETE_SELECTED:
    {
      if (itemNumber >= 0)
      {
        CefCookie cookie = m_items[itemNumber];
        m_items.clear();
        ClearList();
        manager->VisitAllCookies(new CCookieVisitor(this, &cookie));
      }
      break;
    }
    case COOKIE_CONTEXT_MENU__DELETE_DOMAIN:
    {
      if (itemNumber >= 0)
      {
        std::string domain = CefString(&m_items[itemNumber].domain).ToString();
        m_items.clear();
        ClearList();
        manager->VisitAllCookies(new CCookieVisitor(this, domain));
      }
      break;
    }
    case COOKIE_CONTEXT_MENU__DELETE_ALL:
    {
      ClearList();
      manager->DeleteCookies("", "", new CDeleteCookiesCallback(this));
      break;
    }
    case COOKIE_CONTEXT_MENU__SEARCH:
    case COOKIE_CONTEXT_MENU__SEARCH_CONTNUE:
    {
      // Kodi dialogs must be always called independent as they blocks addon main thread.
      std::thread([this, button] {
        if (button == COOKIE_CONTEXT_MENU__SEARCH)
        {
          m_findPosition = 0;
          kodi::gui::dialogs::Keyboard::ShowAndGetInput(m_lastSearchText,
                                                        kodi::GetLocalizedString(30315), true);
        }
        else
        {
          ++m_findPosition;
        }

        if (!m_lastSearchText.empty() && m_findPosition >= 0)
        {
          std::string search = m_lastSearchText;
          StringUtils::ToLower(search);

          std::string text;
          std::size_t found = std::string::npos;
          for (; m_findPosition < static_cast<int>(m_items.size()); ++m_findPosition)
          {
            text = CefString(&m_items[m_findPosition].name).ToString();
            StringUtils::ToLower(text);
            found = text.find(search);
            if (found == std::string::npos)
            {
              text = CefString(&m_items[m_findPosition].domain).ToString();
              StringUtils::ToLower(text);
              found = text.find(search);
            }
            if (found == std::string::npos)
            {
              text = CefString(&m_items[m_findPosition].path).ToString();
              StringUtils::ToLower(text);
              found = text.find(search);
            }
            if (found == std::string::npos)
              continue;

            SetCurrentListPosition(m_findPosition);
            break;
          }
          if (found == std::string::npos)
          {
            std::string dialogText = StringUtils::Format(kodi::GetLocalizedString(30317).c_str(),
                                                         m_lastSearchText.c_str());
            kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30315), dialogText);
            m_findPosition = -1;
          }
        }
      }).detach();
      break;
    }
    case COOKIE_CONTEXT_MENU__OPEN_SETTINGS:
      // Kodi dialogs must be always called independent as they blocks addon main thread.
      std::thread([] {
        kodi::OpenSettings();
      }).detach();
      break;
    default:
      break;
  }
  return true;
}

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
