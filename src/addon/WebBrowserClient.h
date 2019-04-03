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

#pragma once

#define NDEBUG 1

#include "audio/AudioHandler.h"
#include "interface/v8/v8-kodi.h"
#include "renderer/Renderer.h"

#include "include/cef_audio_handler.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_resource_manager.h"
#include "include/views/cef_textfield_delegate.h"

#include <kodi/AudioEngine.h>
#include <kodi/addon-instance/Web.h>
#include <kodi/gui/dialogs/Select.h>
#include <queue>
#include <set>

class CBrowerDialogContextMenu;
class CJSHandler;
class CJSDialogHandler;
class CRequestContextHandler;
class CV8Kodi;

class CWebBrowser;

class CWebBrowserClient
  : public kodi::addon::CWebControl,
    public CefClient,
    public CefDisplayHandler,
    public CefLifeSpanHandler,
    public CefFindHandler,
    public CefRequestHandler,
    public CefLoadHandler
{
public:
  CWebBrowserClient(KODI_HANDLE handle, int iUniqueClientId, const std::string& startURL, CWebBrowser* instance);
  virtual ~CWebBrowserClient();

  CefRefPtr<CefAudioHandler> GetAudioHandler() override;
  CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
  CefRefPtr<CefDialogHandler> GetDialogHandler() override;
  CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
  CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
  CefRefPtr<CefRenderHandler> GetRenderHandler() override;

  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
  CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
  CefRefPtr<CefFindHandler> GetFindHandler() override { return this; }
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

  bool OnAction(int actionId, uint32_t buttoncode, wchar_t unicode, int &nextItem) override;
  bool OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state) override;
  bool OpenWebsite(const std::string& url)  override;
  void Render() override;
  bool Dirty() override;
  void Reload() override { m_browser->Reload(); }
  void StopLoad() override { m_browser->StopLoad(); }
  void GoBack() override { m_browser->GoBack(); }
  void GoForward() override { m_browser->GoForward(); }
  void OpenOwnContextMenu() override;
  bool GetHistory(std::vector<std::string>& historyWebsiteNames, bool behindCurrent) override;
  void SearchText(const std::string& text, bool forward, bool matchCase, bool findNext) override;
  void StopSearch(bool clearSelection) override;
  void ScreenSizeChange(float x, float y, float width, float height, bool fullscreen) override;

  float GetWidth() const override;
  float GetHeight() const override;

  int GetUniqueId() { return m_uniqueClientId; }
  bool IsFullscreen() { return m_isFullScreen; }

  void SetContextMenuOpen(bool openClosed) { m_contextMenuOpenClosed = openClosed; }
  bool ContextMenuOpen() const { return m_contextMenuOpenClosed; }

  bool Initialize();
  bool SetInactive();
  bool SetActive();
  bool CloseComplete();
  void DestroyRenderer();

  /// CefClient methods
  //@{
  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
  //@}

  /// CefDisplayHandler methods
  //@{
  void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) override;
  void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
  void OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls) override;
  void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) override;
  bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) override;
  void OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value) override;
  bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString&  message, const CefString& source, int line) override;
  //@}

  /// CefLifeSpanHandler methods
  //@{
  bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     const CefString& target_url,
                     const CefString& target_frame_name,
                     CefRequestHandler::WindowOpenDisposition target_disposition,
                     bool user_gesture,
                     const CefPopupFeatures& popupFeatures,
                     CefWindowInfo& windowInfo,
                     CefRefPtr<CefClient>& client,
                     CefBrowserSettings& settings,
                     bool* no_javascript_access) override;
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
  //@}

  /// CefFindHandler methods
  //@{
  void OnFindResult(CefRefPtr<CefBrowser> browser, int identifier, int count, const CefRect& selectionRect,
                            int activeMatchOrdinal, bool finalUpdate) override;
  //@}

  /// CefRequestHandler methods
  //@{
  bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool user_gesture, bool is_redirect) override;
  bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url,
                        CefRequestHandler::WindowOpenDisposition target_disposition, bool user_gesture) override;
  CefRequestHandler::ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                                      CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) override;
  CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                                   CefRefPtr<CefRequest> request) override;
  void OnResourceRedirect(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                          CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, CefString& new_url) override;
  bool OnResourceResponse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                          CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response) override;
  bool GetAuthCredentials(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, bool isProxy, const CefString& host,
                          int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback) override;
  bool OnQuotaRequest(CefRefPtr<CefBrowser> browser, const CefString& origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback) override;
  void OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution) override;
  bool OnCertificateError(CefRefPtr<CefBrowser> browser, ErrorCode cert_error, const CefString& request_url,
                          CefRefPtr<CefSSLInfo> ssl_info, CefRefPtr<CefRequestCallback> callback) override;
  void OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path) override;
  void OnRenderViewReady(CefRefPtr<CefBrowser> browser) override;
  void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) override;
  //@}


  /// CefLoadHandler methods
  //@{
  void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) override;
  void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) override;
  void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
  void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                   const CefString& errorText, const CefString& failedUrl) override;
  //@}

  bool IsLoading() { return m_isLoading; }
  CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }

  CWebBrowser& GetMain() { return *m_mainBrowserHandler; }

private:
  IMPLEMENT_REFCOUNTING(CWebBrowserClient);
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserClient);

  typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

  void SendKey(int key);
  bool HandleScrollEvent(int actionId);
  void CreateMessageHandlers(MessageHandlerSet& handlers);

  int ZoomLevelToPercentage(double zoomlevel);
  double PercentageToZoomLevel(int percent);

  const int m_uniqueClientId;                // Unique identification id of this control client
  CWebBrowser* m_mainBrowserHandler;
  int m_browserId;
  int m_browserCount; // The current number of browsers using this handler.

  double m_scrollOffsetX = -1.0;
  double m_scrollOffsetY = -1.0;

  bool m_contextMenuOpenClosed = false; // To know for Keyboard that a context menu is opened
  bool m_focusOnEditableField = false;

  bool m_renderViewReady;
  std::string m_strStartupURL;
  float m_fMouseXScaleFactor;
  float m_fMouseYScaleFactor;
  int m_iMousePreviousFlags;
  cef_mouse_button_type_t m_iMousePreviousControl;

  bool m_isFullScreen;
  bool m_isLoading;
  std::string m_currentURL;
  std::string m_currentTitle;              // Last sended website title string
  std::string m_currentTooltip;            // Last sended tooltip string
  std::string m_currentStatusMsg;          // Last sended status message string
  std::string m_currentIcon;
  std::vector<std::pair<std::string, bool>> m_historyWebsiteNames;
  std::string m_currentSearchText;

  MessageHandlerSet m_messageHandlers; // Set of Handlers registered with the message router.

  CefRefPtr<CefBrowser> m_browser;
  CefRefPtr<CefMessageRouterBrowserSide> m_messageRouter;
  CefRefPtr<CefResourceManager> m_resourceManager;        // Manages the registration and delivery of resources.

  CefRefPtr<CAudioHandler> m_audioHandler;
  CefRefPtr<CBrowerDialogContextMenu> m_dialogContextMenu;
  CefRefPtr<CJSDialogHandler> m_jsDialogHandler;
  CefRefPtr<CRendererClient> m_renderer;
  CV8Kodi m_v8Kodi;
};
