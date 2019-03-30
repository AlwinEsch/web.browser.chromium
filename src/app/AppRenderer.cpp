/*
 *      Copyright (C) 2015-2019 Team KODI
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

#include "AppRenderer.h"

CWebAppRenderer::CWebAppRenderer()
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

void CWebAppRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

void CWebAppRenderer::OnWebKitInitialized()
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

void CWebAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

void CWebAppRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

CefRefPtr<CefLoadHandler> CWebAppRenderer::GetLoadHandler()
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
  return nullptr;
}

void CWebAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

void CWebAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

void CWebAppRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,CefRefPtr<CefV8Context> context,
                                          CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

void CWebAppRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
}

bool CWebAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  fprintf(stderr, "---> %s\n", __PRETTY_FUNCTION__);
  return false;
}
