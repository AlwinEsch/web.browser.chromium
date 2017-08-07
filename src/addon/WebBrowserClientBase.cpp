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

#include <stdio.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#undef Success     // Definition conflicts with cef_message_router.h
#undef RootWindow  // Definition conflicts with root_window.h

#include <kodi/ActionIDs.h>
#include <kodi/Filesystem.h>
#include <kodi/General.h>
#include <kodi/XBMC_vkeys.h>
#include <kodi/gui/dialogs/ContextMenu.h>
#include <kodi/gui/dialogs/FileBrowser.h>
#include <kodi/gui/dialogs/Keyboard.h>
#include <kodi/gui/dialogs/Select.h>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_parser.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/views/cef_textfield.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include "p8-platform/util/StringUtils.h"

#include "addon.h"
#include "WebBrowserClientBase.h"
#include "WebBrowserManager.h"
#include "URICheckHandler.h"
#include "Utils.h"
#include "DOMVisitor.h"
#include "MessageIds.h"
#include "SystemTranslator.h"
#include "JSInterface/Handler.h"
#include "JSInterface/JSDialogHandler.h"



#define ZOOM_MULTIPLY 25.0

using namespace std;
using namespace P8PLATFORM;


enum DOMTestType {
  DOM_TEST_STRUCTURE,
  DOM_TEST_MODIFY,
};


// Custom menu command Ids.
enum client_menu_ids {
  CLIENT_ID_OPEN_SELECTED_SIDE = MENU_ID_USER_FIRST,
  CLIENT_ID_OPEN_KEYBOARD,
};


//#define ACTION_MOUSE_START            100
//#define ACTION_MOUSE_LEFT_CLICK       100
//#define ACTION_MOUSE_RIGHT_CLICK      101
//#define ACTION_MOUSE_MIDDLE_CLICK     102
//#define ACTION_MOUSE_DOUBLE_CLICK     103
//#define ACTION_MOUSE_WHEEL_UP         104
//#define ACTION_MOUSE_WHEEL_DOWN       105
//#define ACTION_MOUSE_DRAG             106
//#define ACTION_MOUSE_MOVE             107
//#define ACTION_MOUSE_LONG_CLICK       108
//#define ACTION_MOUSE_END              109

int GetCefStateModifiers(int actionId)
{
  int modifiers = 0;
  switch (actionId)
  {
    case ACTION_MOUSE_LEFT_CLICK:
      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
      fprintf(stderr, "ACTION_MOUSE_LEFT_CLICK\n");
      break;
    case ACTION_MOUSE_RIGHT_CLICK:
      modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
      fprintf(stderr, "ACTION_MOUSE_RIGHT_CLICK\n");
      break;
    case ACTION_MOUSE_MIDDLE_CLICK:
      modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
      fprintf(stderr, "EVENTFLAG_MIDDLE_MOUSE_BUTTON\n");
      break;
    case ACTION_MOUSE_DOUBLE_CLICK:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ACTION_MOUSE_DOUBLE_CLICK\n");
      break;
    case ACTION_MOUSE_WHEEL_UP:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ACTION_MOUSE_WHEEL_UP\n");
      break;
    case ACTION_MOUSE_WHEEL_DOWN:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ACTION_MOUSE_WHEEL_DOWN\n");
      break;
    case ACTION_MOUSE_DRAG:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ACTION_MOUSE_DRAG\n");
      break;
    case ACTION_MOUSE_MOVE:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ACTION_MOUSE_MOVE\n");
      break;
    case ACTION_MOUSE_LONG_CLICK:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ACTION_MOUSE_LONG_CLICK\n");
      break;
    default:
      break;
  }



//  if (state & GDK_SHIFT_MASK)
////    modifiers |= EVENTFLAG_SHIFT_DOWN;
////  if (state & GDK_LOCK_MASK)
////    modifiers |= EVENTFLAG_CAPS_LOCK_ON;
////  if (state & GDK_CONTROL_MASK)
////    modifiers |= EVENTFLAG_CONTROL_DOWN;
////  if (state & GDK_MOD1_MASK)
////    modifiers |= EVENTFLAG_ALT_DOWN;
////  if (state & GDK_BUTTON1_MASK)
////    modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
////  if (state & GDK_BUTTON2_MASK)
////    modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
////  if (state & GDK_BUTTON3_MASK)
////    modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
  return modifiers;
}



















// class ClientDownloadImageCallback : public CefDownloadImageCallback
// {
//  public:
//   explicit ClientDownloadImageCallback() {}
//
//   void OnDownloadImageFinished(const CefString& image_url,
//                                int http_status_code,
//                                CefRefPtr<CefImage> image) OVERRIDE
//   {
//
//   }
//
//  private:
//   IMPLEMENT_REFCOUNTING(ClientDownloadImageCallback);
//   DISALLOW_COPY_AND_ASSIGN(ClientDownloadImageCallback);
// };

CWebBrowserClientBase::CWebBrowserClientBase(KODI_HANDLE handle, int iUniqueClientId, CWebBrowser* instance)
  : CWebControl(handle, iUniqueClientId),
    m_mainBrowserHandler(instance),
    m_iUniqueClientId(iUniqueClientId),
    m_iBrowserCount(0),
    m_bIsDirty(false),
    m_iMousePreviousFlags(0),
    m_iMousePreviousControl(MBT_LEFT),
    m_BrowserId(-1),
    m_Browser(NULL),
    m_isClosing(false),
    m_isFullScreen(false),
    m_focusedField{0},
    m_isLoading{false}
{
  uint32_t color = GetBackgroundColorARGB();
  m_BackgroundColor[3] = float(CefColorGetA(color)) / 255.0f;
  m_BackgroundColor[2] = float(CefColorGetR(color)) / 255.0f;
  m_BackgroundColor[1] = float(CefColorGetG(color)) / 255.0f;
  m_BackgroundColor[0] = float(CefColorGetB(color)) / 255.0f;
  m_fMouseXScaleFactor = (GetXPos() + GetWidth()) / (GetSkinXPos() + GetSkinWidth());
  m_fMouseYScaleFactor = (GetYPos() + GetHeight()) / (GetSkinYPos() + GetSkinHeight());

  // CEF related sub classes to manage web parts
  m_resourceManager     = new CefResourceManager();


  fprintf(stderr, " -1- %s ---------- %i\n", __PRETTY_FUNCTION__, m_iUniqueClientId);
  fprintf(stderr, " -2- %s ---------- %i\n", __PRETTY_FUNCTION__, GetDataIdentifier());

  // Create the browser-side router for query handling.
  m_jsDialogHandler = new CJSDialogHandler(this);
  m_jsHandler = new CJSHandler(this);
  CefMessageRouterConfig config;
  m_messageRouter = CefMessageRouterBrowserSide::Create(config);
  m_messageRouter->AddHandler(m_jsHandler, false);
}

