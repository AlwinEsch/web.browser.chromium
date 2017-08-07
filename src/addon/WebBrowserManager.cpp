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

#include <vector>

#include <kodi/General.h>
#include <kodi/gui/dialogs/OK.h>
#include <kodi/gui/dialogs/YesNo.h>
#include <kodi/gui/dialogs/Keyboard.h>
#include <kodi/gui/dialogs/FileBrowser.h>

#include "p8-platform/util/StringUtils.h"
#include "include/cef_app.h"
#include "include/cef_version.h"
#include "include/wrapper/cef_helpers.h"

#include "addon.h"
#include "WebBrowserManager.h"
#include "Utils.h"

using namespace std;







// /*
//
//
// class CWebBrowserRender : public CefApp, public CefRenderProcessHandler
// {
// public:
//   CWebBrowserRender()
//     : m_last_node_is_editable(false)
//   {
//     printf("NEW Render Process\n");
//   }
// //   virtual ~CWebBrowserRender();
//
//
//   virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
//                                   CefRefPtr<CefRequest> request, NavigationType navigation_type,
//                                   bool is_redirect) OVERRIDE;
//
//   virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
//                                 CefRefPtr<CefV8Context> context) OVERRIDE;
//
//   virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
//                                  CefRefPtr<CefV8Context> context) OVERRIDE;
//
//   virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
//                                    CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception,
//                                    CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;
//
//   virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
//                                     CefRefPtr<CefDOMNode> node) OVERRIDE;
//
//   virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
//                                         CefRefPtr<CefProcessMessage> message) OVERRIDE;
//
//   virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { fprintf(stderr, " -aaaaaaaaaaaaaaaaaaaaa- %s\n", __PRETTY_FUNCTION__); return this; }
//
// private:
//   bool m_last_node_is_editable;
//
//   IMPLEMENT_REFCOUNTING(CWebBrowserRender);
//   DISALLOW_COPY_AND_ASSIGN(CWebBrowserRender);
// };
//
// bool CWebBrowserRender::OnBeforeNavigation(
//     CefRefPtr<CefBrowser> browser,
//     CefRefPtr<CefFrame> frame,
//     CefRefPtr<CefRequest> request,
//     NavigationType navigation_type,
//     bool is_redirect)
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return false;
// }
//
// void CWebBrowserRender::OnContextCreated(
//     CefRefPtr<CefBrowser> browser,
//     CefRefPtr<CefFrame> frame,
//     CefRefPtr<CefV8Context> context)
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// void CWebBrowserRender::OnContextReleased(
//     CefRefPtr<CefBrowser> browser,
//     CefRefPtr<CefFrame> frame,
//     CefRefPtr<CefV8Context> context)
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// void CWebBrowserRender::OnUncaughtException(
//     CefRefPtr<CefBrowser> browser,
//     CefRefPtr<CefFrame> frame,
//     CefRefPtr<CefV8Context> context,
//     CefRefPtr<CefV8Exception> exception,
//     CefRefPtr<CefV8StackTrace> stackTrace)
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// void CWebBrowserRender::OnFocusedNodeChanged(
//     CefRefPtr<CefBrowser> browser,
//     CefRefPtr<CefFrame> frame,
//     CefRefPtr<CefDOMNode> node)
// {
//   bool is_editable = (node.get() && node->IsEditable());
//   if (is_editable != m_last_node_is_editable) {
//     // Notify the browser of the change in focused element type.
//     m_last_node_is_editable = is_editable;
//     CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("ClientRenderer.FocusedNodeChanged");
//     message->GetArgumentList()->SetBool(0, is_editable);
//     message->GetArgumentList()->SetString(1, node->GetName());
//     message->GetArgumentList()->SetString(2, node->GetValue());
//     message->GetArgumentList()->SetString(3, node->GetFormControlElementType());
//     message->GetArgumentList()->SetInt(4, node->GetType());
//     browser->SendProcessMessage(PID_BROWSER, message);
//   }
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// bool CWebBrowserRender::OnProcessMessageReceived(
//     CefRefPtr<CefBrowser> browser,
//     CefProcessId source_process,
//     CefRefPtr<CefProcessMessage> message)
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//
//   return false;
// }*/













