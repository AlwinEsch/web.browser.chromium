/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
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
