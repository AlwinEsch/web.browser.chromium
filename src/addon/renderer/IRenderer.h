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

#pragma once

#include "include/cef_render_handler.h"

class CWebBrowserClient;

class IRenderer
{
public:
  IRenderer(CWebBrowserClient const* client);
  virtual ~IRenderer();

  virtual void OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) = 0;
  void SetDirty() { m_dirty = true; }
  bool Dirty();
  
  inline bool TransparentBackground() { return m_useTransparentBackground; }
  inline const float* BackgroundColor() { return m_backgroundColor; }
  
  virtual bool Initialize() { return false; }
  virtual void Deinitialize() { }
  virtual void Render() { }
  virtual void ScreenSizeChange(float x, float y, float width, float height) { }
  virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show);
  virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect);

  bool m_dirty = true;
  int m_viewWidth;
  int m_viewHeight;
  CefRect m_updateRect;

protected:
  bool m_useTransparentBackground;
  float m_backgroundColor[4];
  CefRect m_popupRect;
  CefRect m_originalPopupRect;

private:
  CWebBrowserClient const* m_client;

  CefRect GetPopupRectInWebView(const CefRect& original_rect);
  void ClearPopupRects();
};

