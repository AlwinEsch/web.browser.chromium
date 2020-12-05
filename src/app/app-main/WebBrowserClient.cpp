/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "WebBrowserClient.h"

#include "MainCEFProcess.h"
#include "ResourceManager.h"
#include "certificate/CertificateError.h"
#include "certificate/URICheckHandler.h"
#include "gui/DialogBrowserContextMenu.h"
#include "gui/DialogJSHandler.h"
#include "interface/JSException.h"
#include "interface/v8/v8-kodi.h"
#include "../utils/ExtensionUtils.h"
#include "../utils/SystemTranslator.h"
#include "../MessageIds.h"

// Dev-kit
#include "../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/ContextMenu.h"
#include "../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/YesNo.h"

// CEF

// Global
#include <kodi/tools/StringUtils.h>
#include <thread>

#define ZOOM_MULTIPLY 25.0

using kodi::tools::StringUtils;
using namespace chromium::app::main;

namespace
{
static std::atomic_int m_ctorcount{0}; // For debug purposes and to see destructs done
}

CWebBrowserClient::CWebBrowserClient(const WEB_ADDON_GUI_PROPS& properties,
                                     uint64_t handle,
                                     int dataIdentifier,
                                     const std::string& startURL,
                                     CMainCEFProcess& mainProcess,
                                     CefRefPtr<CRequestContextHandler> handler)
  : CWebControl(properties, handle, dataIdentifier),
    m_mainProcess(mainProcess),
    m_contextHandler(handler)
{
  m_clientIdentifier = StringUtils::Format("%s-client-%04i", mainProcess.GetIdentifier().c_str(), dataIdentifier);

  m_fMouseXScaleFactor = GetWidth() / GetSkinWidth();
  m_fMouseYScaleFactor = GetHeight() / GetSkinHeight();

  kodi::Log(ADDON_LOG_DEBUG, "Current browser client sizes:");
  kodi::Log(ADDON_LOG_DEBUG, " - Mouse X Scale Factor: %f", m_fMouseYScaleFactor);
  kodi::Log(ADDON_LOG_DEBUG, " - Mouse Y Scale Factor: %f", m_fMouseYScaleFactor);
  kodi::Log(ADDON_LOG_DEBUG, " - X Position:           %f", GetXPos());
  kodi::Log(ADDON_LOG_DEBUG, " - Width:                %f", GetWidth());
  kodi::Log(ADDON_LOG_DEBUG, " - Skin X Position:      %f", GetSkinXPos());
  kodi::Log(ADDON_LOG_DEBUG, " - Skin Width:           %f", GetSkinWidth());
  kodi::Log(ADDON_LOG_DEBUG, " - Y Position:           %f", GetYPos());
  kodi::Log(ADDON_LOG_DEBUG, " - Height:               %f", GetHeight());
  kodi::Log(ADDON_LOG_DEBUG, " - Skin Y Position:      %f", GetSkinYPos());
  kodi::Log(ADDON_LOG_DEBUG, " - Skin Height:          %f", GetSkinHeight());

  // CEF related sub classes to manage web parts
  m_resourceManager = new CefResourceManager();
  ResourceManager::SetupResourceManager(m_resourceManager);

  // Create the browser-side router for query handling.
  m_dialogContextMenu = new gui::CBrowerDialogContextMenu(this);
  m_jsDialogHandler = new gui::CJSDialogHandler(this);
  m_v8Kodi = new interface::v8::CV8Kodi(this);
  m_renderer = new renderer::CRendererClient(this);
  m_renderer->Initialize(GetWidth(), GetHeight());

  kodi::Log(ADDON_LOG_DEBUG, "CWebBrowserClient START (%p) count open %i", this, ++m_ctorcount);
}

CWebBrowserClient::~CWebBrowserClient()
{
  kodi::Log(ADDON_LOG_DEBUG, "CWebBrowserClient STOP (%p) count open %i", this, --m_ctorcount);

  m_renderer = nullptr;

  // Inform main addon class that's browser client is destroyed and to destroy
  // CEF if needed
  m_mainProcess.InformDestroyed(GetDataIdentifier());
}

bool CWebBrowserClient::SetActive()
{
  m_renderViewReady = true;
  if (m_browser.get())
  {
    m_browser->GetHost()->SetFocus(true);
    SetOpenedAddress(m_currentURL);
    SetOpenedTitle(m_currentTitle);
    SetIconURL(m_currentIcon);
    return true;
  }

  return false;
}

