#pragma once
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

#include <queue>

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "platform/threads/threads.h"

class CWebBrowserClientBase :
  public CefClient,
  public CefLifeSpanHandler
{
public:
  CWebBrowserClientBase(int iUniqueClientId, const WEB_ADDON_GUI_PROPS *props);
  virtual ~CWebBrowserClientBase();

  /*!
   * @brief return the unique identification id of this control client
   */
  int GetUniqueId() { return m_iUniqueClientId; }

  /*!
   * @brief return the from Kodi given identifier name of this browser client
   * Used to identify opened browser on related GUI control.
   */
  std::string GetIdName() { return m_strIdName; }

  /*!
   * @brief Used to inform client to stay inactive and store the time.
   * @return true if need removed, false to leave open for a time
   */
  bool SetInactive();

  /*!
   * @brief Used to inform client to open inactive client again
   * @return true if successed
   */
  bool SetActive();

  /*!
   * @brief Inactive time
   * @return the time to leave open, on every call it become less.
   */
  int CurrentInactiveCountdown();

  /*!
   * @brief Used to initialize client
   * @return true if successed
   */
  bool Initialize();

  /*!
   * @brief Open gived website or local file url
   * @param strURL url to open
   * @param single Allow only one possible website control
   * @param allowMenus Allow to have standart browser menus (can be disabled
   * on use by add-on's which use the system to implement kodi add-ons based
   * on this.
   * @return true if successed
   */
  bool OpenWebsite(const char* strURL, bool single, bool allowMenus);

  /*!
   * Kodi add-on render functions
   */
  virtual void Cleanup() = 0;
  virtual void Render() = 0;
  virtual bool Dirty() = 0;

  /*!
   * CEF class handler alignment
   */
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }

  /*!
   * CefLifeSpanHandler functions
   */
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser)  OVERRIDE;

  IMPLEMENT_REFCOUNTING(CWebBrowserClientBase);

protected:
  /*!
   * Given Kodi's settings data
   */
  void        *m_pDevice;
  int          m_iLeaveOpenTime;
  int          m_iXPos;
  int          m_iYPos;
  int          m_iWidth;
  int          m_iHeight;
  float        m_fPixelRatio;
  std::string  m_strIdName;
  int          m_iGUIItemLeft;
  int          m_iGUIItemRight;
  int          m_iGUIItemTop;
  int          m_iGUIItemBottom;
  int          m_iGUIItemBack;
  bool         m_bTransparentBackground;

  float        m_BackgroundColor[4];

private:
  const int    m_iUniqueClientId;  /*!< Unique identification id of this control client */
  time_t       m_inactivateTime;      /*!< Time where client becomes set inactive to handle the timeout */
  CefRefPtr<CefBrowser> m_Browser;
};
