/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Handler.h"

#include "WebBrowserClient.h"
#include "utils/Utils.h"

#include <kodi/General.h>
#include <thread>
#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>

const std::string& msg_kodi_log = "kodi.Log";
const std::string& msg_queue_notification = "kodi.QueueNotification";
const std::string& msg_get_addon_info = "kodi.GetAddonInfo";
const std::string& msg_kodi_gui_dialogs_OK_ShowAndGetInput = "kodi.gui.dialog.OK.ShowAndGetInput";
const std::string& msg_kodi_gui_dialogs_YesNo_ShowAndGetInput = "kodi.gui.dialog.YesNo.ShowAndGetInput";

CJSHandler::CJSHandler(CefRefPtr<CWebBrowserClient> client)
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

  std::thread(OnQueryProcess, frame->GetURL().ToString(), query_id, request.ToString(), persistent, callback).detach();

  return true;
}

void CJSHandler::OnQueryProcess(std::string url, int64 query_id,
                                std::string request, bool persistent, CefRefPtr<Callback> callback)
{
  const std::string& message_name = request;
  if (message_name.find(msg_kodi_log) == 0)
  {
    std::string level = message_name.substr(msg_kodi_log.size()+1);
    size_t delim = level.find(':');
    if (delim == std::string::npos)
      return;

    std::string message = url + " - " + level.substr(delim + 1);
    level = level.substr(0, delim);

    kodi::Log(static_cast<AddonLog>(std::atoi(level.c_str())), message.c_str());
  }
  else if (message_name.find(msg_queue_notification) == 0)
  {
    size_t delim = msg_queue_notification.size();

    std::string msg = message_name;
    std::string type;
    if (!ParseString(msg, type, delim))
      return;

    std::string header;
    if (!ParseString(type, header, delim))
      return;

    std::string message;
    if (!ParseString(header, message, delim))
      return;

    std::string imageFile;
    if (!ParseString(message, imageFile, delim))
      return;
    if (imageFile == "undefined")
      imageFile.clear();

    std::string displayTime;
    if (!ParseString(imageFile, displayTime, delim))
      return;

    std::string withSound;
    if (!ParseString(displayTime, withSound, delim))
      return;

    std::string messageTime;
    ParseString(withSound, messageTime, delim);

    kodi::QueueNotification(static_cast<QueueMsg>(std::stoi(type)), header, message, imageFile,
                            std::stoi(displayTime), withSound == "true" ? true : false, std::stoi(messageTime));
  }
  else if (message_name.find(msg_get_addon_info) == 0)
  {
    std::string ret;
    std::string id = message_name.substr(msg_get_addon_info.size()+1);

    if (!id.empty())
      ret = kodi::GetAddonInfo(id);

    if (!ret.empty())
      callback->Success(ret);
    else
      callback->Failure(ADDON_LOG_ERROR, CefString("Called '" + msg_get_addon_info + "' with invalid id"));
  }
  else if (message_name.find(msg_kodi_gui_dialogs_OK_ShowAndGetInput) == 0)
  {
    std::string text = message_name.substr(msg_kodi_gui_dialogs_OK_ShowAndGetInput.size()+1);
    size_t delim = text.find(':');
    if (delim == std::string::npos)
      return;

    std::string message = text.substr(delim + 1);
    text = text.substr(0, delim);

    kodi::gui::dialogs::OK::ShowAndGetInput(text, message);
    callback->Success(CefString());
  }
  else if (message_name.find(msg_kodi_gui_dialogs_YesNo_ShowAndGetInput) == 0)
  {
    std::string text = message_name.substr(msg_kodi_gui_dialogs_YesNo_ShowAndGetInput.size()+1);
    size_t delim = text.find(':');
    if (delim == std::string::npos)
      return;

    std::string message = text.substr(delim + 1);
    text = text.substr(0, delim);

    bool canceled = false;
    bool ret = kodi::gui::dialogs::YesNo::ShowAndGetInput(text, message, canceled);
    if (ret)
      callback->Success(CefString());
    else
      callback->Failure(canceled, CefString());
  }
}

void CJSHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "-----------------%s", __FUNCTION__);
}

