/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppRenderer.h"
#include "DOMVisitor.h"
#include "V8Handler.h"

#include "MessageIds.h"

//TODO Make allowed interface URL's more editable by user (To add own)
std::vector<std::string> CWebAppRenderer::m_allowedInterfaceURLs =
{
  "file://",
  "https://kodi.tv",
  "https://forum.kodi.tv/"
};

void CWebAppRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
  CefRefPtr<CefDictionaryValue> addon_settings = extra_info->GetDictionary(0);
  m_securityWebaddonAccess = addon_settings->GetInt(SettingValues::security_webaddon_access);
}

void CWebAppRenderer::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  registrar->AddCustomScheme("kodi", CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_CORS_ENABLED);
}

void CWebAppRenderer::OnWebKitInitialized()
{
  InitWebToKodiInterface();
}

void CWebAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDictionaryValue> extra_info)
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
    m_messageRouter->OnContextCreated(browser, frame, context);
}

void CWebAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  if (m_interfaceAllowed)
    m_messageRouter->OnContextReleased(browser, frame, context);
}

void CWebAppRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context,
                                          CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{
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

  frame->SendProcessMessage(CefProcessId::PID_BROWSER, message);
}

void CWebAppRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
  bool is_editable = (node.get() && node->IsEditable());
  if (is_editable != m_lastNodeIsEditable)
  {
    // Notify the browser of the change in focused element type.
    m_lastNodeIsEditable = is_editable;
    auto message = CefProcessMessage::Create(RendererMessage::FocusedNodeChanged);
    message->GetArgumentList()->SetBool(0, is_editable);
    frame->SendProcessMessage(CefProcessId::PID_BROWSER, message);
  }

}

bool CWebAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                               CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  if (m_messageRouter->OnProcessMessageReceived(browser, frame, source_process, message))
    return true;

  return false;
}

void CWebAppRenderer::InitWebToKodiInterface()
{
  CefMessageRouterConfig config;
//   config.js_query_function = "kodiQuery";
//   config.js_cancel_function = "kodiQueryCancel";
  m_messageRouter = CefMessageRouterRendererSide::Create(config);
  CV8Handler::OnWebKitInitialized(this);
}
