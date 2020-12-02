/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "v8-kodi.h"
#include "../../WebBrowserClient.h"

#include "../../../../../lib/kodi-dev-kit/include/kodi/General.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace interface
{
namespace v8
{

CV8Kodi::CV8Kodi(CefRefPtr<CWebBrowserClient> client) : m_client(client)
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

} /* namespace v8 */
} /* namespace interface */
} /* main */
} /* app */
} /* chromium */
