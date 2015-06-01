#pragma once
/*
 *      Copyright (C) 2015 Team KODI
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

#include <queue>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "platform/threads/threads.h"

class CWebBrowserClientBase :
    public CefClient,
    public CefDisplayHandler,
    public CefDragHandler,
    public CefGeolocationHandler,
    public CefLoadHandler,
    public CefKeyboardHandler,
    public CefDownloadHandler,
    public CefContextMenuHandler,
    public CefLifeSpanHandler,
    public CefRenderHandler,
    public CefRequestHandler
{
public:
  CWebBrowserClientBase(int iUniqueClientId, const WEB_ADDON_GUI_PROPS *props);
  virtual ~CWebBrowserClientBase();

  /*!
   * @brief return the unique identification id of this control client
   */
  int GetUniqueId() { return m_iUniqueClientId; }

  /*!
   * @brief return the from Kodi given identifier name of this browser client
   * Used to identify opened browser on related GUI control.
   */
  std::string GetIdName() { return m_strIdName; }

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

  bool OnAction(int actionId, int &nextItem);
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
  bool OpenWebsite(const char* strURL, bool single, bool allowMenus);

  /*!
   * @brief Reload current active website
   */
  void ReloadWebsite();

  /*!
   * @brief Used to give client the add-on handle data for callbacks.
   * During usage of them must be on dataAddress the m_pControlIdent
   * inserted.
   * @param addonHandle The handle
   */
  void SetAddonHandle(ADDON_HANDLE addonHandle);

  /*!
   * Kodi add-on render functions
   */
  virtual void Cleanup() = 0;
  virtual void Render() = 0;
  virtual bool Dirty();

  /*!
   * @brief CefClient methods
   *
   * Implement this interface to provide handler implementations.
   */
  //{
  virtual bool OnProcessMessageReceived(                           ///<-- Called when a new message is received from a different process. Return true
      CefRefPtr<CefBrowser>                 browser,                  /// if the message was handled or false otherwise. Do not keep a reference to
      CefProcessId                          source_process,           /// or attempt to access the message outside of this callback.
      CefRefPtr<CefProcessMessage>          message)                  ///
                        OVERRIDE;                                     ///
  //}
  /*!
   * @brief CefContextMenuHandler methods
   *
   * Implement this interface to handle context menu events. The methods of this
   * class will be called on the UI thread.
   */
  //{
  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE { return this; }

  virtual void OnBeforeContextMenu(                                ///<-- Called before a context menu is displayed. |params| provides information
      CefRefPtr<CefBrowser>                 browser,                  /// about the context menu state. |model| initially contains the default
      CefRefPtr<CefFrame>                   frame,                    /// context menu. The |model| can be cleared to show no context menu or
      CefRefPtr<CefContextMenuParams>       params,                   /// modified to show a custom menu. Do not keep references to |params| or
      CefRefPtr<CefMenuModel>               model)                    /// |model| outside of this callback.
                        OVERRIDE;                                     ///

  virtual bool OnContextMenuCommand(                               ///<-- Called to execute a command selected from the context menu. Return true if
      CefRefPtr<CefBrowser>                 browser,                  /// the command was handled or false for the default implementation. See
      CefRefPtr<CefFrame>                   frame,                    /// cef_menu_id_t for the command ids that have default implementations. All
      CefRefPtr<CefContextMenuParams>       params,                   /// user-defined command ids should be between MENU_ID_USER_FIRST and
      int                                   command_id,               /// MENU_ID_USER_LAST. |params| will have the same values as what was passed to
      EventFlags                            event_flags)              /// OnBeforeContextMenu(). Do not keep a reference to |params| outside of this
                        OVERRIDE;                                     /// callback.

  virtual void OnContextMenuDismissed(                             ///<-- Called when the context menu is dismissed irregardless of whether the menu
      CefRefPtr<CefBrowser>                 browser,                  /// was empty or a command was selected.
      CefRefPtr<CefFrame>                   frame)                    ///
                        OVERRIDE;                                     ///
  //}
  /*!
   * @brief CefDisplayHandler methods
   *
   * Implement this interface to handle events related to browser display state.
   * The methods of this class will be called on the UI thread.
   */
  //{
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }

  virtual void OnAddressChange(                                       ///
      CefRefPtr<CefBrowser>                 browser,                  /// Called when a frame's address has changed.
      CefRefPtr<CefFrame>                   frame,                    ///
      const CefString&                      url)                      ///
                        OVERRIDE;                                     ///

  virtual void OnTitleChange(                                         ///
      CefRefPtr<CefBrowser>                 browser,                  /// Called when the page title changes.
      const CefString&                      title)                    ///
                        OVERRIDE;                                     ///

  virtual void OnFaviconURLChange(                                    ///
      CefRefPtr<CefBrowser>                 browser,                  /// Called when the page icon changes.
      const std::vector<CefString>&         icon_urls)                ///
                        OVERRIDE;                                     ///

  virtual bool OnTooltip(                                             /// Called when the browser is about to display a tooltip. |text| contains the text that will be
      CefRefPtr<CefBrowser>                 browser,                  /// displayed in the tooltip. To handle the display of the tooltip yourself return true. Otherwise,
      CefString&                            text)                     /// you can optionally modify |text| and then return false to allow the browser to display the
                        OVERRIDE;                                     /// tooltip. When window rendering is disabled the application is responsible for drawing tooltips
                                                                      /// and the return value is ignored.
  virtual void OnStatusMessage(
      CefRefPtr<CefBrowser>                 browser,                  /// Called when the browser receives a status message. |value| contains the
      const CefString&                      value)                    /// text that will be displayed in the status message.
                        OVERRIDE;                                     ///

  virtual bool OnConsoleMessage(                                      ///
      CefRefPtr<CefBrowser>                 browser,                  /// Called to display a console message. Return true to stop the message from
      const CefString&                      message,                  /// being output to the console.
      const CefString&                      source,                   ///
      int                                   line)                     ///
                        OVERRIDE;                                     ///
  //}
  /*!
   * @brief CefDownloadHandler methods
   *
   * Class used to handle file downloads. The methods of this class will called
   * on the browser process UI thread.
   */
  //{
  virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE { return this; }

  virtual void OnBeforeDownload(                                      /// Called before a download begins. |suggested_name| is the suggested name for
      CefRefPtr<CefBrowser>                 browser,                  /// the download file. By default the download will be canceled. Execute
      CefRefPtr<CefDownloadItem>            download_item,            /// |callback| either asynchronously or in this method to continue the download
      const CefString&                      suggested_name,           /// if desired. Do not keep a reference to |download_item| outside of this
      CefRefPtr<CefBeforeDownloadCallback>  callback)                 /// method.
                        OVERRIDE;                                     ///

  virtual void OnDownloadUpdated(                                     /// Called when a download's status or progress information has been updated.
      CefRefPtr<CefBrowser>                 browser,                  /// This may be called multiple times before and after OnBeforeDownload().
      CefRefPtr<CefDownloadItem>            download_item,            /// Execute |callback| either asynchronously or in this method to cancel the
      CefRefPtr<CefDownloadItemCallback>    callback)                 /// download if desired. Do not keep a reference to |download_item| outside of
                        OVERRIDE;                                     /// this method.
  //}
  /*!
   * @brief CefDragHandler methods
   *
   * Implement this interface to handle events related to dragging. The methods of
   * this class will be called on the UI thread.
   */
  //{
  virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE { return this; }

  virtual bool OnDragEnter(                                           /// Called when an external drag event enters the browser window. |dragData|
      CefRefPtr<CefBrowser>                 browser,                  /// contains the drag event data and |mask| represents the type of drag
      CefRefPtr<CefDragData>                dragData,                 /// operation. Return false for default drag handling behavior or true to
      CefRenderHandler::DragOperationsMask  mask)                     /// cancel the drag event.
                        OVERRIDE;                                     ///
  //}
  /*!
   * @brief CefGeolocationHandler methods
   *
   * Implement this interface to handle events related to geolocation permission
   * requests. The methods of this class will be called on the browser process UI
   * thread.
   */
  //{
  CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE { return this; }

  virtual bool OnRequestGeolocationPermission(                        /// Called when a page requests permission to access geolocation information.
      CefRefPtr<CefBrowser>                 browser,                  /// |requesting_url| is the URL requesting permission and |request_id| is the
      const CefString&                      requesting_url,           /// unique ID for the permission request. Return true and call
      int                                   request_id,               /// CefGeolocationCallback::Continue() either in this method or at a later
      CefRefPtr<CefGeolocationCallback>     callback)                 /// time to continue or cancel the request. Return false to cancel the request
                        OVERRIDE;                                     /// immediately.

  virtual void OnCancelGeolocationPermission(                         /// Called when a geolocation access request is canceled. |requesting_url| is
      CefRefPtr<CefBrowser>                 browser,                  /// the URL that originally requested permission and |request_id| is the unique
      const CefString&                      requesting_url,           /// ID for the permission request.
      int                                   request_id)               ///
                        OVERRIDE;                                     ///
  //}
  /*!
   * @brief CefKeyboardHandler methods
   *
   * Implement this interface to handle events related to keyboard input. The
   * methods of this class will be called on the UI thread.
   */
  //{
  virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE { return this; }

  virtual bool OnPreKeyEvent(                                         /// Called before a keyboard event is sent to the renderer. |event| contains
      CefRefPtr<CefBrowser>                 browser,                  /// information about the keyboard event. |os_event| is the operating system
      const CefKeyEvent&                    event,                    /// event message, if any. Return true if the event was handled or false
      CefEventHandle                        os_event,                 /// otherwise. If the event will be handled in OnKeyEvent() as a keyboard
      bool* is_keyboard_shortcut)                                     /// shortcut set |is_keyboard_shortcut| to true and return false.
                        OVERRIDE;                                     ///

  virtual bool OnKeyEvent(                                            /// Called after the renderer and JavaScript in the page has had a chance to
      CefRefPtr<CefBrowser>                 browser,                  /// handle the event. |event| contains information about the keyboard event.
      const CefKeyEvent&                    event,                    /// |os_event| is the operating system event message, if any. Return true if
      CefEventHandle                        os_event)                 /// the keyboard event was handled or false otherwise.
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
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }

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

  virtual bool RunModal(                                              /// Called when a modal window is about to display and the modal loop should
      CefRefPtr<CefBrowser>                 browser)                  /// begin running. Return false to use the default modal loop implementation or
                        OVERRIDE;                                     /// true to use a custom implementation.

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
   * @brief CefLoadHandler methods
   *
   * Implement this interface to handle events related to browser load status. The
   * methods of this class will be called on the browser process UI thread or
   * render process main thread (TID_RENDERER).
   */
  //{
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

  virtual void OnLoadingStateChange(                                  /// Called when the loading state has changed. This callback will be executed
      CefRefPtr<CefBrowser>                 browser,                  /// twice -- once when loading is initiated either programmatically or by user
      bool                                  isLoading,                /// action, and once when loading is terminated due to completion, cancellation
      bool                                  canGoBack,                /// of failure.
      bool                                  canGoForward)             ///
                        OVERRIDE;                                     ///

  virtual void OnLoadStart(                                           /// Called when the browser begins loading a frame. The |frame| value will never be empty -- call
      CefRefPtr<CefBrowser>                 browser,                  /// the IsMain() method to check if this frame is the main frame. Multiple frames may be loading
      CefRefPtr<CefFrame>                   frame)                    /// at the same time. Sub-frames may start or continue loading after the main frame load has ended.
                        OVERRIDE;                                     /// This method may not be called for a particular frame if the load request for that frame fails.
                                                                      /// For notification of overall browser load status use OnLoadingStateChange instead.
  virtual void OnLoadEnd(
      CefRefPtr<CefBrowser>                 browser,                  /// Called when the browser is done loading a frame. The |frame| value will never be empty -- call
      CefRefPtr<CefFrame>                   frame,                    /// the IsMain() method to check if this frame is the main frame. Multiple frames may be loading at
      int                                   httpStatusCode)           /// the same time. Sub-frames may start or continue loading after the main frame load has ended.
                        OVERRIDE;                                     /// Thismethod will always be called for all frames irrespective of whether the request completes
                                                                      /// successfully.
  virtual void OnLoadError(
      CefRefPtr<CefBrowser>                 browser,                  /// Called when the resource load for a navigation fails or is canceled.
      CefRefPtr<CefFrame>                   frame,                    /// |errorCode| is the error code number, |errorText| is the error text and
      ErrorCode                             errorCode,                /// |failedUrl| is the URL that failed to load. See net\base\net_error_list.h
      const CefString&                      errorText,                /// for complete descriptions of the error codes.
      const CefString&                      failedUrl)                ///
                        OVERRIDE;                                     ///
  //}
  /*!
   * @brief CefRenderHandler methods
   *
   * Implement this interface to handle events when window rendering is disabled.
   * The methods of this class will be called on the UI thread.
   */
  //{
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; }

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
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this; }
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

  virtual bool OnBeforePluginLoad(                                 ///<-- Called on the browser process IO thread before a plugin is loaded. Return
      CefRefPtr<CefBrowser>                 browser,                  /// true to block loading of the plugin.
      const CefString&                      url,                      ///
      const CefString&                      policy_url,               ///
      CefRefPtr<CefWebPluginInfo>           info)                     ///
                        OVERRIDE;                                     ///

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
  IMPLEMENT_REFCOUNTING(CWebBrowserClientBase);

