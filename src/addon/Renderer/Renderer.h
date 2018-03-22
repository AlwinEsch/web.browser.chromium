#pragma once
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

#include "include/cef_render_handler.h"
#include "include/internal/cef_ptr.h"
#include "include/cef_base.h"

class CWebBrowserClient;
class IRenderer;

class CRendererClient
  : public CefRenderHandler
{
public:
  CRendererClient(CWebBrowserClient const* client);
  virtual ~CRendererClient();
  
  void Render();
  bool Dirty();
  void ScreenSizeChange(float x, float y, float width, float height);
  
  /// CefRenderHandler functions
  //@{
  virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
  virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) override;
  virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;
  virtual void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info) override;
  virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y) override;
  virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;
  virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override;
  virtual bool StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, DragOperationsMask allowed_ops, int x, int y) override;
  virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser, DragOperation operation) override;
  virtual void OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser, const CefRange& selected_range, const RectList& character_bounds) override;
  //@}

private:
  IMPLEMENT_REFCOUNTING(CRendererClient);
  
  bool Initialize();

  CWebBrowserClient const* m_client;
  IRenderer* m_renderer;
  bool m_initialized;
};
