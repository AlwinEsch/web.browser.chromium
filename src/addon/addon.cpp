/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "addon.h"

#include "../../lib/kodi-dev-kit/src/server/LauncherPosix.h"
#include "../../lib/kodi-dev-kit/src/server/kodi/addon-instance/Web.h"
#include "utils/Utils.h"

#include <kodi/tools/StringUtils.h>

using kodi::tools::StringUtils;

CAddon::CAddon()
{
}

CAddon::~CAddon()
{
}

ADDON_STATUS CAddon::Create()
{


  return ADDON_STATUS_OK;
}

ADDON_STATUS CAddon::CreateInstance(int instanceType,
                                    const std::string& instanceID,
                                    KODI_HANDLE instance,
                                    const std::string& version,
                                    KODI_HANDLE& addonInstance)
{
  if (instanceType == ADDON_INSTANCE_WEB)
  {
    addonInstance = new CWebBrowser(instance, version);
  }
  else
  {
    return ADDON_STATUS_UNKNOWN;
  }

  return ADDON_STATUS_OK;
}

// ---------------------------------------------------------------------------

CWebBrowser::CWebBrowser(KODI_HANDLE instance, const std::string& kodiVersion)
  : kodi::addon::CInstanceWebOwn(instance, kodiVersion),
    m_launcher(new kodi::sandbox::parent::CChildLauncherPosix),
    m_hdl(dynamic_cast<kodi::sandbox::parent::C_addoninstance_Web_h*>(
        m_launcher->GetHdl(kodi::sandbox::funcGroup_addoninstance_Web_h)))
{
}

CWebBrowser::~CWebBrowser()
{
  delete m_launcher;
}

WEB_ADDON_ERROR CWebBrowser::StartInstance()
{
  return WEB_ADDON_ERROR_NO_ERROR;
}

void CWebBrowser::StopInstance()
{
}

bool CWebBrowser::MainInitialize()
{
  std::vector<std::string> argv;
  argv.push_back(kodi::GetAddonPath("kodichromium" EXECUTABLE_SUFFIX));
  argv.push_back("--sandbox=webbrowser");
  argv.push_back("--libpath=" + AddonLibPath());
  argv.push_back("--sharepath=" + AddonSharePath());
  argv.push_back("--basehandle=" + StringUtils::Format("%llX", this));

  if (!m_launcher->Launch(argv, false))
  {
    LOG_MESSAGE(ADDON_LOG_ERROR, "CWebBrowser::%s: Creation failed", __func__);
    return false;
  }

  LOG_MESSAGE(ADDON_LOG_DEBUG, "CWebBrowser::%s: Creation done", __func__);
  return true;
}

void CWebBrowser::MainShutdown()
{
  if (m_childHdl)
    m_hdl->MainShutdown(m_childHdl);
}

void CWebBrowser::MainLoop()
{
  if (m_childHdl)
    m_hdl->MainLoop(m_childHdl);
}

void CWebBrowser::StartDone(uint64_t childHdl)
{
  m_childHdl = childHdl;
}

void CWebBrowser::SetMute(bool mute)
{
  if (m_childHdl)
    m_hdl->SetMute(m_childHdl, mute);
}

bool CWebBrowser::SetLanguage(const std::string& language)
{
  if (m_childHdl)
    return m_hdl->SetLanguage(m_childHdl, language);
  return false;
}

uint64_t CWebBrowser::CreateControl(const std::string& sourceName,
                                    const std::string& startURL,
                                    WEB_CONTROL_HANDLE& handle,
                                    int& dataIdentifier)
{
  if (!m_childHdl)
    return 0;

  return m_hdl->CreateControl(m_childHdl, sourceName, startURL, handle, dataIdentifier);
}

bool CWebBrowser::DestroyControl(uint64_t control, bool complete)
{
  if (!m_childHdl)
    return false;

  return m_hdl->DestroyControl(m_childHdl, control, complete);
}

CWebBrowserClient::CWebBrowserClient(WEB_CONTROL_HANDLE& hdlKodi,
                                     uint64_t hdlChild,
                                     int dataIdentifier,
                                     kodi::sandbox::parent::C_addoninstance_Web_h* hdlParent)
  : kodi::addon::CWebControlOwn(hdlKodi, dataIdentifier),
    m_hdl(hdlParent),
    m_controlHdl(hdlChild)
{
  m_clientIdentifier = StringUtils::Format("%s-client-%04i", m_hdl->GetIdentifier().c_str(), dataIdentifier);
  m_rendering = new CViewRendererOpenGL(m_clientIdentifier);
}

CWebBrowserClient::~CWebBrowserClient()
{
  delete m_rendering;
}

bool CWebBrowserClient::RenderInit(int width, int height)
{
  return m_rendering->Init();
}

void CWebBrowserClient::Render()
{
  m_hdl->ControlRender(m_controlHdl);
  m_rendering->Render();
}

bool CWebBrowserClient::Dirty()
{
  return m_hdl->ControlDirty(m_controlHdl);
}

bool CWebBrowserClient::OnInit()
{
  return m_hdl->ControlOnInit(m_controlHdl);
}

bool CWebBrowserClient::OnAction(const kodi::gui::input::CAction& action,
                                  int& nextItem)
{
  return m_hdl->ControlOnAction(m_controlHdl, action, nextItem);
}

bool CWebBrowserClient::OnMouseEvent(
    int id, double x, double y, double offsetX, double offsetY, int state)
{
  return m_hdl->ControlOnMouseEvent(m_controlHdl, id, x, y, offsetX, offsetY, state);
}

bool CWebBrowserClient::OpenWebsite(const std::string& url)
{
  return m_hdl->ControlOpenWebsite(m_controlHdl, url);
}

bool CWebBrowserClient::GetHistory(std::vector<std::string>& historyWebsiteNames,
                                    bool behindCurrent)
{
  return m_hdl->ControlGetHistory(m_controlHdl, historyWebsiteNames, behindCurrent);
}

void CWebBrowserClient::SearchText(
    const std::string& text, bool forward, bool matchCase, bool findNext)
{
  return m_hdl->ControlSearchText(m_controlHdl, text, forward, matchCase, findNext);
}

void CWebBrowserClient::StopSearch(bool clearSelection)
{
  return m_hdl->ControlStopSearch(m_controlHdl, clearSelection);
}

void CWebBrowserClient::Reload()
{
  return m_hdl->ControlReload(m_controlHdl);
}

void CWebBrowserClient::StopLoad()
{
  return m_hdl->ControlStopLoad(m_controlHdl);
}

void CWebBrowserClient::GoBack()
{
  return m_hdl->ControlGoBack(m_controlHdl);
}

void CWebBrowserClient::GoForward()
{
  return m_hdl->ControlGoForward(m_controlHdl);
}

void CWebBrowserClient::OpenOwnContextMenu()
{
  return m_hdl->ControlOpenOwnContextMenu(m_controlHdl);
}

void CWebBrowserClient::ScreenSizeChange(
    float x, float y, float width, float height, bool fullscreen)
{
  return m_hdl->ControlScreenSizeChange(m_controlHdl, x, y, width, height, fullscreen);
}

ADDONCREATOR(CAddon)
