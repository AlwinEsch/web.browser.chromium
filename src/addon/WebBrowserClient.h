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

#include "DownloadHandler.h"
#include "UploadHandler.h"
#include "Messenger.h"
#include "Renderer/Renderer.h"

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
#include <p8-platform/threads/threads.h>
#include <queue>
#include <set>

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
    public P8PLATFORM::CThread,
    public CefClient,
    public CefDisplayHandler,
    public CefLifeSpanHandler,
    public CefFindHandler,
    public CefRequestHandler,
    public CefContextMenuHandler,
    public CefLoadHandler,
    public CefAudioHandler
{
public:
  CWebBrowserClient(KODI_HANDLE handle, int iUniqueClientId, const std::string& startURL, CWebBrowser* instance);
  virtual ~CWebBrowserClient();

  CefRefPtr<CefDialogHandler> GetDialogHandler() override;
  CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
  CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() override;
  CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
  CefRefPtr<CefRenderHandler> GetRenderHandler() override;
  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
  CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
  CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
  CefRefPtr<CefFindHandler> GetFindHandler() override { return this; }
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
  CefRefPtr<CefAudioHandler> GetAudioHandler() override { return this; }

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
  bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString&  message, const CefString& source, int line) override;
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
  bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) override;
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

  /// CefContextMenuHandler methods
  //@{
  void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;
  bool RunContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params,
                      CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback) override;
  bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params,
                            int command_id, EventFlags event_flags) override;
  //@}

  /// CefLoadHandler methods
  //@{
  void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) override;
  void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) override;
  void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
  void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                   const CefString& errorText, const CefString& failedUrl) override;
  //@}

  /// CefAudioHandler methods
  //@{
  void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser, int audio_stream_id, int channels, ChannelLayout channel_layout,
                            int sample_rate, int original_bits_per_sample, int frames_per_buffer) override;
  void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, int audio_stream_id, const void* data, int data_length, int64_t pts) override;
  void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser, int audio_stream_id) override;
  //@}

  bool IsLoading() { return m_isLoading; }
  CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }

protected:
  void SendGUIMessage(Message& message, bool wait);

  virtual void* Process(void) override;

private:
  const int m_uniqueClientId;                // Unique identification id of this control client

  typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

  IMPLEMENT_REFCOUNTING(CWebBrowserClient);
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserClient);

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

  void HandleGUIMessages();
  std::queue <Message*> m_processGUIQueue;
  P8PLATFORM::CMutex m_processGUIMutex;

  CWebBrowser* m_mainBrowserHandler;
  bool m_renderViewReady;
  std::string m_strStartupURL;
  bool m_bFocusOnEditableField;
  float m_fMouseXScaleFactor;
  float m_fMouseYScaleFactor;
  int m_iMousePreviousFlags;
  cef_mouse_button_type_t m_iMousePreviousControl;

  int m_browserId;
  CefRefPtr<CefBrowser> m_browser;
  int m_browserCount; // The current number of browsers using this handler.

  CefRefPtr<CJSDialogHandler> m_jsDialogHandler;
  CefRefPtr<CefMessageRouterBrowserSide> m_messageRouter;
  CefRefPtr<CRendererClient> m_renderer;
  CefRefPtr<CefResourceManager> m_resourceManager;        // Manages the registration and delivery of resources.

  typedef struct SFocusedField
  {
    bool focusOnEditableField;
    bool isEditable;
    int x;
    int y;
    int width;
    int height;
    std::string type;
    std::string value;
  } SFocusedField;
  SFocusedField m_focusedField;

  bool m_isFullScreen;
  bool m_isLoading;
  std::string m_currentURL;
  std::string m_currentTitle;              // Last sended website title string
  std::string m_currentTooltip;            // Last sended tooltip string
  std::string m_currentStatusMsg;          // Last sended status message string
  std::string m_currentIcon;
  std::vector<std::pair<std::string, bool>> m_historyWebsiteNames;
  std::string m_currentSearchText;
  std::map<int, kodi::audioengine::CAddonAEStream*> m_audioStreams;
  unsigned int m_frames;

  MessageHandlerSet m_messageHandlers; // Set of Handlers registered with the message router.





  #define TMSG_CLIENTTHREAD_DIALOG_SELECT   200
  #define TMSG_CLIENTTHREAD_DIALOG_KEYBOARD 201

  struct ClientThreadData
  {
    ClientThreadData() = default;
    virtual ~ClientThreadData() = default;
  };

  struct ClientThreadData_DialogSelect : public ClientThreadData
  {
    virtual ~ClientThreadData_DialogSelect() = default;
    std::string url;
    std::string type;
    std::string header;
    std::string value;
    std::string id;
    std::string name;
    std::string markup;
    std::vector<SSelectionEntry> entries;
  };

  struct ClientThreadData_DialogKeyboard : public ClientThreadData
  {
    virtual ~ClientThreadData_DialogKeyboard() = default;
    std::string url;
    std::string type;
    std::string header;
    std::string value;
  };

  class ThreadMessage
  {
  public:
    ThreadMessage(int dwMessageType)
      : dwMessage(dwMessageType),
        data(nullptr)
    {
      if (dwMessageType == TMSG_CLIENTTHREAD_DIALOG_SELECT)
        data = new ClientThreadData_DialogSelect;
      else if (dwMessageType == TMSG_CLIENTTHREAD_DIALOG_KEYBOARD)
        data = new ClientThreadData_DialogKeyboard;
    }
    ~ThreadMessage() = default;

    unsigned int dwMessage;
    int param1;
    int param2;
    int param3;
    std::string strParam;
    std::vector<std::string> params;
    std::shared_ptr<P8PLATFORM::CEvent> waitEvent;
    void* lpVoid;
    ClientThreadData* data;
  };

  void SendThreadMessage(ThreadMessage& message, bool wait);
  void HandleThreadMessages();
  std::queue <ThreadMessage*> m_processThreadQueue;
  P8PLATFORM::CMutex m_processThreadMutex;
  P8PLATFORM::CEvent m_processThreadEvent;
};