bool CWebBrowserClient::SetInactive()
{
  m_renderViewReady = false;

  if (m_browser.get())
  {
    m_browser->GetHost()->SetFocus(false);
    return true;
  }

  return false;
}

bool CWebBrowserClient::Initialize()
{
  if (!m_browser.get())
  {
    kodi::Log(ADDON_LOG_ERROR, "%s - Called without present browser", __func__);
    return false;
  }

  if (m_renderViewReady)
    m_browser->GetHost()->SetZoomLevel(PercentageToZoomLevel(kodi::GetSettingInt("main.zoomlevel")));

  return true;
}

void CWebBrowserClient::CloseComplete()
{
  CEF_REQUIRE_UI_THREAD();

  SetInactive();

  if (m_browser.get())
  {
    m_contextHandler->Clear();
    m_contextHandler = nullptr;
    m_browser->GetHost()->CloseBrowser(true);
  }

  m_renderer->ClearClient();

  m_resourceManager = nullptr;
  m_jsDialogHandler = nullptr;
  m_dialogContextMenu = nullptr;
  m_v8Kodi = nullptr;
}

int CWebBrowserClient::ZoomLevelToPercentage(double zoomlevel)
{
  return static_cast<int>((zoomlevel * ZOOM_MULTIPLY) + 100.0);
}

double CWebBrowserClient::PercentageToZoomLevel(int percent)
{
  return (static_cast<double>(percent - 100)) / ZOOM_MULTIPLY;
}

void CWebBrowserClient::CreateMessageHandlers(MessageHandlerSet& handlers)
{
//   handlers.insert(new CJSHandler(this));
}

void CWebBrowserClient::AddExtension(CefRefPtr<CefExtension> extension)
{
  // Don't track extensions that can't be loaded directly.
  if (chromium::app::utils::ExtensionUtils::GetExtensionURL(extension).empty())
    return;

  // Don't add the same extension multiple times.
  for (const auto& it : m_extensions)
  {
    if (it->GetIdentifier() == extension->GetIdentifier())
      return;
  }

  m_extensions.insert(extension);
}

// kodi::addon::CWebControl
//@{

void CWebBrowserClient::Render()
{
  if (m_renderViewReady)
    m_renderer->Render();
}

bool CWebBrowserClient::Dirty()
{
  if (!m_renderViewReady)
    return false;

  return m_renderer->Dirty();
}

bool CWebBrowserClient::OnInit()
{
  return true;
}

