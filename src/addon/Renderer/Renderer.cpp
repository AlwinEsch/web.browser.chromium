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

#include "Renderer.h"
#include "WebBrowserClient.h"

#if defined(HAS_GL) | defined(HAS_GLES2)
#include "RendererOpenGL.h"
#elif defined(HAS_DX)
#include "CRendererClientDirectX.h"
#error Render system is currently not implemented.
#else
#error Render system is not supported.
#endif

#include "include/cef_browser.h"
#include "include/internal/cef_types_wrappers.h"
#include "include/wrapper/cef_helpers.h"

CRendererClient::CRendererClient(CWebBrowserClient const* client)
  : m_client(client),
    m_renderer(nullptr),
    m_initialized(false)
{
}

CRendererClient::~CRendererClient()
{
  if (m_renderer)
  {
    delete m_renderer;
    m_renderer = nullptr;
  }
}

bool CRendererClient::Initialize()
{
#if defined(HAS_GL) || defined(HAS_GLES2)
  m_renderer = new CRendererClientOpenGL(m_client);
#elif defined(HAS_DX)
  m_renderer = new CRendererClientDirectX(m_client);
#else
  m_renderer = new IRenderer(m_client);
#endif
  
  if (!m_renderer->Initialize())
  {
    delete m_renderer;
    m_renderer = nullptr;
  }
}

void CRendererClient::Render()
{
  if (m_renderer)
    m_renderer->Render();
}

bool CRendererClient::Dirty()
{
  if (!m_initialized)
  {
    Initialize();
    m_initialized = true;
  }

  if (m_renderer)
    return m_renderer->Dirty();
  return false;
}

void CRendererClient::ScreenSizeChange(float x, float y, float width, float height)
{
  if (m_renderer)
    m_renderer->ScreenSizeChange(x, y, width, height);
}
  

bool CRendererClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  CEF_REQUIRE_UI_THREAD();

  // The simulated screen and view rectangle are the same. This is necessary
  // for popup menus to be located and sized inside the view.
  rect.x = 0.0f;
  rect.y = 0.0f;
  rect.width = m_client->GetWidth();
  rect.height = m_client->GetHeight();
  return true;
}

bool CRendererClient::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  CEF_REQUIRE_UI_THREAD();

  screenX = viewX;
  screenY = viewY;
  return true;
}

void CRendererClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                              const RectList& dirtyRects, const void* buffer,
                              int width, int height)
{
  if (m_renderer)
    m_renderer->OnPaint(type, dirtyRects, buffer, width, height);
}

void CRendererClient::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info)
{
  ///TODO Use to improve human readable view of mouse area over far distance?
}

void CRendererClient::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y)
{
  ///TODO Use to replace chromium scroll bars with Kodi's one?
}

void CRendererClient::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
  ///TODO Is this usable for Kodi?
}

void CRendererClient::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
  ///TODO Is this usable for Kodi?
}

bool CRendererClient::StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, DragOperationsMask allowed_ops, int x, int y)
{
  ///TODO Is this usable for Kodi?
  return false;
}

void CRendererClient::UpdateDragCursor(CefRefPtr<CefBrowser> browser, DragOperation operation)
{
  ///TODO Is this usable for Kodi?
}

void CRendererClient::OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser, const CefRange& selected_range, const RectList& character_bounds)
{
  ///TODO Is this usable for Kodi?
}
