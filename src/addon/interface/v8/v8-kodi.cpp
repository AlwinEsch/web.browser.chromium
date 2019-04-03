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

#include "v8-kodi.h"

#include <kodi/General.h>

CV8Kodi::CV8Kodi(CWebBrowserClient* client) : m_client(client)
{

}

bool CV8Kodi::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
                                       CefRefPtr<CefProcessMessage> message)
{
  std::string funcName = message->GetArgumentList()->GetString(0);
  if (funcName == "Log")
  {
    AddonLog level = static_cast<AddonLog>(message->GetArgumentList()->GetInt(1));
    std::string text = message->GetArgumentList()->GetString(2);

    kodi::Log(level, text.c_str());
    return true;
  }
  else if (funcName == "QueueNotification")
  {
    QueueMsg type = static_cast<QueueMsg>(message->GetArgumentList()->GetInt(1));
    std::string header = message->GetArgumentList()->GetString(2);
    std::string text = message->GetArgumentList()->GetString(3);
    std::string imageFile = message->GetArgumentList()->GetString(4);
    int displayTime = message->GetArgumentList()->GetInt(5);
    bool withSound = message->GetArgumentList()->GetBool(6);
    int messageTime = message->GetArgumentList()->GetInt(7);

    kodi::QueueNotification(type, header, text, imageFile, displayTime, withSound, messageTime);
    return true;
  }
  return false;
}
