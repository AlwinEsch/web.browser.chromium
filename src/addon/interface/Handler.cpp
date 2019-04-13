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

#include "Handler.h"

#include "WebBrowserClient.h"
#include "Utils.h"

#include <kodi/General.h>

const std::string& msg_kodi_log = "kodi.Log";
const std::string& msg_queue_notification = "kodi.QueueNotification";

CJSHandler::CJSHandler(CWebBrowserClient* client)
  : m_client(client)
{

}

bool CJSHandler::ParseString(std::string& in, std::string& out, size_t& delim)
{
  out = in.substr(delim + 1);
  in = in.substr(0, delim);

  delim = out.find(':');
  if (delim == std::string::npos)
    return false;

  return true;
}

bool CJSHandler::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id,
                         const CefString& request, bool persistent, CefRefPtr<Callback> callback)
{
  CEF_REQUIRE_UI_THREAD();

  bool ret = false;
  const std::string& message_name = request;
  if (message_name.find(msg_kodi_log) == 0)
  {
    std::string level = message_name.substr(msg_kodi_log.size()+1);
    size_t delim = level.find(':');
    if (delim == std::string::npos)
      return false;

    std::string message = frame->GetURL().ToString() + " - " + level.substr(delim + 1);
    level = level.substr(0, delim);

    fprintf(stderr, "---- %s %s\n", level.c_str(), message.c_str());

    kodi::Log(static_cast<AddonLog>(std::atoi(level.c_str())), message.c_str());
//     callback->Continue("Incorrect message format");
    return true;
  }
  else if (message_name.find(msg_queue_notification) == 0)
  {
    size_t delim = msg_queue_notification.size();

    std::string msg = message_name;
    std::string type;
    if (!ParseString(msg, type, delim))
      return false;

    std::string header;
    if (!ParseString(type, header, delim))
      return false;

    std::string message;
    if (!ParseString(header, message, delim))
      return false;

    std::string imageFile;
    if (!ParseString(message, imageFile, delim))
      return false;
    if (imageFile == "undefined")
      imageFile.clear();

    std::string displayTime;
    if (!ParseString(imageFile, displayTime, delim))
      return false;

    std::string withSound;
    if (!ParseString(displayTime, withSound, delim))
      return false;

    std::string messageTime;
    ParseString(withSound, messageTime, delim);

    kodi::QueueNotification(static_cast<QueueMsg>(std::stoi(type)), header, message, imageFile,
                            std::stoi(displayTime), withSound == "true" ? true : false, std::stoi(messageTime));
  }

  callback->Success(CefString());

  return true;
}

void CJSHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "-----------------%s", __FUNCTION__);
}
