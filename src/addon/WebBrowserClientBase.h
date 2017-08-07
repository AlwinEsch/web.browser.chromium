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

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_resource_manager.h"
#include "include/views/cef_textfield_delegate.h"
#include "p8-platform/threads/threads.h"

#include "DownloadHandler.h"
#include "UploadHandler.h"

class CJSHandler;
class CJSDialogHandler;


#define TMSG_SET_CONTROL_READY        100
#define TMSG_SET_OPENED_ADDRESS       101
#define TMSG_SET_OPENED_TITLE         102
#define TMSG_SET_ICON_URL             103
#define TMSG_BROWSER_CLOSE            104
#define TMSG_SET_LOADING_STATE        105
#define TMSG_SET_TOOLTIP              106
#define TMSG_SET_STATUS_MESSAGE       107
#define TMSG_HANDLE_ON_PAINT          108
#define TMSG_FULLSCREEN_MODE_CHANGE   109

struct MessageCallback
{
  void (*callback)(void *userptr);
  void *userptr;
};

/*!
 * Structure to pass browser data to render thread, used on related childs
 */
typedef struct
{
  MessageCallback       client;
  CefRefPtr<CefBrowser> browser;
  CefBrowserHost::PaintElementType      type;
  const CefRenderHandler::RectList*       dirtyRects;
  const void*           buffer;
  int                   width;
  int                   height;
} OnPaintMessage;

typedef struct
{
  unsigned int dwMessage;
  int param1;
  int param2;
  int param3;
  std::string strParam;
  std::vector<std::string> params;
  std::shared_ptr<P8PLATFORM::CEvent> waitEvent;
  void* lpVoid;
} Message;




class CWebBrowser;

