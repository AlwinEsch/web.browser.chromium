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

#include "SandboxControl.h"

#include "utils/Utils.h"
#include "utils/StringUtils.h"

#include <kodi/General.h>
#include <kodi/Filesystem.h>
#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/gui/dialogs/Keyboard.h>
#if defined(TARGET_LINUX)
#include <unistd.h>
#endif

namespace SandboxControl
{

/*!
 * @note and @todo
 *
 * Check and confirm still needed?:
 * https://chromium.googlesource.com/chromium/src/+/master/docs/linux_suid_sandbox_development.md
 */
bool SetSandbox()
{
#if defined(TARGET_LINUX) && !defined(TARGET_ANDROID)
  std::string sandboxBinary = kodi::GetAddonPath("chrome-sandbox");

  struct stat st;
  if (sandboxBinary.empty() || stat(sandboxBinary.c_str(), &st) != 0)
  {
    kodi::gui::dialogs::OK::ShowAndGetInput(kodi::GetLocalizedString(30000), kodi::GetLocalizedString(30001));
    LOG_MESSAGE(ADDON_LOG_ERROR, "Web browser sandbox binary missing, add-on not usable!");
    return false;
  }

  bool bCanceled;
  if (access(sandboxBinary.c_str(), X_OK) != 0 || (st.st_uid != 0) ||
      ((st.st_mode & S_ISUID) == 0) || ((st.st_mode & S_IXOTH)) == 0)
  {
    if (!kodi::gui::dialogs::YesNo::ShowAndGetInput(kodi::GetLocalizedString(30000), kodi::GetLocalizedString(30002), bCanceled))
    {
      return false;
    }

    std::string command;
    std::string strPassword;
    for (int i = 0; i < 3; i++)
    {
      if (kodi::gui::dialogs::Keyboard::ShowAndGetNewPassword(strPassword, kodi::GetLocalizedString(30003), true))
      {
        if (stat("/usr/bin/sudo", &st) == 0 || stat("/bin/sudo", &st) == 0)
        {
          command = StringUtils::Format("echo %s | sudo -S bash -c \"chown root:root %s; sudo -- chmod 4755 %s\"",
                                            strPassword.c_str(),
                                            sandboxBinary.c_str(),
                                            sandboxBinary.c_str());
        }
        else if (stat("/usr/bin/su", &st) == 0 || stat("/bin/su", &st) == 0)
        {
          command = StringUtils::Format("echo %s | su root -c bash -c \"chown root:root %s; sudo -- chmod 4755 %s\"",
                                            strPassword.c_str(),
                                            sandboxBinary.c_str(),
                                            sandboxBinary.c_str());
        }
        else
        {
          LOG_MESSAGE(ADDON_LOG_ERROR, "No super user application found to change chrome-sandbox rights!");
          break;
        }
        if (system(command.c_str()) == 0)
          return true;
      }
      else
        break;
    }
    return false;
  }
#endif
  return true;
}

} /* namespace SandboxControl */
