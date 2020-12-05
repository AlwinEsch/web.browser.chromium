/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "RequestContextHandler.h"
#include "renderer/Renderer.h"

// Dev-kit
#include "../../../lib/kodi-dev-kit/include/kodi/addon-instance/Web.h"

// CEF
#include "include/cef_app.h"
#include "include/cef_audio_handler.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_resource_manager.h"

namespace chromium
{
namespace app
{
namespace main
{

namespace gui
{
class CBrowerDialogContextMenu;
class CJSDialogHandler;
} /* namespace */

class CMainCEFProcess;

namespace interface
{
namespace v8
{
class CV8Kodi;
} /* namespace v8 */
} /* namespace interface */

class CWebBrowserClient : public kodi::addon::CWebControl,
                          public CefClient,
                          public CefDisplayHandler,
                          public CefDragHandler,
                          public CefFindHandler,
                          public CefLifeSpanHandler,
                          public CefLoadHandler,
                          public CefRequestHandler

{
public:
  CWebBrowserClient(const WEB_ADDON_GUI_PROPS& properties,
                    uint64_t handle,
                    int dataIdentifier,
                    const std::string& startURL,
                    CMainCEFProcess& mainProcess,
                    CefRefPtr<CRequestContextHandler> handler);
  virtual ~CWebBrowserClient();

  CefRefPtr<CefAudioHandler> GetAudioHandler() override;
  CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
  CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
  CefRefPtr<CefDialogHandler> GetDialogHandler() override;
  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  CefRefPtr<CefDragHandler> GetDragHandler() override { return this; }
  CefRefPtr<CefFindHandler> GetFindHandler() override { return this; }
  CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
  CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
  CefRefPtr<CefRenderHandler> GetRenderHandler() override;
  CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }

  bool SetActive();
  bool SetInactive();
  bool Initialize();
  void CloseComplete();

  bool IsFullscreen() const { return m_isFullScreen; }
  void SetContextMenuOpen(bool openClosed) { m_contextMenuOpenClosed = openClosed; }
  bool ContextMenuOpen() const { return m_contextMenuOpenClosed; }

  CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }
  void AddExtension(CefRefPtr<CefExtension> extension);

  CMainCEFProcess& GetMain() { return m_mainProcess; }
  const std::string GetClientIdentifier() const { return m_clientIdentifier; }

  // kodi::addon::CWebControl
  //@{
  void Render() override;
  bool Dirty() override;
  bool OnInit() override;
  bool OnAction(const kodi::gui::input::CAction& action, int& nextItem) override;
  bool OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state) override;
  bool OpenWebsite(const std::string& url) override;
  bool GetHistory(std::vector<std::string>& historyWebsiteNames, bool behindCurrent) override;
  void SearchText(const std::string& text, bool forward, bool matchCase, bool findNext) override;
  void StopSearch(bool clearSelection) override;
  void Reload() override;
  void StopLoad() override;
  void GoBack() override;
  void GoForward() override;
  void OpenOwnContextMenu() override;
  void ScreenSizeChange(float x, float y, float width, float height, bool fullscreen) override;
  //@}

  /// CefClient methods
  //@{
  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;
  //@}

