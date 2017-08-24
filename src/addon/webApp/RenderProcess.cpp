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

#include "DOMVisitor.h"
#include "MessageIds.h"
#include "Utils.h"
#include "JSInterface/V8Handler.h"

#include "include/wrapper/cef_message_router.h"

#include <kodi/General.h>

#include "RenderProcess.h"

#define DEBUG_LOGS 1

CRenderProcess::CRenderProcess(const CefSettings& cefSettings)
  : m_cefSettings(cefSettings)
{
  CreateThread();
fprintf(stderr, "-------------------------<> %s\n", __PRETTY_FUNCTION__);
}

CRenderProcess::~CRenderProcess()
{
  CefQuitMessageLoop();
  StopThread();
}

void* CRenderProcess::Process(void)
{
  void* windows_sandbox_info = nullptr;
#if defined(OS_WIN)
  // Manages the life span of the sandbox information object.
  CefScopedSandboxInfo scoped_sandbox;
  windows_sandbox_info = scoped_sandbox.sandbox_info();
#endif
fprintf(stderr, "--1-----------------------<> %s\n", __PRETTY_FUNCTION__);
  CefMainArgs args;
  if (!CefInitialize(args, m_cefSettings, this, windows_sandbox_info))
  {
    LOG_INTERNAL_MESSAGE(ADDON_LOG_ERROR, "%s - Web browser start failed", __FUNCTION__);
    return nullptr;
  }
fprintf(stderr, "---2----------------------<> %s\n", __PRETTY_FUNCTION__);
  CefRunMessageLoop();
fprintf(stderr, "--3-----------------------<> %s\n", __PRETTY_FUNCTION__);
  return nullptr;
}

/// CefRenderProcessHandler
//@{
void CRenderProcess::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "---- %s", __PRETTY_FUNCTION__);
#endif
}

void CRenderProcess::OnWebKitInitialized()
{
  CefMessageRouterConfig config;
  m_messageRouter = CefMessageRouterRendererSide::Create(config);
  CV8Handler::OnWebKitInitialized();
}

void CRenderProcess::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "---- %s", __PRETTY_FUNCTION__);
#endif
}

void CRenderProcess::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "---- %s", __PRETTY_FUNCTION__);
#endif
}

bool CRenderProcess::OnBeforeNavigation(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request,
                                        NavigationType navigation_type, bool is_redirect)
{
#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "---- %s", __PRETTY_FUNCTION__);
#endif
  return false;
}

void CRenderProcess::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  m_messageRouter->OnContextCreated(browser, frame, context);
}

void CRenderProcess::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  m_messageRouter->OnContextReleased(browser, frame, context);
}

void CRenderProcess::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context,
                                         CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{
#ifdef DEBUG_LOGS
  LOG_MESSAGE(ADDON_LOG_DEBUG, "---- %s", __PRETTY_FUNCTION__);
#endif
}

void CRenderProcess::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
  fprintf(stderr, "-------------------------<> %s\n", __PRETTY_FUNCTION__);
  CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(RendererMessage::FocusedNodeChanged);
  if (node.get())
  {
    CefRect bounds = node->GetElementBounds();
    std::string name = node->GetName();
    bool editableForm = node->GetType() == 1 && (name == "SELECT" || name == "INPUT" || name == "TEXTAREA");
    fprintf(stderr, "-------------------------<> %s editableForm %i\n", __PRETTY_FUNCTION__, editableForm);

    message->GetArgumentList()->SetBool(0, editableForm);
    message->GetArgumentList()->SetBool(1, node->IsEditable());
    message->GetArgumentList()->SetInt(2, bounds.x);
    message->GetArgumentList()->SetInt(3, bounds.y);
    message->GetArgumentList()->SetInt(4, bounds.width);
    message->GetArgumentList()->SetInt(5, bounds.height);
    message->GetArgumentList()->SetString(6, node->GetName());
    message->GetArgumentList()->SetString(7, node->GetValue());
  }
  else
  {
    fprintf(stderr, "-------------------------<> %s xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n", __PRETTY_FUNCTION__);

    message->GetArgumentList()->SetBool(0, false);
    message->GetArgumentList()->SetBool(1, false);
    message->GetArgumentList()->SetInt(2, 0);
    message->GetArgumentList()->SetInt(3, 0);
    message->GetArgumentList()->SetInt(4, 0);
    message->GetArgumentList()->SetInt(5, 0);
    message->GetArgumentList()->SetString(6, "");
    message->GetArgumentList()->SetString(7, "");
  }
  browser->SendProcessMessage(PID_BROWSER, message);
}

bool CRenderProcess::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  fprintf(stderr, "-------------------------<> %s\n", __PRETTY_FUNCTION__);
  if (m_messageRouter->OnProcessMessageReceived(browser, source_process, message))
    return true;
fprintf(stderr, "-222222222------------------------<> %s\n", __PRETTY_FUNCTION__);
  std::string message_name = message->GetName();
  if (message_name == AddonClientMessage::FocusedSelected)
  {
    browser->GetMainFrame()->VisitDOM(new CDOMVisitor(browser));
    return true;
  }
  return false;
}
//@}