class CWebBrowserClientBase
  : public kodi::addon::CWebControl,
    public CefClient,
    public CefDisplayHandler,
    public CefDragHandler,
    public CefFindHandler,
    public CefFocusHandler,
    public CefLifeSpanHandler,
    public CefRenderHandler,
    public CefRequestHandler,
    public CefRequestContextHandler,
    public CefContextMenuHandler,
    public CefLoadHandler
{
public:
  CWebBrowserClientBase(KODI_HANDLE handle, int iUniqueClientId, CWebBrowser* instance);
  virtual ~CWebBrowserClientBase();

  virtual CefRefPtr<CefDialogHandler> GetDialogHandler() override;
  virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
  virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() override;
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;
  virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;

  /*!
   * @brief CefDisplayHandler methods
   *
   * Implement this interface to handle events related to browser display state.
   * The methods of this class will be called on the UI thread.
   */
  //@{
  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) override;
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
  virtual void OnFaviconURLChange(CefRefPtr<CefBrowser> browser, const std::vector<CefString>& icon_urls) override;
  virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) override;
  virtual bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) override;
  virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser, const CefString& value) override;
  virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString&  message, const CefString& source, int line) override;
  //@}

  void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node);


  virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE { return this; }
  virtual CefRefPtr<CefFindHandler> GetFindHandler() OVERRIDE { return this; }
  virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE { return this; }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this; }
  //virtual CefRefPtr<CefRequestContextHandler> GetHandler() OVERRIDE { return this; }


  virtual void Reload() override;
  virtual void StopLoad() override;
  virtual void GoBack() override;
  virtual void GoForward() override;
  virtual void OpenOwnContextMenu() override;



  void SetBrowser(CefRefPtr<CefBrowser> browser);

  /*!
   * @brief return the unique identification id of this control client
   */
  int GetUniqueId() { return m_iUniqueClientId; }

  /*!
   * @brief Used to inform client to stay inactive and store the time.
   * @return true if need removed, false to leave open for a time
   */
  bool SetInactive();

  /*!
   * @brief Used to inform client to open inactive client again
   * @return true if successed
   */
  bool SetActive();

  /*!
   * @brief Inactive time
   * @return the time to leave open, on every call it become less.
   */
  int CurrentInactiveCountdown();

  /*!
   * @brief Returns the number of browsers currently using this handler.
   * @return number
   */
  int GetBrowserCount() const { return m_iBrowserCount; }

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
  virtual bool Dirty();

  /*!
   * @brief CefClient methods
   *
   * Implement this interface to provide handler implementations.
   */
  //{
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) override;
  //}


  /*!
   * @brief CefDragHandler methods
   *
   * Implement this interface to handle events related to dragging. The methods of
   * this class will be called on the UI thread.
   */
  //{

  virtual bool OnDragEnter(                                           /// Called when an external drag event enters the browser window. |dragData|
      CefRefPtr<CefBrowser>                 browser,                  /// contains the drag event data and |mask| represents the type of drag
      CefRefPtr<CefDragData>                dragData,                 /// operation. Return false for default drag handling behavior or true to
      CefRenderHandler::DragOperationsMask  mask)                     /// cancel the drag event.
                        OVERRIDE;                                     ///
  //}

  /*!
   * @brief CefFindHandler methods
   *
   * Implement this interface to handle events related to find results. The
   * methods of this class will be called on the UI thread.
   */
  //{

  virtual void OnFindResult(
      CefRefPtr<CefBrowser>                 browser,                  ///
      int                                   identifier,
      int                                   count,
      const CefRect&                        selectionRect,
      int                                   activeMatchOrdinal,
      bool                                  finalUpdate)
                        OVERRIDE;                                     ///
  //}

  /*!
   * @brief CefFocusHandler methods
   *
   * Implement this interface to handle events related to focus. The methods of
   * this class will be called on the UI thread.
   */
  //{

  virtual void OnTakeFocus(
      CefRefPtr<CefBrowser>                 browser,                  ///
      bool                                  next)
                        OVERRIDE;                                     ///

  virtual bool OnSetFocus(
      CefRefPtr<CefBrowser>                 browser,                  ///
      FocusSource                           source)
                        OVERRIDE;                                     ///

  virtual void OnGotFocus(
      CefRefPtr<CefBrowser>                 browser)                  ///
                        OVERRIDE;                                     ///
  //}

  /*!
   * @brief CefLifeSpanHandler methods
   *
   * Implement this interface to handle events related to browser life span. The
   * methods of this class will be called on the UI thread unless otherwise
   * indicated.
   */
  //{

  virtual bool OnBeforePopup(                                         /// Called on the IO thread before a new popup browser is created. The |browser| and |frame| values
      CefRefPtr<CefBrowser>                 browser,                  /// represent the source of the popup request. The |target_url| and |target_frame_name| values
      CefRefPtr<CefFrame>                   frame,                    /// indicate where the popup browser should navigate and may be empty if not specified with the
      const CefString&                      target_url,               /// request. The |target_disposition| value indicates where the user intended to open the popup
      const CefString&                      target_frame_name,        /// (e.g. current tab, new tab, etc). The |user_gesture| value will be true if the popup was opened
      CefRequestHandler::WindowOpenDisposition target_disposition,    /// via explicit user gesture (e.g. clicking a link) or false if the popup opened automatically
      bool                                  user_gesture,             /// (e.g. via the DomContentLoaded event). The |popupFeatures| structure contains additional
      const CefPopupFeatures&               popupFeatures,            /// information about the requested popup window. To allow creation of the popup browser optionally
      CefWindowInfo&                        windowInfo,               /// modify |windowInfo|, |client|, |settings| and |no_javascript_access| and return false. To cancel
      CefRefPtr<CefClient>&                 client,                   /// creation of the popup browser return true. The |client| and |settings| values will default to
      CefBrowserSettings&                   settings,                 /// the source browser's values. If the |no_javascript_access| value is set to false the new browser
      bool*                                 no_javascript_access)     /// will not be scriptable and may not be hosted in the same renderer process as the source browser.
                        OVERRIDE;                                     ///

  virtual void OnAfterCreated(                                        /// Called after a new browser is created.
      CefRefPtr<CefBrowser>                 browser)                  ///
                        OVERRIDE;                                     ///

