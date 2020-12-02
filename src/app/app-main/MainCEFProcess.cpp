/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "MainCEFProcess.h"

#include "RequestContextHandler.h"
#include "WebBrowserClient.h"
#include "renderer/OffscreenBasePosix.h"
#include "scheme/SchemeFactory.h"
#include "../MessageIds.h"

// Dev-kit
#include "../../../lib/kodi-dev-kit/include/kodi/General.h"

// CEF
#include "include/cef_app.h"
#include "include/cef_version.h"
#include "include/cef_web_plugin.h"

// Global
#include <kodi/tools/StringUtils.h>

using kodi::tools::StringUtils;
using namespace chromium::app::main;

std::atomic_int CMainCEFProcess::m_iUniqueClientId{0};

class CRegisterCdmCallback : public CefRegisterCdmCallback
{
public:
  CRegisterCdmCallback() = default;
  ~CRegisterCdmCallback() override = default;

  void OnCdmRegistrationComplete(cef_cdm_registration_error_t result,
                                 const CefString& error_message) override
  {
    if (result == CEF_CDM_REGISTRATION_ERROR_NONE)
      kodi::Log(ADDON_LOG_INFO, "Chromium widevine registration successfully");
    else
      kodi::Log(ADDON_LOG_ERROR, "Chromium widevine registration failed with '%s'",
                error_message.ToString().c_str());
  }

private:
  IMPLEMENT_REFCOUNTING(CRegisterCdmCallback);
  DISALLOW_COPY_AND_ASSIGN(CRegisterCdmCallback);
};

CMainCEFProcess::CMainCEFProcess(uint64_t base_handle, const std::string& identifier)
  : kodi::addon::CInstanceWeb(base_handle),
    m_identifier(identifier),
    m_guiManager(new gui::CBrowserGUIManager(*this)),
    m_offscreenGUIBase(std::make_shared<renderer::COffsceenBasePosix>(*this)),
    m_audioHandler(new audio::CAudioHandler(*this, IsMuted()))
{
}

CMainCEFProcess::~CMainCEFProcess()
{
}

bool CMainCEFProcess::Initialize(const CefMainArgs& args,
                                 const CefSettings& settings,
                                 CefRefPtr<CefApp> app,
                                 void* windowsSandboxInfo)
{
  if (!m_cdmPath.empty())
  {
    // Register the Widevine CDM at the specified path. See comments in
    // cef_web_plugin.h for details. It's safe to call this method before
    // CefInitialize(), and calling it before CefInitialize() is required on
    // Linux.
    CefRegisterWidevineCdm(m_cdmPath, new CRegisterCdmCallback);
  }

  if (!CefInitialize(args, settings, app, windowsSandboxInfo))
    return false;

  // Register scheme handlers.
  scheme::RegisterSchemeHandlers();

  m_offscreenGUIBase->Create();
  m_guiManager->Create();

  m_started = true;
  StartDone();

  kodi::Log(ADDON_LOG_DEBUG, "CMainCEFProcess::%s: Started web browser add-on process", __func__);

  return true;
}

void CMainCEFProcess::Shutdown()
{
  m_started = false;

  scheme::ClearSchemeHandlers();

  m_browserClients.clear();
  m_browserClientsInactive.clear();
  m_browserClientsInDelete.clear();

  m_guiManager->Destroy();
  m_guiManager.reset();
  m_offscreenGUIBase->Destroy();
  m_offscreenGUIBase.reset();
  m_audioHandler = nullptr;

//   CefDoMessageLoopWork();
//
//   CefShutdown();
}

void CMainCEFProcess::SetSettings(CefSettings& settings)
{
  std::string language = kodi::GetLanguage(LANG_FMT_ISO_639_1, true);

  settings.no_sandbox = false;
  CefString(&settings.browser_subprocess_path) = m_browserSubprocessPath;
  CefString(&settings.framework_dir_path) = m_frameworkDirPath;
  CefString(&settings.resources_dir_path) = m_resourcesPath;
  CefString(&settings.locales_dir_path) = m_localesPath;
  settings.multi_threaded_message_loop = false;
  settings.external_message_pump = false;
  settings.windowless_rendering_enabled = true;
  settings.command_line_args_disabled = false;
  CefString(&settings.cache_path) = kodi::GetBaseUserPath("pchHTMLCache");
  CefString(&settings.user_data_path) = kodi::GetBaseUserPath();
  settings.persist_session_cookies = false;
  settings.persist_user_preferences = false;
  CefString(&settings.product_version) =
      StringUtils::Format("Kodi/%s Chrome/%d.%d.%d.%d", KODICHROMIUM_VERSION, CHROME_VERSION_MAJOR,
                          CHROME_VERSION_MINOR, CHROME_VERSION_BUILD, CHROME_VERSION_PATCH);
  CefString(&settings.locale) = language;
  CefString(&settings.log_file) = "";
  settings.log_severity =
      static_cast<cef_log_severity_t>(kodi::GetSettingInt("system.loglevelcef"));
  CefString(&settings.javascript_flags) = kodi::GetTempAddonPath("chromium.log");
  settings.pack_loading_disabled = false;
  settings.remote_debugging_port = 8457;
  settings.uncaught_exception_stack_size =
      kodi::GetSettingInt("system.uncaught_exception_stack_size");
  settings.ignore_certificate_errors = false;
  settings.background_color = 0;
  CefString(&settings.accept_language_list) = language;
}

