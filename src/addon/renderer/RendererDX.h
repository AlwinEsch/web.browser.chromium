/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IRenderer.h"
#include "d3d11.h"

#include "include/cef_render_handler.h"

#include <kodi/General.h>

class CWebBrowserClient;

class BrowserLayer : public d3d11::Layer 
{
public:
  explicit BrowserLayer(const std::shared_ptr<d3d11::Device>& device);

  void render(const std::shared_ptr<d3d11::Context>& ctx) OVERRIDE;

  void on_paint(void* share_handle);

  // After calling on_paint() we can query the texture size.
  std::pair<uint32_t, uint32_t> texture_size() const;

private:
  std::shared_ptr<d3d11::FrameBuffer> m_frame_buffer;

  DISALLOW_COPY_AND_ASSIGN(BrowserLayer);
};

class PopupLayer : public BrowserLayer 
{
public:
  explicit PopupLayer(const std::shared_ptr<d3d11::Device>& device);

  void set_bounds(const CefRect& bounds);

  bool contains(int x, int y) const { return m_bounds.Contains(x, y); }
  int xoffset() const { return m_original_bounds.x - m_bounds.x; }
  int yoffset() const { return m_original_bounds.y - m_bounds.y; }

private:
  CefRect m_original_bounds;
  CefRect m_bounds;

  DISALLOW_COPY_AND_ASSIGN(PopupLayer);
};

class ATTRIBUTE_HIDDEN CRendererClientDirectX : public IRenderer
{
public:
  CRendererClientDirectX(CefRefPtr<CWebBrowserClient> client);
  ~CRendererClientDirectX() = default;

  bool Initialize() override;
  void Deinitialize() override;

  void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;
  void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override;
  void OnAcceleratedPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, void* shared_handle) override;

  void Render() override;
  void ScreenSizeChange(float x, float y, float width, float height) override;

private:
  int m_viewWidth;
  int m_viewHeight;
  std::shared_ptr<d3d11::Device> m_device;
  std::shared_ptr<d3d11::SwapChain> m_swap_chain;
  std::shared_ptr<d3d11::Composition> m_composition;
  std::shared_ptr<BrowserLayer> m_browser_layer;
  std::shared_ptr<PopupLayer> m_popup_layer;

  bool m_external_begin_frame_enabled = true;

  DISALLOW_COPY_AND_ASSIGN(CRendererClientDirectX);
};
