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

#include <string>

class CSettingsMain
{
public:
  CSettingsMain();
  virtual ~CSettingsMain();

  bool LoadSettings(void);
  bool SaveSettings(void);

  bool LoadUserSettings(void);
  bool SaveUserSettings(void);

  std::string GetStartURL() const;
  bool SetStartURL(std::string url);

private:
  std::string GetSettingsFile() const;
  std::string GetUserSettingsFile() const;

  std::string     m_baseSettingsFile;
  std::string     m_currentUserSettingsFile;

  std::string     m_startURL;
};
