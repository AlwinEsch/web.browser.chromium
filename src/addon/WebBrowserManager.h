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

#define NDEBUG 1

#include <map>
#include <kodi/addon-instance/Web.h>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/base/cef_thread_checker.h"
#include "p8-platform/threads/threads.h"

#include "WebBrowserClient.h"



class CWebBrowserApp : public CefApp,
                       /*public CefMessageRouterBrowserSide,*/
                       public CefRenderProcessHandler/*,
                       public CefResourceBundleHandler*/
{
public:
  CWebBrowserApp();
  virtual ~CWebBrowserApp();

  /*!
   * @brief CefApp methods
   *
   * Implement this interface to provide handler implementations. Methods will be
   * called by the process and/or thread indicated.
   */
  //{
  virtual void OnBeforeCommandLineProcessing(                      ///<--
      const CefString& process_type,                                  ///
      CefRefPtr<CefCommandLine> command_line)                         ///
                        OVERRIDE;                                     ///

  virtual void OnRegisterCustomSchemes(                            ///<--
      CefRawPtr<CefSchemeRegistrar> registrar)                        ///
                        OVERRIDE;                                     ///
  //}

  virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefRequest> request, NavigationType navigation_type,
                                  bool is_redirect) OVERRIDE;

  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context) OVERRIDE;

  virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) OVERRIDE;

  virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception,
                                   CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;

  virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefDOMNode> node) OVERRIDE;

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) OVERRIDE;

