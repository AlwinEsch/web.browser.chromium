/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Renderer.h"

#include "addon.h"
#include "WebBrowserClient.h"

#if defined(HAS_GL) | defined(HAS_GLES)
#include "RendererGL.h"
#elif defined(HAS_DX)
#include "RendererDX.h"
#else
#error Render system is not supported.
#endif

#include "include/cef_browser.h"
#include "include/internal/cef_types_wrappers.h"
#include "include/wrapper/cef_helpers.h"

#include <kodi/gui/dialogs/Keyboard.h>

CRendererClient::CRendererClient(CefRefPtr<CWebBrowserClient> client) : m_client(client)
{
#if defined(HAS_GL) || defined(HAS_GLES)
  m_renderer = new CRendererClientOpenGL(m_client);
#elif defined(HAS_DX)
  m_renderer = new CRendererClientDirectX(m_client);
#else
  m_renderer = new IRenderer(m_client);
#endif
  m_renderer->Initialize();
}

CRendererClient::~CRendererClient()
{
  delete m_renderer;
}

void CRendererClient::ClearClient()
{
  m_renderer->ClearClient();
  m_client = nullptr;
}

void CRendererClient::Render()
{
  CEF_REQUIRE_UI_THREAD();
  m_renderer->Render();
}

bool CRendererClient::Dirty()
{
  CEF_REQUIRE_UI_THREAD();

  CefDoMessageLoopWork();
  return m_renderer->Dirty();
}

void CRendererClient::ScreenSizeChange(float x, float y, float width, float height)
{
  m_renderer->ScreenSizeChange(x, y, width, height);
}
  
void CRendererClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  CEF_REQUIRE_UI_THREAD();

  if (!m_client)
    return;

  // The simulated screen and view rectangle are the same. This is necessary
  // for popup menus to be located and sized inside the view.
  rect.x = 0;
  rect.y = 0;
  rect.width = static_cast<int>(m_client->GetWidth());
  rect.height = static_cast<int>(m_client->GetHeight());
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
  CEF_REQUIRE_UI_THREAD();

  m_renderer->OnPaint(type, dirtyRects, buffer, width, height);
}

void CRendererClient::OnAcceleratedPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, 
                                         const RectList& dirtyRects, void* shared_handle)
{
  CEF_REQUIRE_UI_THREAD();

  m_renderer->OnAcceleratedPaint(type, dirtyRects, shared_handle);
}

void CRendererClient::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
  m_renderer->OnPopupShow(browser, show);
}

void CRendererClient::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
  m_renderer->OnPopupSize(browser, rect);
}

void CRendererClient::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info)
{
  ///TODO Use to improve human readable view of mouse area over far distance?
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

void CRendererClient::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y)
{
fprintf(stderr, "----------------- %f %f\n",x, y);
  m_scrollOffsetX = x;
  m_scrollOffsetY = y;
}

void CRendererClient::OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser, const CefRange& selected_range, const RectList& character_bounds)
{
  ///TODO Is this usable for Kodi?
}

void CRendererClient::OnTextSelectionChanged(CefRefPtr<CefBrowser> browser, const CefString& selected_text, const CefRange& selected_range)
{
  ///TODO Is this usable for Kodi?
}

void CRendererClient::OnVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser, TextInputMode input_mode)
{
  if (m_client && !m_client->ContextMenuOpen())
  {
    if (input_mode != CEF_TEXT_INPUT_MODE_NONE)
      m_client->GetMain().GetGUIManager().GetKeyboard().Show(m_client, input_mode);
    else
      m_client->GetMain().GetGUIManager().GetKeyboard().Close();
  }
}
