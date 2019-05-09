/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_render_handler.h"
#include "include/internal/cef_ptr.h"
#include "include/cef_base.h"

class CWebBrowserClient;
class IRenderer;

class CRendererClient : public CefRenderHandler
{
public:
  CRendererClient(CefRefPtr<CWebBrowserClient> client);
  ~CRendererClient() override;

  void ClearClient();
  
  bool SetActive();
  void SetInactive();
  void Render();
  bool Dirty();
  void ScreenSizeChange(float x, float y, float width, float height);

  double ScrollOffsetX() { return m_scrollOffsetX; }
  double ScrollOffsetY() { return m_scrollOffsetY; }

  /// CefRenderHandler functions
  //@{
  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
  bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) override;
  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;
  void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, void* shared_handle) override;
  void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info) override;
  void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;
  void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override;
  bool StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, DragOperationsMask allowed_ops, int x, int y) override;
  void UpdateDragCursor(CefRefPtr<CefBrowser> browser, DragOperation operation) override;
  void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y) override;
  void OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser, const CefRange& selected_range, const RectList& character_bounds) override;
  void OnTextSelectionChanged(CefRefPtr<CefBrowser> browser, const CefString& selected_text, const CefRange& selected_range) override;
  void OnVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser, TextInputMode input_mode) override;
  //@}

private:
  IMPLEMENT_REFCOUNTING(CRendererClient);
  
  double m_scrollOffsetX = 0.0;
  double m_scrollOffsetY = 0.0;
  CefRefPtr<CWebBrowserClient> m_client;
  IRenderer* m_renderer;
};
