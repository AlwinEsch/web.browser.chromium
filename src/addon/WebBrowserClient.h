#pragma once
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

#define NDEBUG 1

#include <kodi/addon-instance/Web.h>

#include <queue>
#include <set>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_resource_manager.h"
#include "include/views/cef_textfield_delegate.h"
#include "p8-platform/threads/threads.h"

#include "DownloadHandler.h"
#include "UploadHandler.h"
#include "Messenger.h"
#include "Renderer/Renderer.h"

class CJSHandler;
class CJSDialogHandler;
class CRequestContextHandler;

#define TMSG_SET_CONTROL_READY        100
#define TMSG_SET_OPENED_ADDRESS       101
#define TMSG_SET_OPENED_TITLE         102
#define TMSG_SET_ICON_URL             103
#define TMSG_SET_LOADING_STATE        105
#define TMSG_SET_TOOLTIP              106
#define TMSG_SET_STATUS_MESSAGE       107
#define TMSG_HANDLE_ON_PAINT          108
#define TMSG_FULLSCREEN_MODE_CHANGE   109


class CWebBrowser;

class CWebBrowserClient
  : public kodi::addon::CWebControl,
    public CefClient,
    public CefDisplayHandler,
    public CefLifeSpanHandler,
    public CefFindHandler,
    public CefRequestHandler,
    public CefContextMenuHandler,
    public CefLoadHandler
{
public:
  CWebBrowserClient(KODI_HANDLE handle, int iUniqueClientId, CWebBrowser* instance);
  virtual ~CWebBrowserClient();

  virtual CefRefPtr<CefDialogHandler> GetDialogHandler() override;
  virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
  virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() override;
  virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
  virtual CefRefPtr<CefFindHandler> GetFindHandler() override { return this; }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

  virtual void Reload() override;
  virtual void StopLoad() override;
  virtual void GoBack() override;
  virtual void GoForward() override;
  virtual void OpenOwnContextMenu() override;
  virtual bool GetHistory(std::vector<std::string>& historyWebsiteNames, bool behindCurrent) override;
  virtual void SearchText(const std::string& text, bool forward, bool matchCase, bool findNext) override;
  virtual void StopSearch(bool clearSelection) override;



  void SetBrowser(CefRefPtr<CefBrowser> browser);

  /*!
   * @brief return the unique identification id of this control client
   */
  int GetUniqueId() { return m_iUniqueClientId; }

  bool SetInactive();
  bool SetActive();
  bool CloseComplete();
  void DestroyRenderer();


  bool OnAction(int actionId, uint32_t buttoncode, wchar_t unicode, int &nextItem);
  bool OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state);

  /*!
   * @brief Used to initialize client
   * @return true if successed
   */
  bool Initialize();

  /*!
   * @brief Open gived website or local file url
   * @param strURL url to open
   * @param single Allow only one possible website control
   * @param allowMenus Allow to have standart browser menus (can be disabled
   * on use by add-on's which use the system to implement kodi add-ons based
   * on this.
   * @return true if successed
   */
  bool OpenWebsite(const std::string& strURL, bool single, bool allowMenus);

  /*!
   * Kodi add-on render functions
   */
  virtual void Render() override;
  virtual bool Dirty() override;


  /// CefClient methods
  //@{
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) override;
  //@}

  /// CefDisplayHandler methods
  //@{
  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) override;
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
  virtual void OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls) override;
  virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) override;
  virtual bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) override;
  virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value) override;
  virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString&  message, const CefString& source, int line) override;
  //@}

  /// CefLifeSpanHandler methods
  //@{
  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
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
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
  //@}

  /// CefFindHandler methods
  //@{
  virtual void OnFindResult(CefRefPtr<CefBrowser> browser, int identifier, int count, const CefRect& selectionRect,
                            int activeMatchOrdinal, bool finalUpdate) override;
  //@}

  /// CefRequestHandler methods
  //@{
  virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) override;
  virtual bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url,
                                CefRequestHandler::WindowOpenDisposition target_disposition, bool user_gesture) override;
  virtual CefRequestHandler::ReturnValue OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                                              CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) override;
  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                                          CefRefPtr<CefRequest> request) override;
  virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                           CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, CefString& new_url) override;
  virtual bool OnResourceResponse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response) override;
  virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, bool isProxy, const CefString& host,
                                  int port, const CefString& realm, const CefString& scheme, CefRefPtr<CefAuthCallback> callback) override;
  virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser, const CefString& origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback) override;
  virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution) override;
  virtual bool OnCertificateError(CefRefPtr<CefBrowser> browser, ErrorCode cert_error, const CefString& request_url,
                                  CefRefPtr<CefSSLInfo> ssl_info, CefRefPtr<CefRequestCallback> callback) override;
  virtual void OnPluginCrashed(CefRefPtr<CefBrowser> browser, const CefString& plugin_path) override;
  virtual void OnRenderViewReady(CefRefPtr<CefBrowser> browser) override;
  virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) override;
  //@}

  /// CefContextMenuHandler methods
  //@{
  virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;
  virtual bool RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params,
                              CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback) override;
  virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params,
                                    int command_id, EventFlags event_flags) override;
  //@}

  /// CefLoadHandler methods
  //@{
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) override;
  virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) override;
  virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                           const CefString& errorText, const CefString& failedUrl) override;
  //@}

  bool IsLoading() { return m_isLoading; }
  CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }

protected:
  void SendMessage(Message& message, bool wait);

private:
  IMPLEMENT_REFCOUNTING(CWebBrowserClient);
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserClient);

  CWebBrowser* m_mainBrowserHandler;
  bool m_renderViewReady;




  std::string   m_strStartupURL;
  const int     m_iUniqueClientId;                /*!< Unique identification id of this control client */
  bool          m_bIsDirty;
  bool          m_bFocusOnEditableField;
  float         m_fMouseXScaleFactor;
  float         m_fMouseYScaleFactor;
  int           m_iMousePreviousFlags;
  cef_mouse_button_type_t m_iMousePreviousControl;

  int m_browserId;
  CefRefPtr<CefBrowser> m_browser;
  int m_browserCount; // The current number of browsers using this handler.


//  CWebGUIDialogContextMenu  m_contextMenu;

  std::queue <Message*> m_processQueue;
  P8PLATFORM::CMutex m_Mutex;


  /*!
   * CEF related sub classes to manage web parts
   */
  //{
  CefRefPtr<CefResourceManager>   m_resourceManager;        /*!< Manages the registration and delivery of resources. */
  //}


std::string popup_url_;
std::string url_;

  std::string m_lastFocusedText;
  std::string m_lastFocusedType;


  bool m_isFullScreen;


  typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

  struct SFocusedField
  {
    bool focusOnEditableField;
    bool isEditable;
    int x;
    int y;
    int width;
    int height;
    std::string type;
    std::string value;
  };

  SFocusedField m_focusedField;
  bool m_isLoading;
  std::string m_currentURL;
  std::string m_currentTitle;              /*!< Last sended website title string */
  std::string m_currentTooltip;            /*!< Last sended tooltip string */
  std::string m_currentStatusMsg;          /*!< Last sended status message string */
  std::string m_currentIcon;

  int ZoomLevelToPercentage(double zoomlevel);
  double PercentageToZoomLevel(int percent);
  void LoadErrorPage(CefRefPtr<CefFrame> frame,
                     const std::string& failed_url,
                     cef_errorcode_t error_code,
                     const std::string& other_info);
  std::string GetCertStatusString(cef_cert_status_t status);
  std::string GetSSLVersionString(cef_ssl_version_t version);
  std::string GetErrorString(cef_errorcode_t code);
  std::string GetDataURI(const std::string& data, const std::string& mime_type);
  std::string GetCertificateInformation(CefRefPtr<CefX509Certificate> cert, cef_cert_status_t certstatus);
  void CreateMessageHandlers(MessageHandlerSet& handlers);
  void HandleMessages();

  CefRefPtr<CJSDialogHandler> m_jsDialogHandler;
  CefRefPtr<CefMessageRouterBrowserSide> m_messageRouter;
  CefRefPtr<CRendererClient> m_renderer;

  std::vector<std::pair<std::string, bool>> m_historyWebsiteNames;
  std::string m_currentSearchText;

  MessageHandlerSet m_messageHandlers; // Set of Handlers registered with the message router.
};
