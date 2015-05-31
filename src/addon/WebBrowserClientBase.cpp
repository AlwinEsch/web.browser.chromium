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

#include <stdio.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_parser.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include "addon.h"
#include "WebBrowserClientBase.h"
#include "Utils.h"

using namespace std;
using namespace ADDON;

CWebBrowserClientBase::CWebBrowserClientBase(int iUniqueClientId, const WEB_ADDON_GUI_PROPS *props) :
  m_iUniqueClientId(iUniqueClientId),
  m_Browser(NULL),
  m_pDevice(props->pDevice),
  m_iXPos(props->iXPos),
  m_iYPos(props->iYPos),
  m_iWidth(props->iWidth),
  m_iHeight(props->iHeight),
  m_fPixelRatio(props->fPixelRatio),
  m_strIdName(props->strName),
  m_iLeaveOpenTime(props->iLeaveOpenTime),
  m_iGUIItemLeft(props->iGUIItemLeft),
  m_iGUIItemRight(props->iGUIItemRight),
  m_iGUIItemTop(props->iGUIItemTop),
  m_iGUIItemBottom(props->iGUIItemBottom),
  m_iGUIItemBack(props->iGUIItemBack),
  m_bTransparentBackground(props->bUseTransparentBackground),
  m_pControlIdent(props->pControlIdent)
{
  m_BackgroundColor[3] = float(CefColorGetA(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[2] = float(CefColorGetR(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[1] = float(CefColorGetG(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[0] = float(CefColorGetB(props->iBackgroundColorARGB)) / 255.0f;
}

CWebBrowserClientBase::~CWebBrowserClientBase()
{
}

bool CWebBrowserClientBase::SetInactive()
{
  m_inactivateTime = time(NULL);
  return m_iLeaveOpenTime < 0;
}

bool CWebBrowserClientBase::SetActive()
{
  return true;
}

int CWebBrowserClientBase::CurrentInactiveCountdown()
{
  return time(NULL) - m_inactivateTime + m_iLeaveOpenTime;
}

bool CWebBrowserClientBase::Initialize()
{
  return true;
}

bool CWebBrowserClientBase::OpenWebsite(const char* strURL, bool single, bool allowMenus)
{
  if (!m_Browser.get())
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Called without present browser", __FUNCTION__);
    return false;
  }

  m_Browser->GetMainFrame()->LoadURL(strURL);
//  m_Browser->Reload();
  return true;
}

void CWebBrowserClientBase::SetAddonHandle(ADDON_HANDLE addonHandle)
{
  m_addonHandle.callerAddress = addonHandle->callerAddress;
  m_addonHandle.dataIdentifier = addonHandle->dataIdentifier;
  m_addonHandle.dataAddress = m_pControlIdent;
}

/*!
 * @brief CefClient methods
 *
 * Implement this interface to provide handler implementations.
 */
//{
bool CWebBrowserClientBase::OnProcessMessageReceived(
    CefRefPtr<CefBrowser>                 browser,
    CefProcessId                          source_process,
    CefRefPtr<CefProcessMessage>          message)
{
  CEF_REQUIRE_UI_THREAD();

  return false;
}
//}

/*!
 * @brief CefContextMenuHandler methods
 */
//{
void CWebBrowserClientBase::OnBeforeContextMenu(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefContextMenuParams>       params,
    CefRefPtr<CefMenuModel>               model)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnContextMenuCommand(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefContextMenuParams>       params,
    int                                   command_id,
    EventFlags                            event_flags)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnContextMenuDismissed(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefDisplayHandler methods
 */
//{
void CWebBrowserClientBase::OnAddressChange(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      url)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s - %s", __FUNCTION__, url.ToString().c_str());
}

void CWebBrowserClientBase::OnTitleChange(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      title)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s - %s", __FUNCTION__, title.ToString().c_str());
  WEB->Control_SetOpenedTitle(&m_addonHandle, title.ToString().c_str());
}

void CWebBrowserClientBase::OnFaviconURLChange(
    CefRefPtr<CefBrowser>                 browser,
    const std::vector<CefString>&         icon_urls)
{

//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnTooltip(
    CefRefPtr<CefBrowser>                 browser,
    CefString&                            text)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s - %s", __FUNCTION__, text.ToString().c_str());
  return false;
}

void CWebBrowserClientBase::OnStatusMessage(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      value)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s - %s", __FUNCTION__, value.ToString().c_str());
}

bool CWebBrowserClientBase::OnConsoleMessage(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      message,
    const CefString&                      source,
    int                                   line)
{
  LOG_INTERNAL_MESSAGE(LOG_ERROR, "%s - Message: %s - Source: %s - Line: %i", __FUNCTION__, message.ToString().c_str(), source.ToString().c_str(), line);
  return true;
}
//}

/*!
 * @brief CefDownloadHandler methods
 */
//{
void CWebBrowserClientBase::OnBeforeDownload(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefDownloadItem>            download_item,
    const CefString&                      suggested_name,
    CefRefPtr<CefBeforeDownloadCallback>  callback)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnDownloadUpdated(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefDownloadItem>            download_item,
    CefRefPtr<CefDownloadItemCallback>    callback)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefDragHandler methods
 */
//{
bool CWebBrowserClientBase::OnDragEnter(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDragData> dragData,
    CefRenderHandler::DragOperationsMask mask)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}
//}

/*!
 * @brief CefGeolocationHandler methods
 */
//{
bool CWebBrowserClientBase::OnRequestGeolocationPermission(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      requesting_url,
    int                                   request_id,
    CefRefPtr<CefGeolocationCallback>     callback)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}