bool CMainCEFProcess::ParseCmdLine(CefRefPtr<CefCommandLine> commandLine)
{
  if (!commandLine->HasSwitch("libpath") || !commandLine->HasSwitch("sharepath") ||
      !commandLine->HasSwitch("basehandle"))
  {
    kodi::Log(ADDON_LOG_ERROR, "CMainCEFProcess::%s: Required values not given in command line",
              __func__);
    return false;
  }

  const std::string& libpath = commandLine->GetSwitchValue("libpath");
  const std::string& sharepath = commandLine->GetSwitchValue("sharepath");
  m_mainshared = commandLine->GetSwitchValue("main-shared");

  m_cdmPath = commandLine->GetSwitchValue("widevine-cdm-path");
  m_baseInstanceId = std::stoul(commandLine->GetSwitchValue("basehandle").ToString(), nullptr, 16);
  m_browserSubprocessPath = libpath + "/kodichromium";
  m_frameworkDirPath = libpath;
  m_resourcesPath = sharepath + "/resources/";
  m_localesPath = sharepath + "/resources/locales/";

  return true;
}

WEB_ADDON_ERROR CMainCEFProcess::StartInstance()
{
  fprintf(stderr, "CHILD--------- %s\n", __PRETTY_FUNCTION__);
  return WEB_ADDON_ERROR_NO_ERROR;
}

void CMainCEFProcess::StopInstance()
{
  fprintf(stderr, "CHILD--------- %s\n", __PRETTY_FUNCTION__);
}

bool CMainCEFProcess::MainInitialize()
{
  fprintf(stderr, "CHILD--------- %s\n", __PRETTY_FUNCTION__);

  StartDone();
  return true;
}

void CMainCEFProcess::MainShutdown()
{
  fprintf(stderr, "CHILD--------- %s\n", __PRETTY_FUNCTION__);
}

void CMainCEFProcess::MainLoop()
{
  if (!m_started)
    return;

//   fprintf(stderr, "CHILD--------- %s\n", __PRETTY_FUNCTION__);

  // Do CEF's message loop work
  PerformMessageLoopWork();
}

bool CMainCEFProcess::PerformMessageLoopWork()
{
  if (m_isActive)
  {
    // When CefDoMessageLoopWork() is called there may be various callbacks
    // (such as paint and IPC messages) that result in additional calls to this
    // method. If re-entrancy is detected we must repost a request again to the
    // owner thread to ensure that the discarded call is executed in the future.
    m_reentrancyDetected = true;
    return false;
  }

  m_reentrancyDetected = false;

  m_isActive = true;
  CefDoMessageLoopWork();
  m_isActive = false;

  // |reentrancy_detected_| may have changed due to re-entrant calls to this
  // method.
  return m_reentrancyDetected;
}

void CMainCEFProcess::SetMute(bool mute)
{
  fprintf(stderr, "CHILD--------- %s\n", __PRETTY_FUNCTION__);
}

bool CMainCEFProcess::SetLanguage(const std::string& language)
{
  fprintf(stderr, "CHILD--------- %s\n", __PRETTY_FUNCTION__);
  return false;
}