CWebBrowserClientBase::~CWebBrowserClientBase()
{
  delete m_jsHandler;
}

CefRefPtr<CefDialogHandler> CWebBrowserClientBase::GetDialogHandler()
{
  return m_mainBrowserHandler->GetUploadHandler();
}

CefRefPtr<CefDownloadHandler> CWebBrowserClientBase::GetDownloadHandler()
{
  return m_mainBrowserHandler->GetDownloadHandler();
}

CefRefPtr<CefGeolocationHandler> CWebBrowserClientBase::GetGeolocationHandler()
{
  return m_mainBrowserHandler->GetGeolocationPermission();
}

CefRefPtr<CefDisplayHandler> CWebBrowserClientBase::GetDisplayHandler()
{
  return this;
}

CefRefPtr<CefJSDialogHandler> CWebBrowserClientBase::GetJSDialogHandler()
{
  return m_jsDialogHandler;
}

CefRefPtr<CefContextMenuHandler> CWebBrowserClientBase::GetContextMenuHandler()
{
  return this;
}

CefRefPtr<CefLoadHandler> CWebBrowserClientBase::GetLoadHandler()
{
  return this;
}

/*!
 * @brief CefDisplayHandler methods
 */
//@{
void CWebBrowserClientBase::OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url)
{
  if (frame->IsMain())
  {
    Message tMsg = {TMSG_SET_OPENED_ADDRESS};
    tMsg.strParam = url.ToString().c_str();
    SendMessage(tMsg, false);
  }
}

void CWebBrowserClientBase::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
  if (m_lastTitle != title.ToString().c_str())
  {
    m_lastTitle = title.ToString().c_str();

    Message tMsg = {TMSG_SET_OPENED_TITLE};
    tMsg.strParam = m_lastTitle.c_str();
    SendMessage(tMsg, false);
  }
}

void CWebBrowserClientBase::OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "From currently opened web site given icon urls (first one used)");
  unsigned int listSize = icon_urls.size();
  for (unsigned int i = 0; i < listSize; ++i)
    LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, " - Icon %i - %s", i+1, icon_urls[i].ToString().c_str());

  Message tMsg = {TMSG_SET_ICON_URL};
  if (listSize > 0)
    tMsg.strParam = icon_urls[0].ToString().c_str();
  else
    tMsg.strParam = "";
  SendMessage(tMsg, false);
}

void CWebBrowserClientBase::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
{
  if (m_isFullScreen != fullscreen)
  {
    m_isFullScreen = fullscreen;

    Message tMsg = {TMSG_FULLSCREEN_MODE_CHANGE};
    tMsg.param1 = fullscreen ? 1 : 0;
    SendMessage(tMsg, false);
  }
}

bool CWebBrowserClientBase::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text)
{
  if (m_lastTooltip != text.ToString().c_str())
  {
    m_lastTooltip = text.ToString().c_str();

    Message tMsg = {TMSG_SET_TOOLTIP};
    tMsg.strParam = m_lastTooltip.c_str();
    SendMessage(tMsg, false);
  }

  return true;
}

void CWebBrowserClientBase::OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value)
{
  if (m_lastStatusMsg != value.ToString().c_str())
  {
    m_lastStatusMsg = value.ToString().c_str();

    Message tMsg = {TMSG_SET_STATUS_MESSAGE};
    tMsg.strParam = m_lastStatusMsg.c_str();
    SendMessage(tMsg, false);
  }
}

bool CWebBrowserClientBase::OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line)
{
  LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Message: %s - Source: %s - Line: %i", __FUNCTION__,
                       message.ToString().c_str(), source.ToString().c_str(), line);
  return true;
}
//@}

void CWebBrowserClientBase::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
//   fprintf(stderr, " -1- %s ---------- %i\n", __PRETTY_FUNCTION__, m_iUniqueClientId);
//   fprintf(stderr, " -2- %s ---------- %i\n", __PRETTY_FUNCTION__, GetDataIdentifier());
}