//   virtual bool RunModal(                                              /// Called when a modal window is about to display and the modal loop should
//       CefRefPtr<CefBrowser>                 browser)                  /// begin running. Return false to use the default modal loop implementation or
//                         OVERRIDE;                                     /// true to use a custom implementation.

  virtual bool DoClose(                                               /// Called when a browser has recieved a request to close. This may result directly from a call to
      CefRefPtr<CefBrowser>                 browser)                  /// CefBrowserHost::CloseBrowser() or indirectly if the browser is a top-level OS window created by
                        OVERRIDE;                                     /// CEF and the user attempts to close the window. This method will be called after the JavaScript
                                                                      /// 'onunload' event has been fired. It will not be called for browsers after the associated OS
                                                                      /// window has been destroyed (for those browsers it is no longer possible to cancel the close).
                                                                      ///
                                                                      /// If CEF created an OS window for the browser returning false will send an OS close notification
                                                                      /// to the browser window's top-level owner (e.g. WM_CLOSE on Windows, performClose: on OS-X and
                                                                      /// "delete_event" on Linux). If no OS window exists (window rendering disabled) returning false
                                                                      /// will cause the browser object to be destroyed immediately. Return true if the browser is
                                                                      /// parented to another window and that other window needs to receive close notification via some
                                                                      /// non-standard technique.
                                                                      ///
                                                                      /// If an application provides its own top-level window it should handle OS close notifications by
                                                                      /// calling CefBrowserHost::CloseBrowser(false) instead of immediately closing (see the example
                                                                      /// below). This gives CEF an opportunity to process the 'onbeforeunload' event and optionally
                                                                      /// cancel the close before DoClose() is called.
                                                                      ///
                                                                      /// The CefLifeSpanHandler::OnBeforeClose() method will be called immediately before the browser
                                                                      /// object is destroyed. The application should only exit after OnBeforeClose() has been called for
                                                                      /// all existing browsers.
                                                                      ///
                                                                      /// If the browser represents a modal window and a custom modal loop implementation was provided in
                                                                      /// CefLifeSpanHandler::RunModal() this callback should be used to restore the opener window to a
                                                                      /// usable state.
                                                                      ///
                                                                      /// By way of example consider what should happen during window close when the browser is parented
                                                                      /// to an application-provided top-level OS window.
                                                                      /// 1.  User clicks the window close button which sends an OS close notification (e.g. WM_CLOSE on
                                                                      ///     Windows, performClose: on OS-X and "delete_event" on Linux).
                                                                      /// 2.  Application's top-level window receives the close notification and:
                                                                      ///     A. Calls CefBrowserHost::CloseBrowser(false).
                                                                      ///     B. Cancels the window close.
                                                                      /// 3.  JavaScript 'onbeforeunload' handler executes and shows the close confirmation dialog (which
                                                                      ///     can be overridden via CefJSDialogHandler::OnBeforeUnloadDialog()).
                                                                      /// 4.  User approves the close.
                                                                      /// 5.  JavaScript 'onunload' handler executes.
                                                                      /// 6.  Application's DoClose() handler is called. Application will:
                                                                      ///     A. Set a flag to indicate that the next close attempt will be allowed.
                                                                      ///     B. Return false.
                                                                      /// 7.  CEF sends an OS close notification.
                                                                      /// 8.  Application's top-level window receives the OS close notification and allows the window to
                                                                      ///     close based on the flag from #6B.
                                                                      /// 9.  Browser OS window is destroyed.
                                                                      /// 10. Application's CefLifeSpanHandler::OnBeforeClose() handler is called and the browser object
                                                                      ///     is destroyed.
                                                                      /// 11. Application exits by calling CefQuitMessageLoop() if no other browsers exist.

  virtual void OnBeforeClose(                                         /// Called just before a browser is destroyed. Release all references to the browser object and do
      CefRefPtr<CefBrowser>                 browser)                  /// not attempt to execute any methods on the browser object after this callback returns. If this
                        OVERRIDE;                                     /// is a modal window and a custom modal loop implementation was provided in RunModal() this callback
                                                                      /// should be used to exit the custom modal loop. See DoClose() documentation for additional usage information.
  //}


  /*!
   * @brief CefRenderHandler methods
   *
   * Implement this interface to handle events when window rendering is disabled.
   * The methods of this class will be called on the UI thread.
   */
  //{

  virtual bool GetRootScreenRect(                                  ///<-- Called to retrieve the root window rectangle in screen coordinates. Return
      CefRefPtr<CefBrowser>                 browser,                  /// true if the rectangle was provided.
      CefRect& rect) { return false; }                                ///

  virtual bool GetViewRect(                                        ///<-- Called to retrieve the view rectangle which is relative to screen
      CefRefPtr<CefBrowser>                 browser,                  /// coordinates. Return true if the rectangle was provided.
      CefRect&                              rect)=0;                  ///

  virtual bool GetScreenPoint(                                     ///<-- Called to retrieve the translation from view coordinates to actual screen
      CefRefPtr<CefBrowser>                 browser,                  /// coordinates. Return true if the screen coordinates were provided.
      int                                   viewX,                    ///
      int                                   viewY,                    ///
      int&                                  screenX,                  ///
      int&                                  screenY) { return false; }///

  virtual bool GetScreenInfo(                                      ///<-- Called to allow the client to fill in the CefScreenInfo object with appropriate values. Return
      CefRefPtr<CefBrowser>                 browser,                  /// true if the |screen_info| structure has been modified.
      CefScreenInfo& screen_info) { return false; }                   /// If the screen info rectangle is left empty the rectangle from GetViewRect will be used. If the
                                                                      /// rectangle is still empty or invalid popups may not be drawn correctly.
  virtual void OnPopupShow(
      CefRefPtr<CefBrowser>                 browser,               ///<-- Called when the browser wants to show or hide the popup widget. The popup
      bool                                  show) {}                  /// should be shown if |show| is true and hidden if |show| is false.

  virtual void OnPopupSize(
      CefRefPtr<CefBrowser>                 browser,               ///<-- Called when the browser wants to move or resize the popup widget. |rect|
      const CefRect&                        rect) {}                  /// contains the new location and size in view coordinates.

  virtual void OnPaint(                                            ///<-- Called when an element should be painted. Pixel values passed to this method are scaled relative
      CefRefPtr<CefBrowser>                 browser,                  /// to view coordinates based on the value of CefScreenInfo.device_scale_factor returned from
      CefRenderHandler::PaintElementType    type,                     /// GetScreenInfo. |type| indicates whether the element is the view or the popup widget. |buffer|
      const RectList&                       dirtyRects,               /// contains the pixel data for the whole image. |dirtyRects| contains the set of rectangles in
      const void*                           buffer,                   /// pixel coordinates that need to be repainted. |buffer| will be |width|*|height|*4 bytes in size
      int                                   width,                    /// and represents a BGRA image with an upper-left origin.
      int                                   height) =0;               ///

  virtual void OnCursorChange(                                     ///<-- Called when the browser's cursor has changed. If |type| is CT_CUSTOM then
      CefRefPtr<CefBrowser>                 browser,                  /// |custom_cursor_info| will be populated with the custom cursor information.
      CefCursorHandle                       cursor,                   ///
      CefRenderHandler::CursorType          type,
      const CefCursorInfo&                  custom_cursor_info) {}    /// Called when the user starts dragging content in the web view. Contextual information about the
                                                                      /// dragged content is supplied by |drag_data|. (|x|, |y|) is the drag start location in screen
                                                                      /// coordinates. OS APIs that run a system message loop may be used within the StartDragging call.
  virtual bool StartDragging(                                      ///<--
      CefRefPtr<CefBrowser>                 browser,                  /// Return false to abort the drag operation. Don't call any of CefBrowserHost::DragSource*Ended*
      CefRefPtr<CefDragData>                drag_data,                /// methods after returning false.
      CefRenderHandler::DragOperationsMask  allowed_ops,              ///
      int                                   x,                        /// Return true to handle the drag operation. Call CefBrowserHost::DragSourceEndedAt and
      int                                   y) { return false; }      /// DragSourceSystemDragEnded either synchronously or asynchronously to inform the web view that
                                                                      /// the drag operation has ended.
  virtual void UpdateDragCursor(
      CefRefPtr<CefBrowser>                 browser,               ///<-- Called when the web view wants to update the mouse cursor during a
      CefRenderHandler::DragOperation       operation) {}             /// drag & drop operation. |operation| describes the allowed operation
                                                                      /// (none, move, copy, link).
  virtual void OnScrollOffsetChanged(
      CefRefPtr<CefBrowser>                 browser,               ///<-- Called when the scroll offset has changed.
      double                                x,                        ///
      double                                y) {}                     ///
  //}
  /*!
   * @brief CefRequestHandler methods
   *
   * Implement this interface to handle events related to browser requests. The
   * methods of this class will be called on the thread indicated.
   */
  //{
                                                                      /// Called on the UI thread before browser navigation. Return true to cancel the navigation or
  virtual bool OnBeforeBrowse(                                     ///<-- false to allow the navigation to proceed. The |request| object cannot be modified in this
      CefRefPtr<CefBrowser>                 browser,                  /// callback. CefLoadHandler::OnLoadingStateChange will be called twice in all cases. If the
      CefRefPtr<CefFrame>                   frame,                    /// navigation is allowed CefLoadHandler::OnLoadStart and CefLoadHandler::OnLoadEnd will be
      CefRefPtr<CefRequest>                 request,                  /// called. If the navigation is canceled CefLoadHandler::OnLoadError will be called with an
      bool                                  is_redirect)              /// |errorCode| value of ERR_ABORTED.
                        OVERRIDE;
                                                                      /// Called on the UI thread before OnBeforeBrowse in certain limited cases where navigating a new
  virtual bool OnOpenURLFromTab(                                   ///<-- or different browser might be desirable. This includes user-initiated navigation that might open
      CefRefPtr<CefBrowser>                 browser,                  /// in a special way (e.g. links clicked via middle-click or ctrl + left-click) and certain types
      CefRefPtr<CefFrame>                   frame,                    /// of cross-origin navigation initiated from the renderer process (e.g. navigating the top-level
      const CefString&                      target_url,               /// frame to/from a file URL). The |browser| and |frame| values represent the source of the navigation.
      CefRequestHandler::WindowOpenDisposition target_disposition,    /// The |target_disposition| value indicates where the user intended to navigate the browser based on
      bool                                  user_gesture)             /// standard Chromium behaviors (e.g. current tab, new tab, etc). The |user_gesture| value will be true
                        OVERRIDE;                                     /// if the browser navigated via explicit user gesture (e.g. clicking a link) or false if it navigated
                                                                      /// automatically (e.g. via the DomContentLoaded event). Return true to cancel the navigation or false
  virtual CefRequestHandler::ReturnValue OnBeforeResourceLoad(        /// to allow the navigation to proceed in the source browser's top-level frame.
      CefRefPtr<CefBrowser>                 browser,
      CefRefPtr<CefFrame>                   frame,                 ///<-- Called on the IO thread before a resource request is loaded. The |request| object may be
      CefRefPtr<CefRequest>                 request,                  /// modified. Return RV_CONTINUE to continue the request immediately. Return RV_CONTINUE_ASYNC and
      CefRefPtr<CefRequestCallback>         callback)                 /// call CefRequestCallback::Continue() at a later time to continue or cancel the request
                        OVERRIDE;                                     /// asynchronously. Return RV_CANCEL to cancel the request immediately.

  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(        ///<-- Called on the IO thread before a resource is loaded. To allow the resource
      CefRefPtr<CefBrowser>                 browser,                  /// to load normally return NULL. To specify a handler for the resource return
      CefRefPtr<CefFrame>                   frame,                    /// a CefResourceHandler object. The |request| object should not be modified in
      CefRefPtr<CefRequest>                 request)                  /// this callback.
                        OVERRIDE;                                     ///

  virtual void OnResourceRedirect(                                 ///<-- Called on the IO thread when a resource load is redirected. The |request|
      CefRefPtr<CefBrowser>                 browser,                  /// parameter will contain the old URL and other request-related information.
      CefRefPtr<CefFrame>                   frame,                    /// The |new_url| parameter will contain the new URL and can be changed if
      CefRefPtr<CefRequest>                 request,                  /// desired. The |request| object cannot be modified in this callback.
      CefRefPtr<CefResponse>                response,                 ///
      CefString&                            new_url)                  ///
                        OVERRIDE;                                     ///

  virtual bool OnResourceResponse(                                 ///<-- Called on the IO thread when a resource response is received. To allow the
      CefRefPtr<CefBrowser>                 browser,                  /// resource to load normally return false. To redirect or retry the resource
      CefRefPtr<CefFrame>                   frame,                    /// modify |request| (url, headers or post body) and return true. The
      CefRefPtr<CefRequest>                 request,                  /// |response| object cannot be modified in this callback.
      CefRefPtr<CefResponse>                response)                 ///
                        OVERRIDE;                                     ///

  virtual bool GetAuthCredentials(                                 ///<-- Called on the IO thread when the browser needs credentials from the user.
      CefRefPtr<CefBrowser>                 browser,                  /// |isProxy| indicates whether the host is a proxy server. |host| contains the
      CefRefPtr<CefFrame>                   frame,                    /// hostname and |port| contains the port number. Return true to continue the
      bool                                  isProxy,                  /// request and call CefAuthCallback::Continue() either in this method or
      const CefString&                      host,                     /// at a later time when the authentication information is available. Return
      int                                   port,                     /// false to cancel the request immediately.
      const CefString&                      realm,                    ///
      const CefString&                      scheme,                   ///
      CefRefPtr<CefAuthCallback>            callback)                 ///
                        OVERRIDE;                                     ///

  virtual bool OnQuotaRequest(                                     ///<-- Called on the IO thread when JavaScript requests a specific storage quota
      CefRefPtr<CefBrowser>                 browser,                  /// size via the webkitStorageInfo.requestQuota function. |origin_url| is the
      const CefString&                      origin_url,               /// origin of the page making the request. |new_size| is the requested quota
      int64                                 new_size,                 /// size in bytes. Return true to continue the request and call
      CefRefPtr<CefRequestCallback>         callback)                 /// CefRequestCallback::Continue() either in this method or at a later time to
                        OVERRIDE;                                     /// grant or deny the request. Return false to cancel the request immediately.

  virtual void OnProtocolExecution(                                ///<-- Called on the UI thread to handle requests for URLs with an unknown
      CefRefPtr<CefBrowser>                 browser,                  /// protocol component. Set |allow_os_execution| to true to attempt execution
      const CefString&                      url,                      /// via the registered OS protocol handler, if any.
      bool&                                 allow_os_execution)       /// SECURITY WARNING: YOU SHOULD USE THIS METHOD TO ENFORCE RESTRICTIONS BASED
                        OVERRIDE;                                     /// ON SCHEME, HOST OR OTHER URL ANALYSIS BEFORE ALLOWING OS EXECUTION.

  virtual bool OnCertificateError(                                 ///<-- Called on the UI thread to handle requests for URLs with an invalid
      CefRefPtr<CefBrowser>                 browser,                  /// SSL certificate. Return true and call CefRequestCallback::Continue() either
      ErrorCode                             cert_error,               /// in this method or at a later time to continue or cancel the request. Return
      const CefString&                      request_url,              /// false to cancel the request immediately. If |callback| is empty the error
      CefRefPtr<CefSSLInfo>                 ssl_info,                 /// cannot be recovered from and the request will be canceled automatically.
      CefRefPtr<CefRequestCallback>         callback)                 /// If CefSettings.ignore_certificate_errors is set all invalid certificates
                        OVERRIDE;                                     /// will be accepted without calling this method.

  virtual void OnPluginCrashed(                                    ///<-- Called on the browser process UI thread when a plugin has crashed.
      CefRefPtr<CefBrowser>                 browser,                  /// |plugin_path| is the path of the plugin that crashed.
      const CefString&                      plugin_path)              ///
                        OVERRIDE;                                     ///

  virtual void OnRenderViewReady(                                  ///<-- Called on the browser process UI thread when the render view associated
      CefRefPtr<CefBrowser>                 browser)                  /// with |browser| is ready to receive/handle IPC messages in the render
                        OVERRIDE;                                     /// process.

  virtual void OnRenderProcessTerminated(                          ///<-- Called on the browser process UI thread when the render process
      CefRefPtr<CefBrowser>                 browser,                  /// terminates unexpectedly. |status| indicates how the process
      TerminationStatus                     status)                   /// terminated.
                        OVERRIDE;                                     ///
  //}



  /*!
   * @brief CefRequestContextHandler methods
   *
   * Implement this interface to handle events related to browser requests. The
   * methods of this class will be called on the thread indicated.
   */
  //{

  virtual CefRefPtr<CefCookieManager> GetCookieManager() OVERRIDE;

  virtual bool OnBeforePluginLoad(                                 ///<--
      const CefString&                      mime_type,                ///
      const CefString&                      plugin_url,               ///
      bool                                  is_main_frame,            ///
      const CefString&                      top_origin_url,           ///
      CefRefPtr<CefWebPluginInfo>           plugin_info,              ///
      PluginPolicy*                         plugin_policy)            ///
                        OVERRIDE;                                     ///
  //}
  IMPLEMENT_REFCOUNTING(CWebBrowserClientBase);

  CWebBrowser* m_mainBrowserHandler;







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
  CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }

protected:
  void SendMessage(Message& message, bool wait);

  float         m_BackgroundColor[4];

private:
  std::string   m_strStartupURL;
  const int     m_iUniqueClientId;                /*!< Unique identification id of this control client */
  time_t        m_inactivateTime;                 /*!< Time where client becomes set inactive to handle the timeout */
  int           m_iBrowserCount;                  /*!< The current number of browsers using this handler. */
  bool          m_bIsDirty;
  bool          m_bFocusOnEditableField;
  float         m_fMouseXScaleFactor;
  float         m_fMouseYScaleFactor;
  int           m_iMousePreviousFlags;
  cef_mouse_button_type_t m_iMousePreviousControl;

  int                     m_BrowserId;
  CefRefPtr<CefBrowser>   m_Browser;


//  CWebGUIDialogContextMenu  m_contextMenu;

  void HandleMessages();

  std::queue <Message*> m_processQueue;
  P8PLATFORM::CMutex m_Mutex;



  bool                          m_isClosing;
  typedef std::list< CefRefPtr<CefBrowser> > browserList;
  browserList                   m_popupBrowsers;          /* List of any popup browser windows. Only accessed on the CEF UI thread. */

  /*!
   * Compare values to prevent call backs with same values.
   */
  //{
  std::string                     m_lastTitle;              /*!< Last sended website title string */
  std::string                     m_lastTooltip;            /*!< Last sended tooltip string */
  std::string                     m_lastStatusMsg;          /*!< Last sended status message string */
  //}

  /*!
   * CEF related sub classes to manage web parts
   */
  //{
  CefRefPtr<CefResourceManager>   m_resourceManager;        /*!< Manages the registration and delivery of resources. */
  //}



  std::string m_lastFocusedText;
  std::string m_lastFocusedType;


  bool m_isFullScreen;


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

  int ZoomLevelToPercentage(double zoomlevel);
  double PercentageToZoomLevel(int percent);
  void LoadErrorPage(CefRefPtr<CefFrame> frame,
                     const std::string& failed_url,
                     cef_errorcode_t error_code,
                     const std::string& other_info);
  std::string GetErrorString(cef_errorcode_t code);
  std::string GetDataURI(const std::string& data, const std::string& mime_type);

  CJSHandler* m_jsHandler;
  CefRefPtr<CJSDialogHandler> m_jsDialogHandler;
  CefRefPtr<CefMessageRouterBrowserSide> m_messageRouter;
};