bool CWebBrowserClient::OnAction(const kodi::gui::input::CAction& action, int& nextItem)
{
  if (!m_browser.get())
    return false;

  fprintf(stderr, "--> %s %i %i %i %i\n", __func__, action.GetID(), action.GetButtonCode(), action.GetUnicode(), nextItem);

  CefRefPtr<CefBrowserHost> host = m_browser->GetHost();
  ADDON_ACTION actionId = action.GetID();
  if (!m_focusOnEditableField)
  {
    switch (actionId)
    {
      case ADDON_ACTION_VOLUME_UP:
      case ADDON_ACTION_VOLUME_DOWN:
      case ADDON_ACTION_VOLAMP:
      case ADDON_ACTION_MUTE:
        return false;
      case ADDON_ACTION_NAV_BACK:
      case ADDON_ACTION_MENU:
      case ADDON_ACTION_PREVIOUS_MENU:
        return false;
      case ADDON_ACTION_MOVE_LEFT:
      case ADDON_ACTION_MOVE_RIGHT:
      case ADDON_ACTION_MOVE_UP:
      case ADDON_ACTION_MOVE_DOWN:
      case ADDON_ACTION_PAGE_UP:
      case ADDON_ACTION_PAGE_DOWN:
      {
        if (!HandleScrollEvent(actionId))
        {
          if (actionId == ADDON_ACTION_MOVE_LEFT)
            nextItem = GetGUIItemLeft();
          else if (actionId == ADDON_ACTION_MOVE_RIGHT)
            nextItem = GetGUIItemRight();
          else if (actionId == ADDON_ACTION_MOVE_UP)
            nextItem = GetGUIItemTop();
          else if (actionId == ADDON_ACTION_MOVE_DOWN)
            nextItem = GetGUIItemBottom();
          else if (actionId == ADDON_ACTION_PAGE_UP)
            nextItem = GetGUIItemTop();
          else if (actionId == ADDON_ACTION_PAGE_DOWN)
            nextItem = GetGUIItemBottom();
          return false;
        }
        break;
      }
      case ADDON_ACTION_FIRST_PAGE:
        SendKey(VKEY_HOME);
        return true;
      case ADDON_ACTION_LAST_PAGE:
        SendKey(VKEY_END);
        return true;
      case ADDON_ACTION_ZOOM_OUT:
      {
        int zoomTo =
            kodi::GetSettingInt("main.zoomlevel") - kodi::GetSettingInt("main.zoom_step_size");
        if (zoomTo < 30)
          break;

        kodi::Log(ADDON_LOG_DEBUG, "%s - Zoom out to %i %%", __func__, zoomTo);
        m_browser->GetHost()->SetZoomLevel(PercentageToZoomLevel(zoomTo));
        kodi::SetSettingInt("main.zoomlevel", zoomTo);
        break;
      }
      case ADDON_ACTION_ZOOM_IN:
      {
        int zoomTo =
            kodi::GetSettingInt("main.zoomlevel") + kodi::GetSettingInt("main.zoom_step_size");
        if (zoomTo > 330)
          break;

        kodi::Log(ADDON_LOG_DEBUG, "%s - Zoom in to %i %% - %i %i", __func__, zoomTo,
                    kodi::GetSettingInt("main.zoomlevel"),
                    kodi::GetSettingInt("main.zoom_step_size"));
        m_browser->GetHost()->SetZoomLevel(PercentageToZoomLevel(zoomTo));
        kodi::SetSettingInt("main.zoomlevel", zoomTo);
        break;
      }
      default:
        return false;
    };
    return true;
  }

  CefKeyEvent key_event;
  key_event.modifiers = CSystemTranslator::ButtonCodeToModifier(action.GetButtonCode());
  key_event.windows_key_code = CSystemTranslator::ButtonCodeToKeyboardCode(action.GetButtonCode());
  key_event.native_key_code = 0;
  key_event.is_system_key = false;
  key_event.character = action.GetUnicode();
  key_event.unmodified_character = CSystemTranslator::ButtonCodeToUnmodifiedCharacter(action.GetButtonCode());
  key_event.focus_on_editable_field = m_focusOnEditableField;

  if (key_event.windows_key_code == VKEY_RETURN)
  {
    // We need to treat the enter key as a key press of character \r.  This
    // is apparently just how webkit handles it and what it expects.
    key_event.unmodified_character = '\r';
  }

  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);

  return true;
}

bool CWebBrowserClient::OnMouseEvent(
    int id, double x, double y, double offsetX, double offsetY, int state)
{
  if (!m_browser.get())
    return true;

  static const int scrollbarPixelsPerTick = 40;
  CefRefPtr<CefBrowserHost> host = m_browser->GetHost();

  CefMouseEvent mouse_event;
  mouse_event.x = static_cast<int>((x - GetSkinXPos()) * m_fMouseXScaleFactor);
  mouse_event.y = static_cast<int>((y - GetSkinYPos()) * m_fMouseYScaleFactor);

  switch (id)
  {
    case ADDON_ACTION_MOUSE_LEFT_CLICK:
    {
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_LEFT;
      break;
    }
    case ADDON_ACTION_MOUSE_RIGHT_CLICK:
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_RIGHT, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_RIGHT, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_RIGHT;
      break;
    case ADDON_ACTION_MOUSE_MIDDLE_CLICK:
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_MIDDLE;
      break;
    case ADDON_ACTION_MOUSE_DOUBLE_CLICK:
      mouse_event.modifiers = m_iMousePreviousFlags;
      host->SendMouseClickEvent(mouse_event, m_iMousePreviousControl, false, 1);
      host->SendMouseClickEvent(mouse_event, m_iMousePreviousControl, true, 1);
      m_iMousePreviousControl = MBT_LEFT;
      m_iMousePreviousFlags = 0;
      break;
    case ADDON_ACTION_MOUSE_WHEEL_UP:
      host->SendMouseWheelEvent(mouse_event, 0, scrollbarPixelsPerTick);
      break;
    case ADDON_ACTION_MOUSE_WHEEL_DOWN:
      host->SendMouseWheelEvent(mouse_event, 0, -scrollbarPixelsPerTick);
      break;
    case ADDON_ACTION_MOUSE_DRAG:
    {
      mouse_event.modifiers = 0;
      mouse_event.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;
      if (!m_dragActive)
      {
        mouse_event.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;
        host->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
        mouse_event.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON | EVENTFLAG_SHIFT_DOWN;
        host->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
        m_dragActive = true;
      }

      host->SendMouseMoveEvent(mouse_event, false);
      break;
    }
    case ADDON_ACTION_MOUSE_DRAG_END:
    {
      if (m_dragActive)
      {
        host->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
        host->SendMouseMoveEvent(mouse_event, true);
        m_dragActive = false;
      }
      break;
    }
    case ADDON_ACTION_MOUSE_MOVE:
    {
      bool mouse_leave = state == 3 ? true : false;
      host->SendMouseMoveEvent(mouse_event, mouse_leave);
      break;
    }
    case ADDON_ACTION_MOUSE_LONG_CLICK:

      break;
   default:
      break;
  }

  return true;
}