void CWebBrowserClientBase::HandleMessages()
{
  // process threadmessages
  CLockObject lock(m_Mutex);
  while (!m_processQueue.empty())
  {
    Message* pMsg = m_processQueue.front();
    m_processQueue.pop();

    std::shared_ptr<CEvent> waitEvent = pMsg->waitEvent;
    lock.Unlock(); // <- see the large comment in SendMessage ^
    switch (pMsg->dwMessage)
    {
      case TMSG_SET_CONTROL_READY:
      {
        LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Web control %s", pMsg->param1 ? "ready" : "failed");
        SetControlReady(pMsg->param1);
        break;
      }
      case TMSG_SET_OPENED_ADDRESS:
      {
        LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Opened web site url '%s'", pMsg->strParam.c_str());
        SetOpenedAddress(pMsg->strParam);
        break;
      }
      case TMSG_SET_OPENED_TITLE:
      {
        LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Opened web site title '%s'", pMsg->strParam.c_str());
        SetOpenedTitle(pMsg->strParam);
        break;
      }
      case TMSG_SET_ICON_URL:
      {
        LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Opened web site set icon url '%s'", pMsg->strParam.c_str());
        SetIconURL(pMsg->strParam);
        break;
      }
      case TMSG_FULLSCREEN_MODE_CHANGE:
      {
        bool fullscreen = pMsg->param1 != 0;
        LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "From currently opened web site becomes fullsreen requested as '%s'", fullscreen ? "yes" : "no");
        SetFullscreen(fullscreen);
        break;
      }
      case TMSG_SET_LOADING_STATE:
      {
        SetLoadingState(pMsg->param1, pMsg->param2, pMsg->param3);
        break;
      }
      case TMSG_SET_TOOLTIP:
      {
        SetTooltip(pMsg->strParam);
        break;
      }
      case TMSG_SET_STATUS_MESSAGE:
      {
        SetStatusMessage(pMsg->strParam);
        break;
      }
      case TMSG_HANDLE_ON_PAINT:
      {
        OnPaintMessage *data = static_cast<OnPaintMessage*>(pMsg->lpVoid);
        data->client.callback(pMsg->lpVoid);
        m_bIsDirty = true;
        break;
      }
      case TMSG_BROWSER_CLOSE:
      {
        CefRefPtr<CefBrowser> browser = (CefBrowser*)pMsg->lpVoid;

        LOG_INTERNAL_MESSAGE(ADDON_LOG_DEBUG, "Browser close");
        break;
      }
      default:
        break;
    };

    if (waitEvent)
      waitEvent->Signal();
    delete pMsg;

    lock.Lock();
  }
}




void CWebBrowserClientBase::SetBrowser(CefRefPtr<CefBrowser> browser)
{
//   fprintf(stderr, "-- %s\n", __func__);
  m_Browser = browser;
}

bool CWebBrowserClientBase::SetInactive()
{
  m_inactivateTime = time(nullptr);
  return GetLeaveOpenTime() < 0;
}

bool CWebBrowserClientBase::SetActive()
{
  return true;
}

int CWebBrowserClientBase::CurrentInactiveCountdown()
{
  return time(nullptr) - m_inactivateTime + GetLeaveOpenTime();
}

void CWebBrowserClientBase::SendMessage(Message& message, bool wait)
{
  std::shared_ptr<CEvent> waitEvent;
  if (wait)
  {
    message.waitEvent.reset(new CEvent(true));
    waitEvent = message.waitEvent;
  }

  CLockObject lock(m_Mutex);

  Message* msg    = new Message();
  msg->dwMessage  = message.dwMessage;
  msg->param1     = message.param1;
  msg->param2     = message.param2;
  msg->param3     = message.param3;
  msg->lpVoid     = message.lpVoid;
  msg->strParam   = message.strParam;
  msg->params     = message.params;
  msg->waitEvent  = message.waitEvent;

  m_processQueue.push(msg);
  lock.Unlock();

  if (waitEvent)
    waitEvent->Wait(1000);
}

bool CWebBrowserClientBase::OnAction(int actionId, uint32_t buttoncode, wchar_t unicode, int &nextItem)
{
  if (!m_Browser.get())
    return false;

  CefRefPtr<CefBrowserHost> host = m_Browser->GetHost();

  fprintf(stderr, "----------int actionId %i, uint32_t buttoncode %X, wchar_t unicode %i, int &nextItem %i\n",
          actionId, buttoncode, unicode, nextItem);

  if (!m_focusedField.isEditable)
  {
    switch (actionId)
    {
      case ACTION_VOLUME_UP:
      case ACTION_VOLUME_DOWN:
      case ACTION_VOLAMP:
      case ACTION_MUTE:
        return false;
      case ACTION_NAV_BACK:
      case ACTION_MENU:
      case ACTION_PREVIOUS_MENU:
        return false;
      case ACTION_ZOOM_OUT:
      {
        int zoomTo = kodi::GetSettingInt("zoomlevel") - kodi::GetSettingInt("zoom_step_size");
        if (zoomTo < 30)
          break;

        LOG_MESSAGE(ADDON_LOG_DEBUG, "%s - Zoom out to %i %%", __FUNCTION__, zoomTo);
        m_Browser->GetHost()->SetZoomLevel(PercentageToZoomLevel(zoomTo));
        kodi::SetSettingInt("zoomlevel", zoomTo);
        break;
      }
      case ACTION_ZOOM_IN:
      {
        int zoomTo = kodi::GetSettingInt("zoomlevel") + kodi::GetSettingInt("zoom_step_size");
        if (zoomTo > 330)
          break;

        LOG_MESSAGE(ADDON_LOG_DEBUG, "%s - Zoom in to %i %%", __FUNCTION__, zoomTo);
        m_Browser->GetHost()->SetZoomLevel(PercentageToZoomLevel(zoomTo));
        kodi::SetSettingInt("zoomlevel", zoomTo);
        break;
      }
      default:
        break;
    };
  }

  CefKeyEvent key_event;
  key_event.modifiers = CSystemTranslator::ButtonCodeToModifier(buttoncode);
  key_event.windows_key_code = CSystemTranslator::ButtonCodeToKeyboardCode(buttoncode);
  key_event.native_key_code = 0;
  key_event.is_system_key = false;
  key_event.character = unicode;
  key_event.unmodified_character = CSystemTranslator::ButtonCodeToUnmodifiedCharacter(buttoncode);
  key_event.focus_on_editable_field = m_focusedField.isEditable;

  if (key_event.windows_key_code == VKEY_RETURN)
  {
    // We need to treat the enter key as a key press of character \r.  This
    // is apparently just how webkit handles it and what it expects.
    key_event.unmodified_character = '\r';
  }

  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);

  return true;
}