CWebBrowserApp::CWebBrowserApp()
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

CWebBrowserApp::~CWebBrowserApp()
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

/*!
 * @brief CefApp methods
 */
//{
void CWebBrowserApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnRegisterCustomSchemes(
    CefRawPtr<CefSchemeRegistrar> registrar)
{
//   registrar->AddCustomScheme("te4", true, true, false, false, false, false);
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}
//}

bool CWebBrowserApp::OnBeforeNavigation(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    NavigationType navigation_type,
    bool is_redirect)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
  return false;
}

void CWebBrowserApp::OnContextCreated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnContextReleased(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnUncaughtException(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context,
    CefRefPtr<CefV8Exception> exception,
    CefRefPtr<CefV8StackTrace> stackTrace)
{
fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

void CWebBrowserApp::OnFocusedNodeChanged(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefDOMNode> node)
{
  bool is_editable = (node.get() && node->IsEditable());
  if (is_editable != m_last_node_is_editable) {
    // Notify the browser of the change in focused element type.
    m_last_node_is_editable = is_editable;
    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("ClientRenderer.FocusedNodeChanged");
    message->GetArgumentList()->SetBool(0, is_editable);
    if (is_editable)
    {
      message->GetArgumentList()->SetString(1, node->GetName());
      message->GetArgumentList()->SetString(2, node->GetValue());
      message->GetArgumentList()->SetString(3, node->GetFormControlElementType());
      message->GetArgumentList()->SetInt(4, node->GetType());
    }
    browser->SendProcessMessage(PID_BROWSER, message);
  }
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
}

bool CWebBrowserApp::OnProcessMessageReceived(
    CefRefPtr<CefBrowser> browser,
    CefProcessId source_process,
    CefRefPtr<CefProcessMessage> message)
{
  fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);

  return false;
}















/*!
 * @brief CefBrowserProcessHandler methods
 */
//{
// void CWebBrowserApp::OnContextInitialized()
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//
//   CEF_REQUIRE_UI_THREAD();
//
//   // Register cookieable schemes with the global cookie manager.
//   CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
//   DCHECK(manager.get());
//   manager->SetSupportedSchemes(m_cookieableSchemes, NULL);
//
// }
//}

/*!
 * @brief CefRenderProcessHandler methods
 */
//{
// void CWebBrowserApp::OnRenderThreadCreated(
//     CefRefPtr<CefListValue> extra_info)
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// void CWebBrowserApp::OnWebKitInitialized()
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// void CWebBrowserApp::OnBrowserCreated(
//     CefRefPtr<CefBrowser> browser)
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// void CWebBrowserApp::OnBrowserDestroyed(
//     CefRefPtr<CefBrowser> browser)
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
// }
//
// CefRefPtr<CefLoadHandler> CWebBrowserApp::GetLoadHandler()
// {
// fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return nullptr;
// }


//}


/*!
 * @brief CefResourceBundle methods
 */
//{
// bool CWebBrowserApp::GetLocalizedString(
//     int                                   string_id,
//     CefString&                            string)
// {
//   fprintf(stderr, " -- %s - %i\n", __PRETTY_FUNCTION__, string_id);
//   return false;
// }
//
// bool CWebBrowserApp::GetDataResource(
//     int                                   resource_id,
//     void*&                                data,
//     size_t&                               data_size)
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return false;
// }
//
// bool CWebBrowserApp::GetDataResourceForScale(
//     int                                   resource_id,
//     ScaleFactor                           scale_factor,
//     void*&                                data,
//     size_t&                               data_size)
// {
//   fprintf(stderr, " -- %s\n", __PRETTY_FUNCTION__);
//   return false;
// }
//}



