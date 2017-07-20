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

#include "kodi/util/XMLUtils.h"
#include "p8-platform/util/StdString.h"
#include "p8-platform/util/util.h"
#include "p8-platform/util/timeutils.h"

#include "include/internal/cef_types.h"

#include "SettingsMain.h"
#include "addon.h"

using namespace ADDON;
using namespace P8PLATFORM;

CSettingsMain::CSettingsMain() :
  m_startURL(""),
  m_allowGeolocation(false),
  m_mouseCursorChangeDisabled(false),
  m_logLevelCEF(LOGSEVERITY_VERBOSE)
{
  m_baseSettingsFile = GetSettingsFile();
  m_currentUserSettingsFile = GetUserSettingsFile();
}

CSettingsMain::~CSettingsMain()
{

}

bool CSettingsMain::LoadSettings(void)
{
  TiXmlDocument xmlDoc;
  if (!xmlDoc.LoadFile(m_baseSettingsFile))
  {
    if (!SaveSettings())
    {
      KODI->Log(LOG_ERROR, "invalid web browser data (no/invalid data file found at '%s')", m_baseSettingsFile.c_str());
      return false;
    }
  }

  TiXmlElement *pRootElement = xmlDoc.RootElement();
  if (strcmp(pRootElement->Value(), "main") != 0)
  {
    KODI->Log(LOG_ERROR, "invalid web browser data (no <radio> tag found)");
    return false;
  }

  /* default start url */
  CStdString strTmp;
  if (!XMLUtils::GetString(pRootElement, "starturl", strTmp))
    m_startURL = "";
  else
    m_startURL = strTmp;

  if (!XMLUtils::GetBoolean(pRootElement, "mousecursorchangedisabled", m_mouseCursorChangeDisabled))
    m_mouseCursorChangeDisabled = false;
  if (!XMLUtils::GetInt(pRootElement, "loglevelcef", (int&)m_logLevelCEF))
    m_logLevelCEF = LOGSEVERITY_VERBOSE;
}

bool CSettingsMain::SaveSettings(void)
{
  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("main");
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == NULL)
    return false;

  XMLUtils::SetString(pRoot, "starturl", m_startURL);
  XMLUtils::SetBoolean(pRoot, "mousecursorchangedisabled", m_mouseCursorChangeDisabled);
  XMLUtils::SetInt(pRoot, "loglevelcef", (int&)m_logLevelCEF);
  if (!xmlDoc.SaveFile(m_baseSettingsFile))
  {
    KODI->Log(LOG_ERROR, "failed to write web browser settings data");
    return false;
  }

  return true;
}

bool CSettingsMain::LoadUserSettings(void)
{
  TiXmlDocument xmlDoc;
  if (!xmlDoc.LoadFile(m_currentUserSettingsFile))
  {
    if (!SaveUserSettings())
    {
      KODI->Log(LOG_ERROR, "invalid web browser data (no/invalid data file found at '%s')", m_currentUserSettingsFile.c_str());
      return false;
    }
  }

  TiXmlElement *pRootElement = xmlDoc.RootElement();
  if (strcmp(pRootElement->Value(), "user") != 0)
  {
    KODI->Log(LOG_ERROR, "invalid web browser data (no <radio> tag found)");
    return false;
  }

  /* default start url */
  if (!XMLUtils::GetBoolean(pRootElement, "allowgeolocation", m_allowGeolocation))
    m_allowGeolocation = false;
}

bool CSettingsMain::SaveUserSettings(void)
{
  if (!KODI->DirectoryExists(g_strUserPath.c_str()))
  {
    if (!KODI->CreateDirectory(g_strUserPath.c_str()))
    {
      KODI->Log(LOG_ERROR, "failed to create user folder '%s' for web browser settings data", g_strUserPath.c_str());
      return false;
    }
  }

  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("user");
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == NULL)
    return false;

  XMLUtils::SetBoolean(pRoot, "allowgeolocation", m_allowGeolocation);

  if (!xmlDoc.SaveFile(m_currentUserSettingsFile))
  {
    KODI->Log(LOG_ERROR, "failed to write web browser settings data");
    return false;
  }

  return true;
}

std::string CSettingsMain::StartURL() const
{
  return m_startURL;
}

bool CSettingsMain::SetStartURL(std::string url)
{
  m_startURL = url;
  return true;
}

std::string CSettingsMain::GetSettingsFile() const
{
  std::string settingFile = g_strAddonSharePath;
  if (settingFile.at(settingFile.size() - 1) == '\\' ||
      settingFile.at(settingFile.size() - 1) == '/')
    settingFile.append("webBrowserSettings.xml");
  else
    settingFile.append("/webBrowserSettings.xml");
  return settingFile;
}

std::string CSettingsMain::GetUserSettingsFile() const
{
  std::string settingFile = g_strUserPath;
  if (settingFile.at(settingFile.size() - 1) == '\\' ||
      settingFile.at(settingFile.size() - 1) == '/')
    settingFile.append("webSettings.xml");
  else
    settingFile.append("/webSettings.xml");
  return settingFile;
}