bool CWebBrowserClientBase::OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state)
{
  if (!m_Browser.get())
    return true;

  static const int scrollbarPixelsPerTick = 40;

  CefRefPtr<CefBrowserHost> host = m_Browser->GetHost();

  CefMouseEvent mouse_event;
  mouse_event.x = x * m_fMouseXScaleFactor;
  mouse_event.y = y * m_fMouseYScaleFactor;

  switch (id)
  {
    case ACTION_MOUSE_LEFT_CLICK:
    {
      if (m_focusedField.isEditable &&
          mouse_event.x >= m_focusedField.x &&
          mouse_event.x <= m_focusedField.x + m_focusedField.width &&
          mouse_event.y >= m_focusedField.y &&
          mouse_event.y <= m_focusedField.y + m_focusedField.height)
      {
        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(AddonClientMessage::FocusedSelected);
        m_Browser->SendProcessMessage(PID_RENDERER, message);
      }
      else
      {
        m_focusedField = {0};

        mouse_event.modifiers = 0;
        mouse_event.modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
        host->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
        host->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
        m_iMousePreviousFlags = mouse_event.modifiers;
        m_iMousePreviousControl = MBT_LEFT;
      }
      break;
    }
    case ACTION_MOUSE_RIGHT_CLICK:
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_RIGHT, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_RIGHT, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_RIGHT;
      break;
    case ACTION_MOUSE_MIDDLE_CLICK:
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_MIDDLE;
      break;
    case ACTION_MOUSE_DOUBLE_CLICK:
      mouse_event.modifiers = m_iMousePreviousFlags;
      host->SendMouseClickEvent(mouse_event, m_iMousePreviousControl, false, 1);
      host->SendMouseClickEvent(mouse_event, m_iMousePreviousControl, true, 1);
      m_iMousePreviousControl = MBT_LEFT;
      m_iMousePreviousFlags = 0;
      break;
    case ACTION_MOUSE_WHEEL_UP:
      host->SendMouseWheelEvent(mouse_event, 0, scrollbarPixelsPerTick);
      break;
    case ACTION_MOUSE_WHEEL_DOWN:
      host->SendMouseWheelEvent(mouse_event, 0, -scrollbarPixelsPerTick);
      break;
    case ACTION_MOUSE_DRAG:

      break;
    case ACTION_MOUSE_MOVE:
    {
      bool mouse_leave = state == 3 ? true : false;
      host->SendMouseMoveEvent(mouse_event, mouse_leave);
      break;
    }
    case ACTION_MOUSE_LONG_CLICK:

      break;
    default:
      break;
  }

  return true;
}

bool CWebBrowserClientBase::Initialize()
{
  if (!m_Browser.get())
  {
    LOG_MESSAGE(ADDON_LOG_ERROR, "%s - Called without present browser", __FUNCTION__);
    return false;
  }

  m_Browser->GetHost()->SetZoomLevel(PercentageToZoomLevel(kodi::GetSettingInt("zoomlevel")));


  return true;
}

bool CWebBrowserClientBase::Dirty()
{
  HandleMessages();
  bool ret = m_bIsDirty;
  m_bIsDirty = false;
  return ret;
}

bool CWebBrowserClientBase::OpenWebsite(const std::string& strURL, bool single, bool allowMenus)
{
//   fprintf(stderr, "-- %s\n", __func__);
  if (!m_Browser.get())
  {
    LOG_MESSAGE(ADDON_LOG_ERROR, "%s - Called without present browser", __FUNCTION__);
    return false;
  }

  CefRefPtr<CefFrame> frame = m_Browser->GetMainFrame();
  if (!frame.get())
  {
    LOG_MESSAGE(ADDON_LOG_ERROR, "%s - Called without present frame", __FUNCTION__);
    return false;
  }

  if (m_strStartupURL.empty())
    m_strStartupURL = strURL;

  frame->LoadURL(strURL);

  return true;
}

void CWebBrowserClientBase::Reload()
{
  m_Browser->Reload();
}

void CWebBrowserClientBase::StopLoad()
{
  m_Browser->StopLoad();
}

void CWebBrowserClientBase::GoBack()
{
  m_Browser->GoBack();
}

void CWebBrowserClientBase::GoForward()
{
  m_Browser->GoForward();
}

void CWebBrowserClientBase::OpenOwnContextMenu()
{
  std::vector<std::string> entries;
  entries.push_back(kodi::GetLocalizedString(30009));
  entries.push_back(kodi::GetLocalizedString(30323));
  entries.push_back(kodi::GetLocalizedString(30032));

  int ret = kodi::gui::dialogs::ContextMenu::Show("", entries);
  if (ret >= 0)
  {
    switch (ret)
    {
      case 0:
      {
        m_mainBrowserHandler->OpenDownloadDialog();
        break;
      }
      case 1:
      {
        m_mainBrowserHandler->OpenCookieHandler();
        break;
      }
      case 2:
      {
        kodi::OpenSettings();
        break;
      }
      default:
        break;
    }
  }
}


/*!
 * @brief CefClient methods
 *
 * Implement this interface to provide handler implementations.
 */
