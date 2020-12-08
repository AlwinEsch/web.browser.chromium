/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppBrowser.h"

// Own
#include "../common/Scheme.h"
#include "../utils/Info.h"
#include "MainCEFProcess.h"
#include "powerpolicy/PowerPolicyController.h"
#include "print/PrintHandler.h"

// Dev kit
#include "../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../lib/kodi-dev-kit/include/kodi/Processor.h"
#include "../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

using kodi::tools::StringUtils;

#define KODI_TO_CHROMIUM_LANGUAGE_START 50000

namespace chromium
{
namespace app
{
namespace main
{

CClientAppBrowser::CClientAppBrowser(std::shared_ptr<CMainCEFProcess> addonMain)
  : m_addonMain(addonMain)
{
  m_powerPolicyController = new powerpolicy::CPowerPolicyController(addonMain);
}

CClientAppBrowser::~CClientAppBrowser()
{
  m_powerPolicyController = nullptr;
}

/// CefResourceBundleHandler
//@{
bool CClientAppBrowser::GetLocalizedString(int string_id, CefString& str)
{
  str = kodi::GetLocalizedString(string_id + KODI_TO_CHROMIUM_LANGUAGE_START);
  if (str.empty())
  {
    if (!utils::UnsuedBlacklistedChromiumString(string_id))
    {
      kodi::Log(ADDON_LOG_DEBUG,
                "%s: Currently not on addon itself supported Chromium string id %i <------------",
              __func__, string_id);
    }
    return false;
  }
  return true;
}

bool CClientAppBrowser::GetDataResource(int resource_id, void*& data, size_t& data_size)
{
  return GetDataResourceForScale(resource_id, SCALE_FACTOR_NONE, data, data_size);
}

bool CClientAppBrowser::GetDataResourceForScale(int resource_id,
                                                ScaleFactor scale_factor,
                                                void*& data,
                                                size_t& data_size)
{
  //TODO is useful?
  return false;
}
//@}

/// CefBrowserProcessHandler
//@{
void CClientAppBrowser::GetCookieableSchemes(std::vector<CefString>& schemes,
                                             bool& include_defaults)
{
  // Defaults are "http", "https", "ws" and "wss"
  include_defaults = kodi::GetSettingBoolean("security.allow_cookies", true);
  if (include_defaults)
  {
    schemes.emplace_back("kodi");
  }
}

void CClientAppBrowser::OnBeforeCommandLineProcessing(const CefString& process_type,
                                                      CefRefPtr<CefCommandLine> command_line)
{
#ifdef WIN32
  command_line->AppendSwitch("disable-gpu-shader-disk-cache");
  command_line->AppendSwitchWithValue("use-angle", "d3d11");
#else
  command_line->AppendSwitch("disable-gpu-compositing");
  command_line->AppendSwitch("disable-software-rasterizer");
#endif // WIN32
}

void CClientAppBrowser::OnContextInitialized()
{
  // Register cookieable schemes with the global cookie manager.
  CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
  if (manager.get())
    manager->SetSupportedSchemes(m_cookieableSchemes, true, nullptr);
  else
    kodi::Log(ADDON_LOG_ERROR, "Failed to get cookie manager");

  m_printHandler = print::CPrintHandler::CreatePrintHandler();
}

void CClientAppBrowser::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
  const std::string id = m_addonMain->GetMainShared() + "-childProcess-" +
                         std::to_string(m_nextChildProcessIdentifier++);

  kodi::sandbox::CChildProcessor& proc = kodi::sandbox::CChildProcessor::GetActiveProcessor();
  std::string useId = proc.InitSubChild(id);

  command_line->AppendSwitchWithValue("main-shared", useId);
}

void CClientAppBrowser::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  chromium::app::common::RegisterCustomSchemes(registrar);
}

CefRefPtr<CefPrintHandler> CClientAppBrowser::GetPrintHandler()
{
  return m_printHandler;
}

void CClientAppBrowser::OnScheduleMessagePumpWork(int64 delay_ms)
{
  fprintf(stderr, "-----------------------\n");
}

//@}

} /* namespace main */
} /* namespace app */
} /* namespace chromium */