bool CWebBrowserClient::OpenWebsite(const std::string& url)
{
  kodi::Log(ADDON_LOG_DEBUG, "Open website '%s'", url.c_str());
  if (!m_browser.get())
  {
    kodi::Log(ADDON_LOG_ERROR, "CWebBrowserClient::%s: Called without present browser", __func__);
    return false;
  }

  CefRefPtr<CefFrame> frame = m_browser->GetMainFrame();
  if (!frame.get())
  {
    kodi::Log(ADDON_LOG_ERROR, "CWebBrowserClient::%s: Called without present frame", __func__);
    return false;
  }

  std::string usedURL;
  if (url.empty())
    usedURL = kodi::GetSettingString("main.starturl");
  else
    usedURL = url;

  if (m_strStartupURL.empty())
    m_strStartupURL = usedURL;

  m_currentIcon = "DefaultFile.png"; // Use default image from Kodi itself
  SetIconURL(m_currentIcon);

  frame->LoadURL(usedURL);

  return true;

}

bool CWebBrowserClient::GetHistory(std::vector<std::string>& historyWebsiteNames,
                                   bool behindCurrent)
{
  if (!m_browser)
    return false;

  bool currentFound = false;
  for (const auto& entry : m_historyWebsiteNames)
  {
    if (!behindCurrent && entry.second)
      break;
    if (entry.second)
    {
      currentFound = true;
      continue;
    }

    if (behindCurrent && !currentFound)
      continue;

    historyWebsiteNames.push_back(entry.first);
  }

  return true;
}

void CWebBrowserClient::SearchText(const std::string& text,
                                   bool forward,
                                   bool matchCase,
                                   bool findNext)
{
  if (m_browser)
  {
    if (m_currentSearchText != text)
      m_browser->GetHost()->StopFinding(true);
    m_browser->GetHost()->Find(0, text, forward, matchCase, findNext);
    m_currentSearchText = text;
  }
}

void CWebBrowserClient::StopSearch(bool clearSelection)
{
  if (m_browser)
    m_browser->GetHost()->StopFinding(clearSelection);
  m_currentSearchText.clear();
}

void CWebBrowserClient::Reload()
{
}

void CWebBrowserClient::StopLoad()
{
}

void CWebBrowserClient::GoBack()
{
}

void CWebBrowserClient::GoForward()
{
}

void CWebBrowserClient::OpenOwnContextMenu()
{
  std::thread([this] {
    std::vector<std::string> entries;
    entries.push_back(kodi::GetLocalizedString(30009));
    entries.push_back(kodi::GetLocalizedString(30323));
    entries.push_back(kodi::GetLocalizedString(30032));
    entries.push_back(kodi::GetLocalizedString(30324));

    int ret = kodi::gui::dialogs::ContextMenu::Show("", entries);
    if (ret >= 0)
    {
      switch (ret)
      {
        case 0:
        {
          m_mainProcess.GetGUIManager()->GetDownloadDialog()->Open();
          break;
        }
        case 1:
        {
          m_mainProcess.GetGUIManager()->GetCookieDialog().Open();
          break;
        }
        case 2:
        {
          kodi::OpenSettings();
          break;
        }
        case 3:
        {
          if (!m_currentURL.empty())
            kodi::SetSettingString("main.starturl", m_currentURL);
          break;
        }
        default:
          break;
      }
    }
  }).detach();
}

void CWebBrowserClient::ScreenSizeChange(
    float x, float y, float width, float height, bool fullscreen)
{
  m_isFullScreen = true;
  m_renderer->ScreenSizeChange(x, y, width, height);
  if (m_browser.get())
    m_browser->GetHost()->WasResized();
}

