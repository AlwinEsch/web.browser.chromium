/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppRenderer.h"
#include "DOMVisitor.h"
#include "V8Handler.h"

#include "../MessageIds.h"
#include "../common/Scheme.h"

#include "../../../lib/kodi-dev-kit/include/kodi/General.h"



class CLauncherAccessor : public CefV8Accessor
{
public:
  CLauncherAccessor()
  {
    fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  }

  ~CLauncherAccessor()
  {
    fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  }

  bool Get(
    const CefString& name,
    const CefRefPtr<CefV8Value> object,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception) override
  {

    fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
    return false;
  }

  bool Set(
    const CefString& name,
    const CefRefPtr<CefV8Value> object,
    const CefRefPtr<CefV8Value> value,
    CefString& exception) override
  {

    fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
    return false;
  }

private:
  IMPLEMENT_REFCOUNTING(CLauncherAccessor);
};
























//TODO Make allowed interface URL's more editable by user (To add own)
std::vector<std::string> CWebAppRenderer::m_allowedInterfaceURLs =
{
  "file://",
  "https://kodi.tv",
  "https://forum.kodi.tv/"
};

CWebAppRenderer::CWebAppRenderer()
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
}

CWebAppRenderer::~CWebAppRenderer()
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
}

// void CWebAppRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
// {
//   CefRefPtr<CefDictionaryValue> addon_settings = extra_info->GetDictionary(0);
//   m_securityWebaddonAccess = addon_settings->GetInt(SettingValues::security_webaddon_access);
// }

void CWebAppRenderer::OnBeforeCommandLineProcessing(const CefString& process_type,
                                     CefRefPtr<CefCommandLine> command_line)
{
  m_mainShared = command_line->GetSwitchValue("main-shared");
  fprintf(stderr, "---------> %s %s\n", __func__, m_mainShared.c_str());

}

void CWebAppRenderer::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  chromium::app::common::RegisterCustomSchemes(registrar);
}

void CWebAppRenderer::OnWebKitInitialized()
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  CefMessageRouterConfig config;
//   config.js_query_function = "kodiQuery";
//   config.js_cancel_function = "kodiQueryCancel";
  m_messageRouter = CefMessageRouterRendererSide::Create(config);
}

void CWebAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info)
{
  m_securityWebaddonAccess = extra_info->GetInt(SettingValues::security_webaddon_access);
  m_browser = browser;

  fprintf(stderr, "---------> %p %s %i\n", this, __PRETTY_FUNCTION__, m_securityWebaddonAccess);
//   if (!m_v8Inited)
//   {
//     fprintf(stderr, "-xxxxxxxxxx--------> %s\n", __func__);
//     CV8Handler::OnWebKitInitialized(this);
//     m_v8Inited = true;
//   }
}

void CWebAppRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  m_browser = nullptr;
}

CefRefPtr<CefLoadHandler> CWebAppRenderer::GetLoadHandler()
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  return nullptr;
}

void CWebAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  std::string url = frame->GetURL().ToString();
  m_interfaceAllowed = false;

  // Check site is allowed to access Kodi
  switch (m_securityWebaddonAccess)
  {
    case SettingValues::webaddonAccess_LocalAndKnown:
    {
      for (const auto& allowedURL : m_allowedInterfaceURLs)
      {
        if (url.compare(0, allowedURL.size(), allowedURL) == 0)
        {
          m_interfaceAllowed = true;
          break;
        }
      }
    }
    break;

    case SettingValues::webaddonAccess_LocalOnly:
    {
      std::string allowedURL = "file://";
      if (url.compare(0, allowedURL.size(), allowedURL) == 0)
      {
        m_interfaceAllowed = true;
      }
    }
    break;

    case SettingValues::webaddonAccess_Everyone:
      m_interfaceAllowed = true;
    break;

    case SettingValues::webaddonAccess_Off:
    default:
      m_interfaceAllowed = false;
      break;
  }

  if (m_interfaceAllowed)
  {
    CefRefPtr<CV8Handler> handler = new CV8Handler(this);
    handler->InitKodiAPI(browser, frame, context);

    m_messageRouter->OnContextCreated(browser, frame, context);
  }
}

void CWebAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
//   if (m_interfaceAllowed)
    m_messageRouter->OnContextReleased(browser, frame, context);
}

void CWebAppRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context,
                                          CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  auto message = CefProcessMessage::Create(RendererMessage::OnUncaughtException);
  auto list = message->GetArgumentList();

  // Needed in the browser process to get the frame.
  int64 value = frame->GetIdentifier();
  auto binaryValue = CefBinaryValue::Create(&value, sizeof(value));
  list->SetBinary(0, binaryValue);
  list->SetString(1, exception->GetMessage());
  list->SetString(2, exception->GetSourceLine());
  list->SetString(3, exception->GetScriptResourceName());

  auto frames = CefListValue::Create();
  for (auto i = 0; i < stackTrace->GetFrameCount(); i++)
  {
    auto frameValues = CefListValue::Create();
    auto frameArg = stackTrace->GetFrame(i);

    frameValues->SetString(0, frameArg->GetFunctionName());
    frameValues->SetInt(1, frameArg->GetLineNumber());
    frameValues->SetInt(2, frameArg->GetColumn());
    frameValues->SetString(3, frameArg->GetScriptNameOrSourceURL());

    frames->SetList(i, frameValues);
  }

  list->SetList(4, frames);

  browser->GetMainFrame()->SendProcessMessage(CefProcessId::PID_BROWSER, message);
}

void CWebAppRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  bool is_editable = (node.get() && node->IsEditable());
  if (is_editable != m_lastNodeIsEditable)
  {
    // Notify the browser of the change in focused element type.
    m_lastNodeIsEditable = is_editable;
    auto message = CefProcessMessage::Create(RendererMessage::FocusedNodeChanged);
    message->GetArgumentList()->SetBool(0, is_editable);
    browser->GetMainFrame()->SendProcessMessage(CefProcessId::PID_BROWSER, message);
  }

}

bool CWebAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
  if (m_messageRouter->OnProcessMessageReceived(browser, frame, source_process, message))
    return true;

  return false;
}

void CWebAppRenderer::InitWebToKodiInterface()
{
  fprintf(stderr, "---------> %p %s\n", this, __PRETTY_FUNCTION__);
//   CefMessageRouterConfig config;
//   config.js_query_function = "kodiQuery";
//   config.js_cancel_function = "kodiQueryCancel";
//   m_messageRouter = CefMessageRouterRendererSide::Create(config);
}