protected:
  /*!
   * Given Kodi's settings data
   */
  void        *m_pDevice;
  int          m_iLeaveOpenTime;
  int          m_iXPos;
  int          m_iYPos;
  int          m_iWidth;
  int          m_iHeight;
  float        m_fPixelRatio;
  std::string  m_strIdName;
  int          m_iGUIItemLeft;
  int          m_iGUIItemRight;
  int          m_iGUIItemTop;
  int          m_iGUIItemBottom;
  int          m_iGUIItemBack;
  bool         m_bTransparentBackground;
  void        *m_pControlIdent;

  float        m_BackgroundColor[4];

private:
  const int    m_iUniqueClientId;  /*!< Unique identification id of this control client */
  time_t       m_inactivateTime;      /*!< Time where client becomes set inactive to handle the timeout */
  CefRefPtr<CefBrowser> m_Browser;
  CefRefPtr<CefMessageRouterBrowserSide> m_pMessageRouter;
  ADDON_HANDLE_STRUCT m_addonHandle;

  void NotifyAddress(const CefString& url);

  #define TMSG_SET_OPENED_ADDRESS       100
  #define TMSG_SET_OPENED_TITLE         101
  #define TMSG_SET_ICON_URL             102

  typedef struct
  {
    unsigned int dwMessage;
    int param1;
    int param2;
    std::string strParam;
    std::vector<std::string> params;
    std::shared_ptr<PLATFORM::CEvent> waitEvent;
    void* lpVoid;
  } Message;

  struct MessageCallback
  {
    void (*callback)(void *userptr);
    void *userptr;
  };

  void SendMessage(Message& message, bool wait);
  void HandleMessages();

  std::queue <Message*> m_processQueue;
};