//@}

void CWebBrowserClient::SendKey(int key)
{
  CefRefPtr<CefBrowserHost> host = m_browser->GetHost();
  CefKeyEvent key_event;
  key_event.windows_key_code = key;
  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);
}

bool CWebBrowserClient::HandleScrollEvent(int actionId)
{
  switch (actionId)
  {
    case ADDON_ACTION_MOVE_LEFT:
      SendKey(VKEY_LEFT);
      break;
    case ADDON_ACTION_MOVE_RIGHT:
      SendKey(VKEY_RIGHT);
      break;
    case ADDON_ACTION_MOVE_UP:
      SendKey(VKEY_UP);
      break;
    case ADDON_ACTION_MOVE_DOWN:
      SendKey(VKEY_DOWN);
      break;
    case ADDON_ACTION_PAGE_UP:
      SendKey(VKEY_PRIOR);
      break;
    case ADDON_ACTION_PAGE_DOWN:
      SendKey(VKEY_NEXT);
      break;
  }

  double scrollOffsetX = m_renderer->ScrollOffsetX();
  double scrollOffsetY = m_renderer->ScrollOffsetY();

  if (scrollOffsetX == m_scrollOffsetX && scrollOffsetY == m_scrollOffsetY)
  {
    return false;
  }

  m_scrollOffsetX = scrollOffsetX;
  m_scrollOffsetY = scrollOffsetY;
  return true;
}

// -----------------------------------------------------------------------------

CefRefPtr<CefAudioHandler> CWebBrowserClient::GetAudioHandler()
{
  return GetMain().GetAudioHandler();
}

CefRefPtr<CefContextMenuHandler> CWebBrowserClient::GetContextMenuHandler()
{
  return m_dialogContextMenu;
}

CefRefPtr<CefDialogHandler> CWebBrowserClient::GetDialogHandler()
{
  return GetMain().GetGUIManager()->GetFileDialog();
}

CefRefPtr<CefDownloadHandler> CWebBrowserClient::GetDownloadHandler()
{
  return GetMain().GetGUIManager()->GetDownloadDialog();
}

CefRefPtr<CefJSDialogHandler> CWebBrowserClient::GetJSDialogHandler()
{
  return m_jsDialogHandler;
}

CefRefPtr<CefRenderHandler> CWebBrowserClient::GetRenderHandler()
{
  return m_renderer;
}

// -----------------------------------------------------------------------------

/// CefClient methods
//@{
bool CWebBrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                                 CefRefPtr<CefFrame> frame,
                                                 CefProcessId source_process,
                                                 CefRefPtr<CefProcessMessage> message)
{
  CEF_REQUIRE_UI_THREAD();

  if (m_messageRouter->OnProcessMessageReceived(browser, frame, source_process, message))
    return true;

  std::string message_name = message->GetName();
  CefRefPtr<CefBrowserHost> host = browser->GetHost();
  if (message_name == RendererMessage::FocusedNodeChanged)
  {
    // A message is sent from ClientRenderDelegate to tell us whether the
    // currently focused DOM node is editable. Use of |focus_on_editable_field_|
    // is redundant with CefKeyEvent.focus_on_editable_field in OnPreKeyEvent
    // but is useful for demonstration purposes.
    m_focusOnEditableField = message->GetArgumentList()->GetBool(0);
    return true;
  }
  else if (message_name == RendererMessage::V8AddonCall)
  {
    m_v8Kodi->OnProcessMessageReceived(browser, source_process, message);
    return true;
  }
  else if (message_name == RendererMessage::OnUncaughtException)
  {
    interface::JSException::ReportJSException(message);
    return true;
  }

  return false;
}
//@}

/// CefDisplayHandler methods
//@{
void CWebBrowserClient::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& url)
{
  CEF_REQUIRE_UI_THREAD();

  if (frame->IsMain())
  {
    m_currentURL = url.ToString();
    SetOpenedAddress(m_currentURL);
  }
}

void CWebBrowserClient::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
  CEF_REQUIRE_UI_THREAD();

  if (m_currentTitle != title.ToString())
  {
    m_currentTitle = title.ToString();
    SetOpenedTitle(m_currentTitle);
  }
}

