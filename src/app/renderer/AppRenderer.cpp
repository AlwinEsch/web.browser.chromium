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

#include "AppRenderer.h"
#include "MessageIds.h"
#include "DOMVisitor.h"
#include "V8Handler.h"

void CWebAppRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
}

void CWebAppRenderer::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  registrar->AddCustomScheme("kodi", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
}

void CWebAppRenderer::OnWebKitInitialized()
{
  CefMessageRouterConfig config;
//   config.js_query_function = "kodiQuery";
//   config.js_cancel_function = "kodiQueryCancel";
  m_messageRouter = CefMessageRouterRendererSide::Create(config);
  CV8Handler::OnWebKitInitialized(this);
}

void CWebAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
  m_browser = browser;
}

void CWebAppRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
  m_browser = nullptr;
}

CefRefPtr<CefLoadHandler> CWebAppRenderer::GetLoadHandler()
{
  return nullptr;
}

void CWebAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  m_messageRouter->OnContextCreated(browser, frame, context);
}

void CWebAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  m_messageRouter->OnContextReleased(browser, frame, context);
}

void CWebAppRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,CefRefPtr<CefV8Context> context,
                                          CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{
}

void CWebAppRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
    bool is_editable = (node.get() && node->IsEditable());
    if (is_editable != m_lastNodeIsEditable)
    {
      // Notify the browser of the change in focused element type.
      m_lastNodeIsEditable = is_editable;
      CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(RendererMessage::FocusedNodeChanged);
      message->GetArgumentList()->SetBool(0, is_editable);
      browser->SendProcessMessage(PID_BROWSER, message);
    }

}

bool CWebAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  if (m_messageRouter->OnProcessMessageReceived(browser, source_process, message))
    return true;

  fprintf(stderr, "---------------------------------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<<\n");
  return false;
}