  /// CefDisplayHandler methods
  //@{
  void OnAddressChange(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       const CefString& url) override;
  void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
  void OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
                          const std::vector<CefString>& icon_urls) override;
  void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) override;
  bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) override;
  void OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value) override;
  bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                        cef_log_severity_t level,
                        const CefString& message,
                        const CefString& source,
                        int line) override;
  //@}

  // CefDragHandler methods
  //@{
  bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefDragData> dragData,
                   CefDragHandler::DragOperationsMask mask) override;
  void OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 const std::vector<CefDraggableRegion>& regions) override;
  //@}

  /// CefFindHandler methods
  //@{
  void OnFindResult(CefRefPtr<CefBrowser> browser,
                    int identifier,
                    int count,
                    const CefRect& selectionRect,
                    int activeMatchOrdinal,
                    bool finalUpdate) override;
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
                     CefRefPtr<CefDictionaryValue>& extra_info,
                     bool* no_javascript_access) override;
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
  //@}

  /// CefLoadHandler methods
  //@{
  void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                            bool isLoading,
                            bool canGoBack,
                            bool canGoForward) override;
  void OnLoadStart(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   TransitionType transition_type) override;
  void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                 CefRefPtr<CefFrame> frame,
                 int httpStatusCode) override;
  void OnLoadError(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   ErrorCode errorCode,
                   const CefString& errorText,
                   const CefString& failedUrl) override;
  //@}

  /// CefRequestHandler methods
  //@{
  bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                      CefRefPtr<CefFrame> frame,
                      CefRefPtr<CefRequest> request,
                      bool user_gesture,
                      bool is_redirect) override;
  bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefFrame> frame,
                        const CefString& target_url,
                        CefRequestHandler::WindowOpenDisposition target_disposition,
                        bool user_gesture) override;
  bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                          const CefString& origin_url,
                          bool isProxy,
                          const CefString& host,
                          int port,
                          const CefString& realm,
                          const CefString& scheme,
                          CefRefPtr<CefAuthCallback> callback) override;
  bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                      const CefString& origin_url,
                      int64 new_size,
                      CefRefPtr<CefRequestCallback> callback) override;
  bool OnCertificateError(CefRefPtr<CefBrowser> browser,
                          ErrorCode cert_error,
                          const CefString& request_url,
                          CefRefPtr<CefSSLInfo> ssl_info,
                          CefRefPtr<CefRequestCallback> callback) override;
  void OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path) override;
  void OnRenderViewReady(CefRefPtr<CefBrowser> browser) override;
  void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) override;
  //@}

private:
  IMPLEMENT_REFCOUNTING(CWebBrowserClient);
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserClient);

  typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

  void SendKey(int key);
  bool HandleScrollEvent(int actionId);
  void CreateMessageHandlers(MessageHandlerSet& handlers);
  int ZoomLevelToPercentage(double zoomlevel);
  double PercentageToZoomLevel(int percent);

  CMainCEFProcess& m_mainProcess;

  CefRefPtr<CefBrowser> m_browser;
  CefRefPtr<CefMessageRouterBrowserSide> m_messageRouter;
  CefRefPtr<CefResourceManager> m_resourceManager; // Manages the reg. and delivery of resources.
  CefRefPtr<chromium::app::main::CRequestContextHandler> m_contextHandler;
  CefRefPtr<chromium::app::main::gui::CBrowerDialogContextMenu> m_dialogContextMenu;
  CefRefPtr<chromium::app::main::gui::CJSDialogHandler> m_jsDialogHandler;
  CefRefPtr<chromium::app::main::renderer::CRendererClient> m_renderer;
  CefRefPtr<interface::v8::CV8Kodi> m_v8Kodi;

  std::string m_clientIdentifier;
  int m_browserId{-1};
  int m_browserCount{0}; // The current number of browsers using this handler.
  bool m_contextMenuOpenClosed{false}; // To know for Keyboard that a context menu is opened
  bool m_isFullScreen{false};
  bool m_isLoading{false};
  bool m_focusOnEditableField{false};
  bool m_dragActive{false};
  bool m_renderViewReady{false};
  double m_scrollOffsetX{-1.0};
  double m_scrollOffsetY{-1.0};
  float m_fMouseXScaleFactor;
  float m_fMouseYScaleFactor;
  int m_iMousePreviousFlags{0};
  cef_mouse_button_type_t m_iMousePreviousControl{MBT_LEFT};
  std::string m_strStartupURL;
  std::string m_currentURL;
  std::string m_currentTitle; // Last sended website title string
  std::string m_currentTooltip; // Last sended tooltip string
  std::string m_currentStatusMsg; // Last sended status message string
  std::string m_currentIcon;
  std::vector<std::pair<std::string, bool>> m_historyWebsiteNames;
  std::string m_currentSearchText;
  MessageHandlerSet m_messageHandlers; // Set of Handlers registered with the message router.

  // Loaded extensions. Only accessed on the main thread.
  std::set<CefRefPtr<CefExtension>> m_extensions;
};

} /* namespace main */
} /* namespace app */
} /* namespace chromium */