void CWebBrowserClient::OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
                                           const std::vector<CefString>& icon_urls)
{
  CEF_REQUIRE_UI_THREAD();

  size_t listSize = icon_urls.size();
#ifdef DEBUG_LOGS
  kodi::Log(ADDON_LOG_DEBUG, "From currently opened web site given icon urls (first one used)");
  for (unsigned int i = 0; i < listSize; ++i)
    kodi::Log(ADDON_LOG_DEBUG, " - Icon %i - %s", i + 1, icon_urls[i].ToString().c_str());
#endif

  if (listSize > 0)
    m_currentIcon = icon_urls[0].ToString();
  else
    m_currentIcon = "";

  SetIconURL(m_currentIcon);
}

void CWebBrowserClient::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen)
{
  CEF_REQUIRE_UI_THREAD();

  if (m_isFullScreen != fullscreen)
  {
    m_isFullScreen = fullscreen;

    kodi::Log(ADDON_LOG_DEBUG, "From currently opened web site becomes fullsreen requested as '%s'",
              m_isFullScreen ? "yes" : "no");
    SetFullscreen(m_isFullScreen);
  }
}

bool CWebBrowserClient::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text)
{
  CEF_REQUIRE_UI_THREAD();

  if (m_currentTooltip != text.ToString().c_str())
  {
    m_currentTooltip = text.ToString().c_str();
    SetTooltip(m_currentTooltip);
  }

  return true;
}

void CWebBrowserClient::OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value)
{
  CEF_REQUIRE_UI_THREAD();

  if (m_currentStatusMsg != value.ToString().c_str())
  {
    m_currentStatusMsg = value.ToString().c_str();
    SetStatusMessage(m_currentStatusMsg);
  }
}

bool CWebBrowserClient::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                         cef_log_severity_t level,
                                         const CefString& message,
                                         const CefString& source,
                                         int line)
{
  return true;
}
//@}

/// CefLifeSpanHandler methods
//@{
bool CWebBrowserClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      const CefString& target_url,
                                      const CefString& target_frame_name,
                                      CefRequestHandler::WindowOpenDisposition target_disposition,
                                      bool user_gesture,
                                      const CefPopupFeatures& popupFeatures,
                                      CefWindowInfo& windowInfo,
                                      CefRefPtr<CefClient>& client,
                                      CefBrowserSettings& settings,
                                      CefRefPtr<CefDictionaryValue>& extra_info,
                                      bool* no_javascript_access)
{
  // #ifdef DEBUG_LOGS
  kodi::Log(ADDON_LOG_DEBUG, " - target_url '%s'", std::string(target_url).c_str());
  kodi::Log(ADDON_LOG_DEBUG, " - target_frame_name '%s'", std::string(target_frame_name).c_str());
  kodi::Log(ADDON_LOG_DEBUG, " - user_gesture '%i'", user_gesture);
  kodi::Log(ADDON_LOG_DEBUG, " - no_javascript_access '%i'", *no_javascript_access);
  kodi::Log(ADDON_LOG_DEBUG, " - windowInfo.x '%i'", windowInfo.x);
  kodi::Log(ADDON_LOG_DEBUG, " - windowInfo.y '%i'", windowInfo.y);
  kodi::Log(ADDON_LOG_DEBUG, " - windowInfo.height '%i'", windowInfo.height);
  kodi::Log(ADDON_LOG_DEBUG, " - windowInfo.width '%i'", windowInfo.width);
  kodi::Log(ADDON_LOG_DEBUG, " - windowInfo.windowless_rendering_enabled '%i'",
              windowInfo.windowless_rendering_enabled);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.height '%i'", popupFeatures.height);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.width '%i'", popupFeatures.width);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.heightSet '%i'", popupFeatures.heightSet);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.widthSet '%i'", popupFeatures.widthSet);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.menuBarVisible '%i'",
              popupFeatures.menuBarVisible);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.scrollbarsVisible '%i'",
              popupFeatures.scrollbarsVisible);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.statusBarVisible '%i'",
              popupFeatures.statusBarVisible);
  kodi::Log(ADDON_LOG_DEBUG, " - popupFeatures.toolBarVisible '%i'",
              popupFeatures.toolBarVisible);
  kodi::Log(ADDON_LOG_DEBUG, " - target_disposition '%i'", target_disposition);
  // #endif

  if (target_disposition == WOD_UNKNOWN)
  {
    kodi::Log(ADDON_LOG_ERROR, "Browser popup requested with unknown target disposition");
    return false;
  }

  windowInfo.windowless_rendering_enabled = true;
