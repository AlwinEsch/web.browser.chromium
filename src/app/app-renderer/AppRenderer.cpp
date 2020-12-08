/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppRenderer.h"

// Own
#include "../MessageIds.h"
#include "../common/Scheme.h"
#include "v8/V8Handler.h"

// Dev kit
#include "../../../lib/kodi-dev-kit/include/kodi/General.h"

// Global
#include <atomic>

#define KODI_TO_CHROMIUM_LANGUAGE_START 50000

namespace chromium
{
namespace app
{
namespace renderer
{

//TODO Make allowed interface URL's more editable by user (To add own)
std::vector<std::string> CWebAppRenderer::m_allowedInterfaceURLs = {"file://", "https://kodi.tv",
                                                                    "https://forum.kodi.tv/"};

namespace
{
static std::atomic_int m_ctorcount{0}; // For debug purposes and to see destructs done
}

CWebAppRenderer::CWebAppRenderer()
{
  fprintf(stderr, "CWebAppRenderer START (%p) count open %i\n", this, ++m_ctorcount);
}

CWebAppRenderer::~CWebAppRenderer()
{
  fprintf(stderr, "CWebAppRenderer STOP (%p) count open %i\n", this, --m_ctorcount);
}

/// CefApp
//@{
void CWebAppRenderer::OnBeforeCommandLineProcessing(const CefString& process_type,
                                                    CefRefPtr<CefCommandLine> command_line)
{
}

void CWebAppRenderer::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  chromium::app::common::RegisterCustomSchemes(registrar);
}
//@}

/// CefRenderProcessHandler
//@{
void CWebAppRenderer::OnWebKitInitialized()
{
  CefMessageRouterConfig config;
  config.js_query_function = "kodiQuery";
  config.js_cancel_function = "kodiQueryCancel";
  m_messageRouter = CefMessageRouterRendererSide::Create(config);
}

void CWebAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefDictionaryValue> extra_info)
{
  m_securityWebaddonAccess = extra_info->GetInt(SettingValues::security_webaddon_access);
  m_browser = browser;
}

void CWebAppRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
  m_browser = nullptr;
}

CefRefPtr<CefLoadHandler> CWebAppRenderer::GetLoadHandler()
{
  // Load handler seems not needed on render process, used currently browser
  // process only.
  return nullptr;
}

void CWebAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefV8Context> context)
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
  {
    // Becomes strored on init by "CefBrowser"
    CefRefPtr<v8::CV8Handler> handler = new v8::CV8Handler(*this);
    handler->InitKodiAPI(browser, frame, context);

    m_messageRouter->OnContextCreated(browser, frame, context);
  }
}

void CWebAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefV8Context> context)
{
  m_messageRouter->OnContextReleased(browser, frame, context);
}

void CWebAppRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefV8Context> context,
                                          CefRefPtr<CefV8Exception> exception,
                                          CefRefPtr<CefV8StackTrace> stackTrace)
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

  browser->GetMainFrame()->SendProcessMessage(CefProcessId::PID_BROWSER, message);
}

void CWebAppRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                           CefRefPtr<CefFrame> frame,
                                           CefRefPtr<CefDOMNode> node)
{
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

bool CWebAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                               CefRefPtr<CefFrame> frame,
                                               CefProcessId source_process,
                                               CefRefPtr<CefProcessMessage> message)
{
  if (m_messageRouter->OnProcessMessageReceived(browser, frame, source_process, message))
    return true;

  return false;
}
//@}

/// CefResourceBundleHandler
//@{
bool CWebAppRenderer::GetLocalizedString(int string_id, CefString& str)
{
/*
  // Is really a much on render process ;-)
  str = kodi::GetLocalizedString(string_id + KODI_TO_CHROMIUM_LANGUAGE_START);
  if (str.empty())
  {
    kodi::Log(ADDON_LOG_DEBUG,
              "%s: Currently not on addon itself supported Chromium string id %i <------------",
              __func__, string_id);
    return false;
  }
  return true;
*/
  return false;
}

bool CWebAppRenderer::GetDataResource(int resource_id, void*& data, size_t& data_size)
{
  return GetDataResourceForScale(resource_id, SCALE_FACTOR_NONE, data, data_size);
}

bool CWebAppRenderer::GetDataResourceForScale(int resource_id,
                                              ScaleFactor scale_factor,
                                              void*& data,
                                              size_t& data_size)
{
  //TODO is useful?
  return false;
}
//@}

} /* namespace renderer */
} /* namespace app */
} /* namespace chromium */