//{
bool CWebBrowserClientBase::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
                                                     CefRefPtr<CefProcessMessage> message)
{
  CEF_REQUIRE_UI_THREAD();

  if (m_messageRouter->OnProcessMessageReceived(browser, source_process, message))
    return true;

  std::string message_name = message->GetName();
  CefRefPtr<CefBrowserHost> host = browser->GetHost();
  if (message_name == RendererMessage::ExecuteJavaScriptBrowserSide)
  {
    browser->GetMainFrame()->ExecuteJavaScript(message->GetArgumentList()->GetString(0),
                                               message->GetArgumentList()->GetString(1),
                                               message->GetArgumentList()->GetInt(2));
    return true;
  }
  else if (message_name == RendererMessage::FocusedNodeChanged)
  {
    m_focusedField.focusOnEditableField = message->GetArgumentList()->GetBool(0);
    m_focusedField.isEditable = message->GetArgumentList()->GetBool(1);
    m_focusedField.x = message->GetArgumentList()->GetInt(2);
    m_focusedField.y = message->GetArgumentList()->GetInt(3);
    m_focusedField.width = message->GetArgumentList()->GetInt(4);
    m_focusedField.height = message->GetArgumentList()->GetInt(5);
    m_focusedField.type = message->GetArgumentList()->GetString(6);
    m_focusedField.value = message->GetArgumentList()->GetString(7);
    return true;
  }
  else if (message_name == RendererMessage::SendString)
  {
    if (m_focusedField.focusOnEditableField)
    {
      CefRange replacement_range;
      replacement_range.from = 0;
      replacement_range.to = -1;
      m_Browser->GetHost()->ImeCommitText(message->GetArgumentList()->GetString(0), replacement_range, 0);
    }
    return true;
  }
  else if (message_name == RendererMessage::ShowKeyboard)
  {
    std::string type = message->GetArgumentList()->GetString(0);
    std::string header = message->GetArgumentList()->GetString(1);
    std::string value = message->GetArgumentList()->GetString(2);
    std::string id = message->GetArgumentList()->GetString(3);
    std::string name = message->GetArgumentList()->GetString(4);
    std::string markup = message->GetArgumentList()->GetString(5);

    if (header.empty())
    {
      if (type == "password")
        header = kodi::GetLocalizedString(30012);
      else if (type == "text")
        header = kodi::GetLocalizedString(30013);
      else if (type == "textarea")
        header = kodi::GetLocalizedString(30014);
    }

    if (kodi::gui::dialogs::Keyboard::ShowAndGetInput(value, header, true, type == "password"))
    {
      std::string code;
      if (!id.empty())
        code = "document.getElementById(\"" + id + "\").value = \"" + value + "\"\n";
      else
      {
        code =
          "var myelements = document.getElementsByName(\"" + name + "\")\n"
          "for (var i=0; i<myelements.length; i++) {\n"
          "  if (myelements[i].nodeType != 1) {\n"
          "    break;\n"
          "  }\n"
          "  myelements[i].value = \"" + value + "\"\n"
          "}\n";
      }
      browser->GetMainFrame()->ExecuteJavaScript(code, browser->GetFocusedFrame()->GetURL(), 0);
    }
  }
  else if (message_name == RendererMessage::ShowSelect)
  {
    std::string type = message->GetArgumentList()->GetString(0);
    std::string header = message->GetArgumentList()->GetString(1);
    std::string value = message->GetArgumentList()->GetString(2);
    std::string id = message->GetArgumentList()->GetString(3);
    std::string name = message->GetArgumentList()->GetString(4);
    std::string markup = message->GetArgumentList()->GetString(5);

    std::vector<SSelectionEntry> entries;
    for (int i = 0; i < message->GetArgumentList()->GetInt(6); ++i)
    {
      SSelectionEntry entry;
      entry.id = message->GetArgumentList()->GetString(i*3+0+7);
      entry.name = message->GetArgumentList()->GetString(i*3+1+7);
      entry.selected = message->GetArgumentList()->GetBool(i*3+2+7);
      entries.push_back(std::move(entry));
    }

    if (!entries.empty())
    {
      bool ret = false;
      if (type == "select-multiple")
        ret = kodi::gui::dialogs::Select::ShowMultiSelect("", entries);
      else
        ret = kodi::gui::dialogs::Select::Show("", entries) >= 0;

      if (ret)
      {
        std::string values = "";
        for (const auto& entry : entries)
        {
          if (entry.selected)
            values += entry.id + ",";
        }
        const std::string& code =
            "var values = \"" + values + "\";\n"
            "var myselect = document.getElementsByName(\"" + name +"\");\n"
            "for (var i=0; i<myselect.length; i++) {\n"
            "  if (myselect[i].nodeType != 1)\n"
            "    continue;\n"
            "  for (var j=0; j<myselect[i].options.length; j++) {\n"
            "    myselect[i].options[j].selected=false;\n"
            "  }\n"
            "  values.split(',').forEach(function(v) {\n"
            "    Array.from(myselect[i]).find(c => c.value == v).selected = true;\n"
            "  });\n"
            "}\n";
        browser->GetMainFrame()->ExecuteJavaScript(code, browser->GetFocusedFrame()->GetURL(), 0);
      }
    }
  }

  return false;
}
//}





/*!
 * @brief CefDragHandler methods
 */
//{
bool CWebBrowserClientBase::OnDragEnter(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDragData> dragData,
    CefRenderHandler::DragOperationsMask mask)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

  /* Forbid dragging of link URLs */
  if(mask & DRAG_OPERATION_LINK)
    return true;

  return false;
}
//}


/*!
 * @brief CefFindHandler methods
 *
 * Implement this interface to handle events related to find results. The
 * methods of this class will be called on the UI thread.
 */
//{
void CWebBrowserClientBase::OnFindResult(
    CefRefPtr<CefBrowser>                 browser,
    int                                   identifier,
    int                                   count,
    const CefRect&                        selectionRect,
    int                                   activeMatchOrdinal,
    bool                                  finalUpdate)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefFocusHandler methods
 */
//{
void CWebBrowserClientBase::OnTakeFocus(
    CefRefPtr<CefBrowser>                 browser,
    bool                                  next)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnSetFocus(
    CefRefPtr<CefBrowser>                 browser,
    FocusSource                           source)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnGotFocus(
    CefRefPtr<CefBrowser>                 browser)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefLifeSpanHandler methods
 */
//{
bool CWebBrowserClientBase::OnBeforePopup(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      target_url,
    const CefString&                      target_frame_name,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool                                  user_gesture,
    const CefPopupFeatures&               popupFeatures,
    CefWindowInfo&                        windowInfo,
    CefRefPtr<CefClient>&                 client,
    CefBrowserSettings&                   settings,
    bool*                                 no_javascript_access)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s - %s", __FUNCTION__, std::string(target_url).c_str());

  OpenWebsite(std::string(target_url), false, false);
  return true; /* Cancel popups in off-screen rendering mode */
}