kodi::addon::CWebControl* CMainCEFProcess::CreateControl(const WEB_ADDON_GUI_PROPS& properties,
                                                         const std::string& sourceName,
                                                         const std::string& startURL,
                                                         uint64_t handle)
{
  kodi::Log(ADDON_LOG_DEBUG, "CWebBrowser::%s: Web browser control creation started", __func__);

  CefRefPtr<CWebBrowserClient> browserClient;

  auto itr = m_browserClientsInactive.find(sourceName);
  if (itr != m_browserClientsInactive.end())
  {
    kodi::Log(ADDON_LOG_INFO, "CWebBrowser::%s: Found control in inactive mode and setting active",
              __func__);
    browserClient = itr->second;
    browserClient->SetActive();
    m_browserClientsInactive.erase(itr);
  }
  else
  {
    for (const auto& entry : m_browserClients)
    {
      if (entry.second->GetName() == sourceName)
      {
        return entry.second.get();
      }
    }

    CefRefPtr<CRequestContextHandler> contextHandler = new CRequestContextHandler;
    browserClient =
      new CWebBrowserClient(properties, handle, m_iUniqueClientId++, startURL, *this, contextHandler);
    contextHandler->Init(browserClient);

    CefWindowInfo info;
    info.SetAsWindowless(kNullWindowHandle);
#ifdef WIN32
    info.shared_texture_enabled = true;
    info.external_begin_frame_enabled = false;
#endif // WIN32

    CefBrowserSettings settings;
    //TODO Check CefBrowserHost::SetWindowlessFrameRate(...) usable for streams?
    settings.windowless_frame_rate = static_cast<int>(browserClient->GetFPS());
    CefString(&settings.standard_font_family) = "";
    CefString(&settings.fixed_font_family) = "";
    CefString(&settings.serif_font_family) = "";
    CefString(&settings.sans_serif_font_family) = "";
    CefString(&settings.cursive_font_family) = "";
    CefString(&settings.fantasy_font_family) = "";
    settings.default_font_size = 0;
    settings.default_fixed_font_size = 0;
    settings.minimum_font_size = 0;
    settings.minimum_logical_font_size = 0;
    CefString(&settings.default_encoding) = ""; // "ISO-8859-1" if empty
    settings.remote_fonts = STATE_DEFAULT;
    settings.javascript = STATE_ENABLED;
    settings.javascript_close_windows = STATE_DEFAULT;
    settings.javascript_access_clipboard = STATE_DEFAULT;
    settings.javascript_dom_paste = STATE_DEFAULT;
    settings.plugins = STATE_ENABLED;
    settings.universal_access_from_file_urls = STATE_DEFAULT;
    settings.file_access_from_file_urls = STATE_DEFAULT;
    settings.web_security = STATE_DEFAULT;
    settings.image_loading = STATE_DEFAULT;
    settings.image_shrink_standalone_to_fit = STATE_DEFAULT;
    settings.text_area_resize = STATE_DEFAULT;
    settings.tab_to_links = STATE_DEFAULT;
    settings.local_storage = STATE_DEFAULT;
    settings.databases = STATE_DEFAULT;
    settings.application_cache = STATE_DEFAULT;
    settings.webgl = STATE_ENABLED;
    settings.background_color = 0x00; // fully transparent
    CefString(&settings.accept_language_list) = "";

    CefRefPtr<CefDictionaryValue> extra_info = CefDictionaryValue::Create();
    extra_info->SetInt(SettingValues::security_webaddon_access,
                       kodi::GetSettingInt("security.webaddon.access"));
    CefRefPtr<CefRequestContext> request_context =
        CefRequestContext::CreateContext(CefRequestContext::GetGlobalContext(), contextHandler);
    if (!CefBrowserHost::CreateBrowser(info, browserClient, "", settings, extra_info,
                                       request_context))
    {
      kodi::Log(ADDON_LOG_ERROR, "CWebBrowser::%s: Web browser creation failed", __func__);
      if (browserClient)
      {
        contextHandler->Clear();
        browserClient = nullptr;
      }
      return nullptr;
    }
  }

  int uniqueId = browserClient->GetDataIdentifier();
  m_browserClients.emplace(uniqueId, browserClient);

  kodi::Log(ADDON_LOG_DEBUG, "CWebBrowser::%s: Web browser control created", __func__);
  return browserClient.get();
}

bool CMainCEFProcess::DestroyControl(kodi::addon::CWebControl* control, bool complete)
{
  //! Check for wrongly passed empty handle.
  CWebBrowserClient* browserClient = static_cast<CWebBrowserClient*>(control);
  if (browserClient == nullptr)
  {
    kodi::Log(ADDON_LOG_ERROR,
              "CWebBrowser::%s: Web browser control destroy called without handle!", __func__);
    return false;
  }

  browserClient->SetInactive();

  if (complete)
  {
    kodi::Log(ADDON_LOG_DEBUG, "CWebBrowser::%s: Web browser control destroy complete", __func__);
    const auto& inactiveClient = m_browserClientsInactive.find(browserClient->GetName());
    if (inactiveClient != m_browserClientsInactive.end())
      m_browserClientsInactive.erase(inactiveClient);
    const auto& activeClient = m_browserClients.find(browserClient->GetDataIdentifier());
    if (activeClient != m_browserClients.end())
      m_browserClients.erase(activeClient);

    m_browserClientsInDelete.insert(browserClient->GetDataIdentifier());
    browserClient->CloseComplete();
  }
  else
  {
    kodi::Log(ADDON_LOG_DEBUG, "CWebBrowser::%s: Web browser control destroy to set inactive",
              __func__);
    const auto& itr = m_browserClients.find(browserClient->GetDataIdentifier());
    if (itr == m_browserClients.end())
    {
      kodi::Log(ADDON_LOG_ERROR,
                "CWebBrowser::%s: Web browser control destroy called for "
                "invalid id '%i'",
                __func__, browserClient->GetDataIdentifier());
      return false;
    }

    m_browserClientsInactive[browserClient->GetName()] = itr->second;
    m_browserClients.erase(itr);
  }

  kodi::Log(ADDON_LOG_DEBUG, "CWebBrowser::%s: Web browser control destroy done", __func__);
  return true;
}

void CMainCEFProcess::InformDestroyed(int uniqueClientId)
{
  m_browserClientsInDelete.erase(uniqueClientId);
}
