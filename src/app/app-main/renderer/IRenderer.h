/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_render_handler.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/addon-instance/Web.h"

namespace chromium
{
namespace app
{
namespace main
{

class CWebBrowserClient;

namespace renderer
{

class IRenderer
{
public:
  IRenderer(CefRefPtr<CWebBrowserClient> client);
  virtual ~IRenderer() = default;

  void ClearClient();

  virtual void OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height) { }
  virtual void OnAcceleratedPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, void* shared_handle) { }
  void SetDirty() { m_dirty = true; }
  bool Dirty();
  
  inline bool TransparentBackground() { return m_useTransparentBackground; }
  inline const float* BackgroundColor() { return m_backgroundColor; }
  
  virtual bool Initialize(int width, int height) { return false; }
  virtual void Deinitialize() { }
  virtual void Render() { }
  virtual void ScreenSizeChange(float x, float y, float width, float height) { }
  virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show);
  virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect);

  CefRefPtr<CWebBrowserClient> GetClient() { return m_client; }

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
  CefRefPtr<CWebBrowserClient> m_client;

  CefRect GetPopupRectInWebView(const CefRect& original_rect);
  void ClearPopupRects();
};

} /* renderer */
} /* main */
} /* app */
} /* chromium */