void CWebBrowserClientBase::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

  if (!m_Browser.get())
  {
    LOG_MESSAGE(ADDON_LOG_DEBUG, "---------------------------------------------------------%s", __FUNCTION__);
    m_Browser   = browser;
    m_BrowserId = browser->GetIdentifier();

    Message tMsg = {TMSG_SET_CONTROL_READY};
    tMsg.param1 = true;
    SendMessage(tMsg, false);
  }
  else if(browser->IsPopup())
    m_popupBrowsers.push_back(browser); /* Add to the list of popup browsers */

  m_iBrowserCount++;
}

// bool CWebBrowserClientBase::RunModal(CefRefPtr<CefBrowser> browser)
// {
//     LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
//   return false;
// }

bool CWebBrowserClientBase::DoClose(CefRefPtr<CefBrowser> browser)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

  /*
   * Closing the main window requires special handling. See the DoClose()
   * documentation in the CEF header for a detailed destription of this process
   */

  if (m_BrowserId == browser->GetIdentifier())
  {
    //FIXME: browser->GetHost(); /* Notify the browser that the parent window is about to close. */
    m_isClosing = true; /* Set a flag to indicate that the window close should be allowed */
  }

  return false; /* Allow the close. For windowed browsers this will result in the OS close event being sent */
}

void CWebBrowserClientBase::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);

  CEF_REQUIRE_UI_THREAD();

  m_messageRouter->OnBeforeClose(browser);

  if (m_BrowserId == browser->GetIdentifier())
  {
    m_Browser = nullptr;/* Free the browser pointer so that the browser can be destroyed */

    Message tMsg = {TMSG_BROWSER_CLOSE};
    tMsg.lpVoid = browser;
    SendMessage(tMsg, true);
  }
  else if (browser->IsPopup())
  {
    // Remove from the browser popup list.
    for (browserList::iterator it = m_popupBrowsers.begin(); it != m_popupBrowsers.end(); ++it)
    {
      if((*it)->IsSame(browser))
      {
        m_popupBrowsers.erase(it);
        break;
      }
    }
  }
}
//}

/*!
 * @brief CefRequestHandler methods
 */
//{
bool CWebBrowserClientBase::OnBeforeBrowse(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    bool                                  is_redirect)
{
  std::string strURL = request->GetURL();
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s - %s", __FUNCTION__, strURL.c_str());

  // We only care about these on the main frame
  if (!frame.get() || !frame->IsMain())
    return false;

  m_messageRouter->OnBeforeBrowse(browser, frame);
  return false;
}

bool CWebBrowserClientBase::OnOpenURLFromTab(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      target_url,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool                                  user_gesture)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

CefRequestHandler::ReturnValue CWebBrowserClientBase::OnBeforeResourceLoad(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefRefPtr<CefRequestCallback>         callback)
{
  CEF_REQUIRE_IO_THREAD();

  return m_resourceManager->OnBeforeResourceLoad(browser, frame, request, callback);
}

CefRefPtr<CefResourceHandler> CWebBrowserClientBase::GetResourceHandler(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request)
{
  CEF_REQUIRE_IO_THREAD();

  return m_resourceManager->GetResourceHandler(browser, frame, request);
}

void CWebBrowserClientBase::OnResourceRedirect(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefRefPtr<CefResponse>                response,
    CefString&                            new_url)
{
    LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnResourceResponse(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefRefPtr<CefResponse>                response)
{
//     LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::GetAuthCredentials(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    bool                                  isProxy,
    const CefString&                      host,
    int                                   port,
    const CefString&                      realm,
    const CefString&                      scheme,
    CefRefPtr<CefAuthCallback>            callback)
{
    LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::OnQuotaRequest(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      origin_url,
    int64                                 new_size,
    CefRefPtr<CefRequestCallback>         callback)
{
  CEF_REQUIRE_IO_THREAD();

  static const int64 max_size = 1024 * 1024 * 20;  // 20mb.

  // Grant the quota request if the size is reasonable.
  callback->Continue(new_size <= max_size);
  return true;
}

void CWebBrowserClientBase::OnProtocolExecution(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      url,
    bool&                                 allow_os_execution)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);

  CEF_REQUIRE_UI_THREAD();

  std::string urlStr = url;

  // Allow OS execution of Spotify URIs.
  if (urlStr.find("spotify:") == 0)
    allow_os_execution = true;
}