#ifdef WIN32
  windowInfo.shared_texture_enabled = true;
#endif

  if (!m_isFullScreen && kodi::GetSettingBoolean("main.allow_open_to_tabs") &&
      target_disposition != WOD_CURRENT_TAB)
    RequestOpenSiteInNewTab(target_url); /* Request to do on kodi itself */
  else
    OpenWebsite(std::string(target_url));
  return false; /* Cancel popups in off-screen rendering mode */
}

void CWebBrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  CEF_REQUIRE_UI_THREAD();

  m_browserCount++;

  if (m_messageRouter == nullptr)
  {
    // Create the browser-side router for query handling.
    CefMessageRouterConfig config;
    config.js_query_function = "kodiQuery";
    config.js_cancel_function = "kodiQueryCancel";
    m_messageRouter = CefMessageRouterBrowserSide::Create(config);

    // Register handlers with the router.
    CreateMessageHandlers(m_messageHandlers);
    for (const auto& entry : m_messageHandlers)
      m_messageRouter->AddHandler(entry, false);
  }

  // Disable mouse cursor change if requested via the settings
  if (kodi::GetSettingBoolean("system.mouse_cursor_change_disabled"))
    browser->GetHost()->SetMouseCursorChangeDisabled(true);

  if (browser->GetHost()->GetExtension())
  {
    // Browsers hosting extension apps should auto-resize.
    browser->GetHost()->SetAutoResizeEnabled(true, CefSize(20, 20), CefSize(1000, 1000));

    CefRefPtr<CefExtension> extension = browser->GetHost()->GetExtension();
    if (chromium::app::utils::ExtensionUtils::IsInternalExtension(extension->GetPath()))
    {
      // Register the internal handler for extension resources.
      chromium::app::utils::ExtensionUtils::AddInternalExtensionToResourceManager(extension, m_resourceManager);
    }
  }

  if (!m_browser.get())
  {
    m_browser = browser;
    m_browserId = browser->GetIdentifier();

    /* Inform Kodi the control is ready */
    SetControlReady(true);
  }
}

bool CWebBrowserClient::DoClose(CefRefPtr<CefBrowser> browser)
{
  CEF_REQUIRE_UI_THREAD();
  return false; /* Allow the close. For windowed browsers this will result in the OS close event being sent */
}

void CWebBrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  CEF_REQUIRE_UI_THREAD();

  m_messageRouter->OnBeforeClose(browser);
  if (--m_browserCount == 0)
  {
    for (const auto& entry : m_messageHandlers)
    {
      m_messageRouter->RemoveHandler(entry);
      delete entry;
    }
    m_messageHandlers.clear();
    m_messageRouter = nullptr;
  }

  m_browser = nullptr;
}
//@}

/// CefDragHandler methods
//@{
bool CWebBrowserClient::OnDragEnter(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefDragData> dragData,
                                    CefDragHandler::DragOperationsMask mask)
{
  CEF_REQUIRE_UI_THREAD();

  return false;
}

void CWebBrowserClient::OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser,
                                                  CefRefPtr<CefFrame> frame,
                                                  const std::vector<CefDraggableRegion>& regions)
{
  CEF_REQUIRE_UI_THREAD();
}
//@}

/// CefRequestHandler methods
//@{
bool CWebBrowserClient::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefRequest> request,
                                       bool user_gesture,
                                       bool is_redirect)
{
  CEF_REQUIRE_UI_THREAD();

  m_messageRouter->OnBeforeBrowse(browser, frame);
  return false;
}

bool CWebBrowserClient::OnOpenURLFromTab(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& target_url,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool user_gesture)
{
  return false;
}

bool CWebBrowserClient::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                           const CefString& origin_url,
                                           bool isProxy,
                                           const CefString& host,
                                           int port,
                                           const CefString& realm,
                                           const CefString& scheme,
                                           CefRefPtr<CefAuthCallback> callback)
{
  ///TODO Useful and secure?
  return false;
}

bool CWebBrowserClient::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                       const CefString& origin_url,
                                       int64 new_size,
                                       CefRefPtr<CefRequestCallback> callback)
{
  CEF_REQUIRE_IO_THREAD();

  static const int64 max_size = 1024 * 1024 * 20; // 20mb.
  if (new_size > max_size)
    kodi::Log(ADDON_LOG_DEBUG,
              "JavaScript on '%s' requests a specific storage quota with size %li MBytes who "
              "becomes not granded",
              origin_url.ToString().c_str(), new_size / 1024 / 1024);

  // Grant the quota request if the size is reasonable.
  callback->Continue(new_size <= max_size);
  return true;
}

