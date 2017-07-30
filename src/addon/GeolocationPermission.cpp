/*
 *      Copyright (C) 2015-2017 Team KODI
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

#include <kodi/General.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/util/XMLUtils.h>
#include <p8-platform/util/StringUtils.h>

#include "include/wrapper/cef_helpers.h"

#include "GeolocationPermission.h"

#define GEOLOCATION_ASK   0
#define GEOLOCATION_ALLOW 1
#define GEOLOCATION_BLOCK 2

CWebBrowserGeolocationPermission::CWebBrowserGeolocationPermission()
{
  LoadGeolocationPermission(true);
}

bool CWebBrowserGeolocationPermission::OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser,
                                                                      const CefString& requesting_url,
                                                                      int request_id,
                                                                      CefRefPtr<CefGeolocationCallback> callback)
{
  CEF_REQUIRE_UI_THREAD();

  int locationUsage = kodi::GetSettingInt("security.location.ask");
  switch (locationUsage)
  {
    case GEOLOCATION_ALLOW:
    {
      kodi::Log(ADDON_LOG_INFO, "Url '%s' requested geolocation, and is always allowed", requesting_url.ToString().c_str());
      break;
    }

    case GEOLOCATION_BLOCK:
    {
      kodi::Log(ADDON_LOG_INFO, "Url '%s' requested geolocation, but becomes always blocked", requesting_url.ToString().c_str());
      return false;
    }

    case GEOLOCATION_ASK:
    default:
    {
      for (const auto allowed : m_allowedSides)
      {
        if (requesting_url == allowed)
        {
          callback->Continue(true);
          return true;
        }
      }
      for (const auto blocked : m_blockedSides)
      {
        if (requesting_url == blocked)
          return false;
      }

      bool canceled;
      std::string text = StringUtils::Format(kodi::GetLocalizedString(30029).c_str(), requesting_url.ToString().c_str());
      bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30028), text, canceled,
                                                            kodi::GetLocalizedString(30031), kodi::GetLocalizedString(30030));
      if (canceled)
      {
        kodi::Log(ADDON_LOG_INFO, "Url '%s' requested geolocation, but becomes canceled", requesting_url.ToString().c_str());
        return false;
      }

      if (ret)
      {
        kodi::Log(ADDON_LOG_INFO, "Url '%s' requested geolocation, and allowed forever", requesting_url.ToString().c_str());
        m_allowedSides.push_back(requesting_url.ToString());
        SaveGeolocationPermission();
      }
      else
      {
        kodi::Log(ADDON_LOG_INFO, "Url '%s' requested geolocation, and blocked forever", requesting_url.ToString().c_str());
        m_blockedSides.push_back(requesting_url.ToString());
        SaveGeolocationPermission();
        return false;
      }
      break;
    }
  }

  callback->Continue(true);
  return true;
}

bool CWebBrowserGeolocationPermission::LoadGeolocationPermission(bool initial)
{
  TiXmlDocument xmlDoc;
  std::string strSettingsFile = kodi::GetBaseUserPath("geolocation_permission.xml");

  if (!xmlDoc.LoadFile(strSettingsFile))
  {
    if (initial)
    {
      if (!SaveGeolocationPermission())
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
  if (strcmp(pRootElement->Value(), "geolocationpermission") != 0)
  {
    if (!initial)
      kodi::Log(ADDON_LOG_ERROR, "invalid geolocation permission data (no <geolocationpermission> tag found)");
    return false;
  }

  TiXmlElement *pAllowedElement = pRootElement->FirstChildElement("allowed");
  if (pAllowedElement)
  {
    TiXmlNode *pAllowedNode = nullptr;
    while ((pAllowedNode = pAllowedElement->IterateChildren(pAllowedNode)) != nullptr)
    {
      CStdString url;
      if (!XMLUtils::GetString(pAllowedNode, "url", url))
      {
        kodi::Log(ADDON_LOG_ERROR, "Allowed geolocation defined without url");
        continue;
      }
      m_allowedSides.push_back(url);
    }
  }

  TiXmlElement *pBlockedElement = pRootElement->FirstChildElement("blocked");
  if (pBlockedElement)
  {
    TiXmlNode *pBlockedNode = nullptr;
    while ((pBlockedNode = pBlockedElement->IterateChildren(pBlockedNode)) != nullptr)
    {
      CStdString url;
      if (!XMLUtils::GetString(pBlockedNode, "url", url))
      {
        kodi::Log(ADDON_LOG_ERROR, "Blocked geolocation defined without url");
        continue;
      }
      m_blockedSides.push_back(url);
    }
  }

  return true;
}

bool CWebBrowserGeolocationPermission::SaveGeolocationPermission()
{
  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("geolocationpermission");
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == nullptr)
    return false;

  TiXmlElement xmlAllowed("allowed");
  TiXmlNode* pAllowedNodes = pRoot->InsertEndChild(xmlAllowed);
  if (pAllowedNodes)
  {
    for (const auto& entry : m_allowedSides)
    {
      TiXmlElement xmlSetting("allow");
      TiXmlNode* pAllowedNode = pAllowedNodes->InsertEndChild(xmlSetting);
      if (pAllowedNode)
        XMLUtils::SetString(pAllowedNode, "url", entry.c_str());
    }
  }

  TiXmlElement xmlBlocked("blocked");
  TiXmlNode* pBlockedNodes = pRoot->InsertEndChild(xmlBlocked);
  if (pBlockedNodes)
  {
    for (const auto& entry : m_blockedSides)
    {
      TiXmlElement xmlSetting("allow");
      TiXmlNode* pBlockedNode = pBlockedNodes->InsertEndChild(xmlSetting);
      if (pBlockedNode)
        XMLUtils::SetString(pBlockedNode, "url", entry.c_str());
    }
  }

  std::string strSettingsFile = kodi::GetBaseUserPath("geolocation_permission.xml");
  if (!xmlDoc.SaveFile(strSettingsFile))
  {
    kodi::Log(ADDON_LOG_ERROR, "failed to write geolocation permission data");
    return false;
  }

  return true;
}

void CWebBrowserGeolocationPermission::ResetGeolocationPermission()
{
  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("geolocationpermission");
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == nullptr)
    return;

  TiXmlElement xmlAllowed("allowed");
  pRoot->InsertEndChild(xmlAllowed);

  TiXmlElement xmlBlocked("blocked");
  pRoot->InsertEndChild(xmlBlocked);

  std::string strSettingsFile = kodi::GetBaseUserPath("geolocation_permission.xml");
  if (!xmlDoc.SaveFile(strSettingsFile))
  {
    kodi::Log(ADDON_LOG_ERROR, "failed to write geolocation permission data");
    return;
  }

  m_allowedSides.clear();
  m_blockedSides.clear();
}