bool CWebBrowserClientBase::OnCertificateError(
    CefRefPtr<CefBrowser>                 browser,
    ErrorCode                             cert_error,
    const CefString&                      request_url,
    CefRefPtr<CefSSLInfo>                 ssl_info,
    CefRefPtr<CefRequestCallback>         callback)
{
    LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnPluginCrashed(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      plugin_path)
{
    LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnRenderViewReady(
    CefRefPtr<CefBrowser>                 browser)
{
    LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

  m_messageRouter->OnRenderProcessTerminated(browser);

  // Don't reload if there's no start URL, or if the crash URL was specified.
  if (m_strStartupURL.empty() || m_strStartupURL == "chrome://crash")
    return;

  CefRefPtr<CefFrame> frame = browser->GetMainFrame();
  std::string url = frame->GetURL();

  // Don't reload if the termination occurred before any URL had successfully
  // loaded.
  if (url.empty())
    return;

  std::string start_url = m_strStartupURL;
  StringUtils::ToLower(url);
  StringUtils::ToLower(start_url);

  // Don't reload the URL that just resulted in termination.
  if (url.find(start_url) == 0)
    return;

  frame->LoadURL(m_strStartupURL);
}
//}


/*!
 * @brief CefRequestContextHandler methods
 */
//{
CefRefPtr<CefCookieManager> CWebBrowserClientBase::GetCookieManager()
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  return NULL;
}

bool CWebBrowserClientBase::OnBeforePluginLoad(
    const CefString&                      mime_type,
    const CefString&                      plugin_url,
    bool                                  is_main_frame,
    const CefString&                      top_origin_url,
    CefRefPtr<CefWebPluginInfo>           plugin_info,
    PluginPolicy*                         plugin_policy)
{
    LOG_MESSAGE(ADDON_LOG_DEBUG, "%s", __FUNCTION__);
  // Always allow the PDF plugin to load.
  if (*plugin_policy != PLUGIN_POLICY_ALLOW && mime_type == "application/pdf")
  {
    *plugin_policy = PLUGIN_POLICY_ALLOW;
    return true;
  }

  return false;
}
//}



































/// CefContextMenuHandler methods
//@{
void CWebBrowserClientBase::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                                CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
  std::string url = params->GetLinkUrl().ToString();
  if (!url.empty())
    model->InsertItemAt(0, CLIENT_ID_OPEN_SELECTED_SIDE, kodi::GetLocalizedString(30000 + CLIENT_ID_OPEN_SELECTED_SIDE));

  int flags = params->GetTypeFlags();
  if (flags & CM_TYPEFLAG_EDITABLE)
    model->InsertItemAt(0, CLIENT_ID_OPEN_KEYBOARD, kodi::GetLocalizedString(30000 + CLIENT_ID_OPEN_KEYBOARD));

#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "CefContextMenuParams");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- %ix%i - TypeFlags: 0x%X - ImageContents: %s - MediaType: %i - MediaStateFlags %i - EditStateFlags %i", params->GetXCoord(),
      params->GetYCoord(), (int)params->GetTypeFlags(), params->HasImageContents() ? "yes" : "no",
      (int)params->GetMediaType(), (int)params->GetMediaStateFlags(), (int)params->GetEditStateFlags());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- LinkUrl:                %s", params->GetLinkUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- UnfilteredLinkUrl:      %s", params->GetUnfilteredLinkUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- SourceUrl:              %s", params->GetSourceUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- PageUrl:                %s", params->GetPageUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- FrameUrl :              %s", params->GetFrameUrl().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- FrameCharset :          %s", params->GetFrameCharset().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- SelectionText :         %s", params->GetSelectionText().ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- MisspelledWord :        %s", params->GetMisspelledWord().ToString().c_str());
  std::vector<CefString> suggestions;
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- DictionarySuggestions : %s", params->GetDictionarySuggestions(suggestions) ? "OK" : "fail");
  for (unsigned int i = 0; i < suggestions.size(); i++)
    LOG_MESSAGE(ADDON_LOG_DEBUG, "  - %02i: %s", i, suggestions[i].ToString().c_str());
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsEditable :            %s", params->IsEditable() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsSpellCheckEnabled :   %s", params->IsSpellCheckEnabled() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsCustomMenu :          %s", params->IsCustomMenu() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- IsPepperMenu :          %s", params->IsPepperMenu() ? "yes" : "no");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "CefMenuModel");
  LOG_MESSAGE(ADDON_LOG_DEBUG, "- Count:                  %i", model->GetCount());
  for (unsigned int i = 0; i < model->GetCount(); i++)
    LOG_MESSAGE(ADDON_LOG_DEBUG, "  - %02i: ID '%i' Type '%i' - Name '%s'",
                    i, model->GetCommandIdAt(i), model->GetTypeAt(i), model->GetLabelAt(i).ToString().c_str());
#endif
}

bool CWebBrowserClientBase::RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params,
                                           CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback)
{
  std::vector<std::pair<int, std::string>> entries;
  for (unsigned int i = 0; i < model->GetCount(); ++i)
  {
    int id = model->GetCommandIdAt(i);
    if (id < 0 ||
        id == MENU_ID_PRINT ||
        id == MENU_ID_VIEW_SOURCE ||
        !model->IsEnabled(id))
    {
      // ignored parts!
      continue;
    }

    cef_menu_item_type_t type = model->GetTypeAt(i);
    if (type == MENUITEMTYPE_SEPARATOR)
    {
      // ignore separators
      continue;
    }
    else if (type != MENUITEMTYPE_COMMAND)
    {
      // TODO add support for other formats e.g. boolean check
      LOG_MESSAGE(ADDON_LOG_ERROR, "cef_menu_item_type_t '%i' currently not supported!", type);
      continue;
    }

    entries.push_back(std::pair<int, std::string>(id, kodi::GetLocalizedString(30000 + id)));
  }

  if (entries.empty())
  {
    callback->Cancel();
    return true;
  }

  int ret = kodi::gui::dialogs::ContextMenu::Show("", entries);
  if (ret >= 0)
    callback->Continue(entries[ret].first, EVENTFLAG_LEFT_MOUSE_BUTTON);
  else
    callback->Cancel();

  return true;
}

bool CWebBrowserClientBase::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params,
                                                 int command_id, EventFlags event_flags)
{
  if (command_id == CLIENT_ID_OPEN_SELECTED_SIDE)
  {
    std::string url = params->GetLinkUrl().ToString();
    OpenWebsite(url, false, false);
  }
  else if (command_id == CLIENT_ID_OPEN_KEYBOARD)
  {
    if (m_focusedField.focusOnEditableField)
    {
      CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(AddonClientMessage::FocusedSelected);
      browser->SendProcessMessage(PID_RENDERER, message);
    }
    else
    {
      std::string header;
      if (m_focusedField.type == "password")
        header = kodi::GetLocalizedString(30012);
      else if (m_focusedField.type == "text")
        header = kodi::GetLocalizedString(30013);
      else if (m_focusedField.type == "textarea")
        header = kodi::GetLocalizedString(30014);

      if (kodi::gui::dialogs::Keyboard::ShowAndGetInput(m_focusedField.value, header, true, m_focusedField.type == "password"))
      {
        CefRange replacement_range;
        replacement_range.from = 0;
        replacement_range.to = -1;
        browser->GetHost()->ImeCommitText(m_focusedField.value, replacement_range, 0);
      }
    }
  }
  else
  {
    return false;
  }

  return true;
}
//@}