//   /*!
//    * @brief CefBrowserProcessHandler methods
//    */
//   //{
// /*  virtual void OnContextInitialized()                              ///<--
//                         OVERRIDE; */                                    ///
//   //}
//
// //   /*!
// //    * @brief CefRenderProcessHandler methods
// //    *
// //    * Class used to implement render process callbacks. The methods of this class
// //    * will be called on the render process main thread (TID_RENDERER) unless
// //    * otherwise indicated.
// //    */
// //   //{
// //   virtual void OnRenderThreadCreated(                              ///<--
// //       CefRefPtr<CefListValue> extra_info)                             ///
// //                         OVERRIDE;                                     ///
// //
// //   virtual void OnWebKitInitialized()                               ///<--
// //                         OVERRIDE;                                     ///
// //
// //   virtual void OnBrowserCreated(                                   ///<--
// //       CefRefPtr<CefBrowser> browser)                                  ///
// //                         OVERRIDE;                                     ///
// //
// //   virtual void OnBrowserDestroyed(                                 ///<--
// //       CefRefPtr<CefBrowser> browser)                                  ///
// //                         OVERRIDE;                                     ///
// //
// //   virtual CefRefPtr<CefLoadHandler> GetLoadHandler()               ///<--
// //                         OVERRIDE;                                     ///
// //
// /*  virtual bool OnBeforeNavigation(                                 ///<--
//       CefRefPtr<CefBrowser> browser,                                  ///
//       CefRefPtr<CefFrame> frame,                                      ///
//       CefRefPtr<CefRequest> request,                                  ///
//       NavigationType navigation_type,                                 ///
//       bool is_redirect)                                               ///
//                         OVERRIDE
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return false;
// };  */                                   ///
//
// //   virtual void OnContextCreated(                                   ///<--
// //       CefRefPtr<CefBrowser> browser,                                  ///
// //       CefRefPtr<CefFrame> frame,                                      ///
// //       CefRefPtr<CefV8Context> context)                                ///
// //                         OVERRIDE
// // {
// //   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// // };                                     ///
// //
// //   virtual void OnContextReleased(                                  ///<--
// //       CefRefPtr<CefBrowser> browser,                                  ///
// //       CefRefPtr<CefFrame> frame,                                      ///
// //       CefRefPtr<CefV8Context> context)                                ///
// //                         OVERRIDE
// // {
// //   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// // };                                     ///
// //
// //   virtual void OnUncaughtException(                                ///<--
// //       CefRefPtr<CefBrowser> browser,                                  ///
// //       CefRefPtr<CefFrame> frame,                                      ///
// //       CefRefPtr<CefV8Context> context,                                ///
// //       CefRefPtr<CefV8Exception> exception,                            ///
// //       CefRefPtr<CefV8StackTrace> stackTrace)                          ///
// //                         OVERRIDE
// // {
// //   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// // };                                     ///
//
// /*  virtual void OnFocusedNodeChanged(                               ///<--
//       CefRefPtr<CefBrowser> browser,                                  ///
//       CefRefPtr<CefFrame> frame,                                      ///
//       CefRefPtr<CefDOMNode> node)                                     ///
//                         OVERRIDE
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// };        */                             ///
//
//   virtual bool AddHandler(Handler* handler, bool first)
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return false;
// };
//
//   ///
//   // Remove an existing query handler. Any pending queries associated with the
//   // handler will be canceled. Handler::OnQueryCanceled will be called and the
//   // associated JavaScript onFailure callback will be executed with an error
//   // code of -1. Returns true if the handler is removed successfully or false
//   // if the handler is not found. Must be called on the browser process UI
//   // thread.
//   ///
//   virtual bool RemoveHandler(Handler* handler)
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return false;
// };
//
//   ///
//   // Cancel all pending queries associated with either |browser| or |handler|.
//   // If both |browser| and |handler| are NULL all pending queries will be
//   // canceled. Handler::OnQueryCanceled will be called and the associated
//   // JavaScript onFailure callback will be executed in all cases with an error
//   // code of -1.
//   ///
//   virtual void CancelPending(CefRefPtr<CefBrowser> browser,
//                              Handler* handler)
//   {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// };
//
//   ///
//   // Returns the number of queries currently pending for the specified |browser|
//   // and/or |handler|. Either or both values may be empty. Must be called on the
//   // browser process UI thread.
//   ///
//   virtual int GetPendingCount(CefRefPtr<CefBrowser> browser,
//                               Handler* handler)
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return 0;
// };
//
//   // The below methods should be called from other CEF handlers. They must be
//   // called exactly as documented for the router to function correctly.
//
//   ///
//   // Call from CefLifeSpanHandler::OnBeforeClose. Any pending queries associated
//   // with |browser| will be canceled and Handler::OnQueryCanceled will be
//   // called. No JavaScript callbacks will be executed since this indicates
//   // destruction of the browser.
//   ///
//   virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser)
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// };
//
//   ///
//   // Call from CefRequestHandler::OnRenderProcessTerminated. Any pending queries
//   // associated with |browser| will be canceled and Handler::OnQueryCanceled
//   // will be called. No JavaScript callbacks will be executed since this
//   // indicates destruction of the context.
//   ///
//   virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser)
//   {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// };
//
//   ///
//   // Call from CefRequestHandler::OnBeforeBrowse only if the navigation is
//   // allowed to proceed. If |frame| is the main frame then any pending queries
//   // associated with |browser| will be canceled and Handler::OnQueryCanceled
//   // will be called. No JavaScript callbacks will be executed since this
//   // indicates destruction of the context.
//   ///
//   virtual void OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
//                               CefRefPtr<CefFrame> frame)                           ///
//                         OVERRIDE
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// };                                     ///
//
//
//   virtual bool OnProcessMessageReceived(                           ///<--
//       CefRefPtr<CefBrowser> browser,                                  ///
//       CefProcessId source_process,                                    ///
//       CefRefPtr<CefProcessMessage> message)                           ///
//                         OVERRIDE
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return false;
// };                                     ///
// //   //}

  /*!
   * @brief CefResourceBundleHandler methods
   *
   * Class used to implement a custom resource bundle interface. See CefSettings
   * for additional options related to resource bundle loading. The methods of
   * this class may be called on multiple threads.
   */
  //{