bool CWebBrowserClient::OnCertificateError(CefRefPtr<CefBrowser> browser,
                                           ErrorCode cert_error,
                                           const CefString& request_url,
                                           CefRefPtr<CefSSLInfo> ssl_info,
                                           CefRefPtr<CefRequestCallback> callback)
{
  return certificate::OnCertificateError(browser, cert_error, request_url, ssl_info, callback);
}

void CWebBrowserClient::OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path)
{
  kodi::Log(ADDON_LOG_ERROR, "Browser Plugin '%s' crashed (URL: '%s'",
            plugin_path.ToString().c_str(),
            browser->GetFocusedFrame()->GetURL().ToString().c_str());
}

void CWebBrowserClient::OnRenderViewReady(CefRefPtr<CefBrowser> browser)
{
  m_renderViewReady = true;
}

void CWebBrowserClient::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                                  TerminationStatus status)
{
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
//@}

/// CefFindHandler methods
//@{
void CWebBrowserClient::OnFindResult(CefRefPtr<CefBrowser> browser,
                                     int identifier,
                                     int count,
                                     const CefRect& selectionRect,
                                     int activeMatchOrdinal,
                                     bool finalUpdate)
{
  if (finalUpdate && activeMatchOrdinal <= 1)
  {
    std::string text = StringUtils::Format(kodi::GetLocalizedString(30038).c_str(), count,
                                           m_currentSearchText.c_str());
    kodi::QueueNotification(QUEUE_INFO, kodi::GetLocalizedString(30037), text);
  }
  kodi::Log(ADDON_LOG_DEBUG, "CWebBrowserClient::%s: identifier %i, count %i finalUpdate %i activeMatchOrdinal %i",
              __func__, identifier, count, finalUpdate, activeMatchOrdinal);
}
//@}

/// CefLoadHandler methods
//@{
void CWebBrowserClient::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                             bool isLoading,
                                             bool canGoBack,
                                             bool canGoForward)
{
  CEF_REQUIRE_UI_THREAD();

  SetLoadingState(isLoading, canGoBack, canGoForward);
}

void CWebBrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    TransitionType transition_type)
{
  kodi::Log(ADDON_LOG_DEBUG, "Load started (id='%d', URL='%s'", browser->GetIdentifier(),
              frame->GetURL().ToString().c_str());
  CEF_REQUIRE_UI_THREAD();

  m_isLoading = true;
  Initialize();
}

void CWebBrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  int httpStatusCode)
{
  kodi::Log(ADDON_LOG_DEBUG, "Load done with status code '%i'", httpStatusCode);
  CEF_REQUIRE_UI_THREAD();
  m_isLoading = false;

  class CHistoryReporter : public CefNavigationEntryVisitor
  {
  public:
    CHistoryReporter(std::vector<std::pair<std::string, bool>>& historyWebsiteNames)
      : m_historyWebsiteNames(historyWebsiteNames)
    {
      m_historyWebsiteNames.clear();
    }
    virtual bool Visit(CefRefPtr<CefNavigationEntry> entry,
                       bool current,
                       int index,
                       int total) override
    {
      m_historyWebsiteNames.push_back(std::pair<std::string, bool>(entry->GetTitle(), current));
      return true;
    }

  private:
    std::vector<std::pair<std::string, bool>>& m_historyWebsiteNames;
    IMPLEMENT_REFCOUNTING(CHistoryReporter);
  };
  browser->GetHost()->GetNavigationEntries(new CHistoryReporter(m_historyWebsiteNames), false);
}

void CWebBrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    ErrorCode errorCode,
                                    const CefString& errorText,
                                    const CefString& failedUrl)
{
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Don't display an error for external protocols that we allow the OS to
  // handle. See OnProtocolExecution().
  if (errorCode == ERR_UNKNOWN_URL_SCHEME)
  {
    std::string url = frame->GetURL();
    if (url.find("spotify:") == 0)
      return;
  }

  kodi::QueueNotification(QUEUE_WARNING, kodi::GetLocalizedString(30133), failedUrl.ToString());

  // Load the error page.
  certificate::CURICheck::LoadErrorPage(frame, failedUrl, errorCode, errorText);
}
//@}