/// CefLoadHandler methods
//@{
void CWebBrowserClientBase::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
{
  CEF_REQUIRE_UI_THREAD();

  Message tMsg = {TMSG_SET_LOADING_STATE};
  tMsg.param1 = isLoading;
  tMsg.param2 = canGoBack;
  tMsg.param3 = canGoForward;
  SendMessage(tMsg, false);
}

void CWebBrowserClientBase::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "Load started (id='%d', URL='%s'", browser->GetIdentifier(), frame->GetURL().ToString().c_str());
  CEF_REQUIRE_UI_THREAD();

  m_isLoading = true;
  Initialize();
}

void CWebBrowserClientBase::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
  LOG_MESSAGE(ADDON_LOG_DEBUG, "Load done with status code '%i'", httpStatusCode);
  CEF_REQUIRE_UI_THREAD();
  m_isLoading = false;
}

void CWebBrowserClientBase::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                                        const CefString& errorText, const CefString& failedUrl)
{
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Don't display an error for external protocols that we allow the OS to
  // handle. See OnProtocolExecution().
  if (errorCode == ERR_UNKNOWN_URL_SCHEME)
  {
    std::string urlStr = frame->GetURL();
    if (urlStr.find("spotify:") == 0)
      return;
  }

  kodi::QueueNotification(QUEUE_WARNING, kodi::GetLocalizedString(30133), failedUrl.ToString());

  // Load the error page.
  LoadErrorPage(frame, failedUrl, errorCode, errorText);
}
//@}

int CWebBrowserClientBase::ZoomLevelToPercentage(double zoomlevel)
{
  return int((zoomlevel*ZOOM_MULTIPLY)+100.0);
}

double CWebBrowserClientBase::PercentageToZoomLevel(int percent)
{
  return (double(percent-100))/ZOOM_MULTIPLY;
}

void CWebBrowserClientBase::LoadErrorPage(CefRefPtr<CefFrame> frame,
                   const std::string& failed_url,
                   cef_errorcode_t error_code,
                   const std::string& other_info)
{
  std::stringstream ss;
  ss << "<html><head><title>"<< kodi::GetLocalizedString(30133) << "</title></head>"
        "<body bgcolor=\"white\">"
        "<h3>"<< kodi::GetLocalizedString(30133) << ".</h3>"
        "URL: <a href=\""
     << failed_url << "\">" << failed_url
     << "</a><br/>Error: " << GetErrorString(error_code) << " ("
     << error_code << ")";

  if (!other_info.empty())
    ss << "<br/>" << other_info;

  ss << "</body></html>";
  frame->LoadURL(GetDataURI(ss.str(), "text/html"));
}

std::string CWebBrowserClientBase::GetErrorString(cef_errorcode_t code)
{
// Case condition that returns |code| as a string.
#define CASE(code) \
  case code:       \
    return #code

  switch (code) {
    CASE(ERR_NONE);
    CASE(ERR_FAILED);
    CASE(ERR_ABORTED);
    CASE(ERR_INVALID_ARGUMENT);
    CASE(ERR_INVALID_HANDLE);
    CASE(ERR_FILE_NOT_FOUND);
    CASE(ERR_TIMED_OUT);
    CASE(ERR_FILE_TOO_BIG);
    CASE(ERR_UNEXPECTED);
    CASE(ERR_ACCESS_DENIED);
    CASE(ERR_NOT_IMPLEMENTED);
    CASE(ERR_CONNECTION_CLOSED);
    CASE(ERR_CONNECTION_RESET);
    CASE(ERR_CONNECTION_REFUSED);
    CASE(ERR_CONNECTION_ABORTED);
    CASE(ERR_CONNECTION_FAILED);
    CASE(ERR_NAME_NOT_RESOLVED);
    CASE(ERR_INTERNET_DISCONNECTED);
    CASE(ERR_SSL_PROTOCOL_ERROR);
    CASE(ERR_ADDRESS_INVALID);
    CASE(ERR_ADDRESS_UNREACHABLE);
    CASE(ERR_SSL_CLIENT_AUTH_CERT_NEEDED);
    CASE(ERR_TUNNEL_CONNECTION_FAILED);
    CASE(ERR_NO_SSL_VERSIONS_ENABLED);
    CASE(ERR_SSL_VERSION_OR_CIPHER_MISMATCH);
    CASE(ERR_SSL_RENEGOTIATION_REQUESTED);
    CASE(ERR_CERT_COMMON_NAME_INVALID);
    CASE(ERR_CERT_DATE_INVALID);
    CASE(ERR_CERT_AUTHORITY_INVALID);
    CASE(ERR_CERT_CONTAINS_ERRORS);
    CASE(ERR_CERT_NO_REVOCATION_MECHANISM);
    CASE(ERR_CERT_UNABLE_TO_CHECK_REVOCATION);
    CASE(ERR_CERT_REVOKED);
    CASE(ERR_CERT_INVALID);
    CASE(ERR_CERT_END);
    CASE(ERR_INVALID_URL);
    CASE(ERR_DISALLOWED_URL_SCHEME);
    CASE(ERR_UNKNOWN_URL_SCHEME);
    CASE(ERR_TOO_MANY_REDIRECTS);
    CASE(ERR_UNSAFE_REDIRECT);
    CASE(ERR_UNSAFE_PORT);
    CASE(ERR_INVALID_RESPONSE);
    CASE(ERR_INVALID_CHUNKED_ENCODING);
    CASE(ERR_METHOD_NOT_SUPPORTED);
    CASE(ERR_UNEXPECTED_PROXY_AUTH);
    CASE(ERR_EMPTY_RESPONSE);
    CASE(ERR_RESPONSE_HEADERS_TOO_BIG);
    CASE(ERR_CACHE_MISS);
    CASE(ERR_INSECURE_RESPONSE);
    default:
      return "UNKNOWN";
  }
}

std::string CWebBrowserClientBase::GetDataURI(const std::string& data, const std::string& mime_type)
{
  return "data:" + mime_type + ";base64," +
         CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
             .ToString();
}