void CWebBrowserClientBase::OnCancelGeolocationPermission(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      requesting_url,
    int                                   request_id)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefLifeSpanHandler methods
 */
//{
bool CWebBrowserClientBase::OnBeforePopup(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      target_url,
    const CefString&                      target_frame_name,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool                                  user_gesture,
    const CefPopupFeatures&               popupFeatures,
    CefWindowInfo&                        windowInfo,
    CefRefPtr<CefClient>&                 client,
    CefBrowserSettings&                   settings,
    bool*                                 no_javascript_access)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  if (!m_Browser)
  {
    m_Browser = browser;
  }
}

bool CWebBrowserClientBase::RunModal(CefRefPtr<CefBrowser> browser)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::DoClose(CefRefPtr<CefBrowser> browser)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefLoadHandler methods
 */
//{
void CWebBrowserClientBase::OnLoadingStateChange(
    CefRefPtr<CefBrowser>                 browser,
    bool                                  isLoading,
    bool                                  canGoBack,
    bool                                  canGoForward)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnLoadStart(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnLoadEnd(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    int                                   httpStatusCode)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnLoadError(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    ErrorCode                             errorCode,
    const CefString&                      errorText,
    const CefString&                      failedUrl)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefKeyboardHandler methods
 */
//{
bool CWebBrowserClientBase::OnPreKeyEvent(
    CefRefPtr<CefBrowser>                 browser,
    const CefKeyEvent&                    event,
    CefEventHandle                        os_event,
    bool*                                 is_keyboard_shortcut)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::OnKeyEvent(
    CefRefPtr<CefBrowser>                 browser,
    const CefKeyEvent&                    event,
    CefEventHandle                        os_event)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}
//}

/*!
 * @brief CefRequestHandler methods
 */
//{
bool CWebBrowserClientBase::OnBeforeBrowse(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    bool                                  is_redirect)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::OnOpenURLFromTab(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      target_url,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool                                  user_gesture)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

CefRequestHandler::ReturnValue CWebBrowserClientBase::OnBeforeResourceLoad(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefRefPtr<CefRequestCallback>         callback)
{
  return RV_CONTINUE;
}

void CWebBrowserClientBase::OnResourceRedirect(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefString&                            new_url)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnResourceResponse(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefRefPtr<CefResponse>                response)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::GetAuthCredentials(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    bool                                  isProxy,
    const CefString&                      host,
    int                                   port,
    const CefString&                      realm,
    const CefString&                      scheme,
    CefRefPtr<CefAuthCallback>            callback)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

CefRefPtr<CefResourceHandler> CWebBrowserClientBase::GetResourceHandler(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return NULL;
}

bool CWebBrowserClientBase::OnQuotaRequest(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      origin_url,
    int64                                 new_size,
    CefRefPtr<CefRequestCallback>         callback)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnProtocolExecution(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      url,
    bool&                                 allow_os_execution)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnCertificateError(
    CefRefPtr<CefBrowser>                 browser,
    ErrorCode                             cert_error,
    const CefString&                      request_url,
    CefRefPtr<CefSSLInfo>                 ssl_info,
    CefRefPtr<CefRequestCallback>         callback)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::OnBeforePluginLoad(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      url,
    const CefString&                      policy_url,
    CefRefPtr<CefWebPluginInfo>           info)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnPluginCrashed(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      plugin_path)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnRenderViewReady(
    CefRefPtr<CefBrowser>                 browser)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnRenderProcessTerminated(
    CefRefPtr<CefBrowser>                 browser,
    TerminationStatus                     status)
{
//  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}