//   virtual bool GetLocalizedString(                                 ///<--
//       int                                   string_id,                ///
//       CefString&                            string)                   ///
//                         OVERRIDE;                                     ///
//
//   virtual bool GetDataResource(                                    ///<--
//       int                                   resource_id,              ///
//       void*&                                data,                     ///
//       size_t&                               data_size)                ///
//                         OVERRIDE;                                     ///
//
//   virtual bool GetDataResourceForScale(                            ///<--
//       int                                   resource_id,              ///
//       ScaleFactor                           scale_factor,             ///
//       void*&                                data,                     ///
//       size_t&                               data_size)                ///
//                         OVERRIDE;                                     ///
  //}

//   virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() OVERRIDE { return this; }
//   virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { fprintf(stderr, " -bbbbbbbbbbbbbbbbbbbbbb- %s\n", __PRETTY_FUNCTION__); return this; }

protected:
  // Schemes that will be registered with the global cookie manager.
  std::vector<CefString>  m_cookieableSchemes;

private:
  bool m_last_node_is_editable;

  IMPLEMENT_REFCOUNTING(CWebBrowserApp);
  DISALLOW_COPY_AND_ASSIGN(CWebBrowserApp);
};






/*



class CWebBrowserManager
{
public:
  CWebBrowserManager(WebBrowserAddon instance);
  virtual ~CWebBrowserManager();

  WEB_ADDON_ERROR CreateControl(const WEB_ADDON_GUI_PROPS &props, const char *webType, ADDON_HANDLE handle);
  bool DestroyControl(const ADDON_HANDLE handle);
  bool SetLanguage(const char *language);
  void Render(const ADDON_HANDLE handle);
  void Stop(const ADDON_HANDLE handle);
  bool Dirty(const ADDON_HANDLE handle);
  bool OnInit(const ADDON_HANDLE handle);
  bool OnAction(const ADDON_HANDLE handle, int actionId, int &nextItem);
  bool OnMouseEvent(const ADDON_HANDLE handle, int id, double x, double y, double offsetX, double offsetY, int state);

  bool OpenWebsite(const ADDON_HANDLE handle, const char* strURL, bool single, bool allowMenus);
  void CallSingleCommand(const ADDON_HANDLE handle, WEB_ADDON_SINGLE_COMMANDS command);

  void *Process(void);

private:
  static int        m_iUniqueClientId;

  CefSettings       m_cefSettings;
  CefRefPtr<CefApp> m_CefApp;

  struct sMainThreadData;
  typedef void (*MainThreadFunction)(struct sMainThreadData *data);
  typedef struct sMainThreadData
  {
    CWebBrowserManager *manager;
    const ADDON_HANDLE *handle;
    MainThreadFunction  function;
    union data_t
    {
      struct
      {
        const WEB_ADDON_GUI_PROPS *props;
        unsigned int webType;
        ADDON_HANDLE *handle;
      } CreateControl;
      struct
      {
        const char* strURL;
        bool single;
        bool allowMenus;
      } OpenWebsite;
      struct
      {
        int actionId;
        int *nextItem;
      } OnAction;
      struct
      {
        WEB_ADDON_SINGLE_COMMANDS command;
      } CallSingleCommand;
      struct
      {
        int mouseId;
        double x;
        double y;
        double offsetX;
        double offsetY;
        int state;
      } OnMouseEvent;
    } data;
    union ret_t
    {
      bool booleanError;
      WEB_ADDON_ERROR addonError;
    } ret;
    P8PLATFORM::CEvent event;
  } sMainThreadData;

  std::queue <sMainThreadData*> m_processQueue;
  P8PLATFORM::CMutex m_processQueueMutex;
  P8PLATFORM::CMutex  m_Mutex;
  std::map<int, CWebBrowserClient*> m_BrowserClients;
  std::map<std::string, CWebBrowserClient*> m_BrowserClientsInactive;

  CefRefPtr<CefBrowser>         m_pBrowser;
  CefRefPtr<CWebBrowserClient>  m_pBrowserClient;
  WebBrowserAddon m_instance;
};*/
