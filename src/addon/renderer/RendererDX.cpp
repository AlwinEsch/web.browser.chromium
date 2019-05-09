/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "RendererDX.h"

#include "addon.h"
#include "WebBrowserClient.h"
#include "utils/Utils.h"

#include "include/wrapper/cef_helpers.h"

#include <kodi/gui/General.h>

BrowserLayer::BrowserLayer(const std::shared_ptr<d3d11::Device>& device)
  : d3d11::Layer(device, true /* flip */) 
{
  m_frame_buffer = std::make_shared<d3d11::FrameBuffer>(m_device);
}

void BrowserLayer::render(const std::shared_ptr<d3d11::Context>& ctx)
{
  // Use the base class method to draw our texture.
  render_texture(ctx, m_frame_buffer->texture());
}

void BrowserLayer::on_paint(void* share_handle) 
{
  m_frame_buffer->on_paint(share_handle);
}

std::pair<uint32_t, uint32_t> BrowserLayer::texture_size() const 
{
  const auto texture = m_frame_buffer->texture();
  if (!texture)
    return std::pair<uint32_t, uint32_t>(0, 0);
  return std::make_pair(texture->width(), texture->height());
}

PopupLayer::PopupLayer(const std::shared_ptr<d3d11::Device>& device)
  : BrowserLayer(device) 
{
}

void PopupLayer::set_bounds(const CefRect& bounds) 
{
  const auto comp = composition();
  if (!comp)
    return;

  const auto outer_width = comp->width();
  const auto outer_height = comp->height();
  if (outer_width == 0 || outer_height == 0)
    return;

  m_original_bounds = bounds;
  m_bounds = bounds;

  // If x or y are negative, move them to 0.
  if (m_bounds.x < 0)
    m_bounds.x = 0;
  if (m_bounds.y < 0)
    m_bounds.y = 0;
  // If popup goes outside the view, try to reposition origin
  if (m_bounds.x + m_bounds.width > outer_width)
    m_bounds.x = outer_width - m_bounds.width;
  if (m_bounds.y + m_bounds.height > outer_height)
    m_bounds.y = outer_height - m_bounds.height;
  // If x or y became negative, move them to 0 again.
  if (m_bounds.x < 0)
    m_bounds.x = 0;
  if (m_bounds.y < 0)
    m_bounds.y = 0;

  const auto x = m_bounds.x / float(outer_width);
  const auto y = m_bounds.y / float(outer_height);
  const auto w = m_bounds.width / float(outer_width);
  const auto h = m_bounds.height / float(outer_height);
  move(x, y, w, h);
}

CRendererClientDirectX::CRendererClientDirectX(CefRefPtr<CWebBrowserClient> client)
  : IRenderer(client)
{
}

bool CRendererClientDirectX::Initialize()
{
  CEF_REQUIRE_UI_THREAD();

  // Create a D3D11 device instance.
  m_device = d3d11::Device::create();
  DCHECK(m_device);
  if (!m_device)
    return false;

  // Create the browser layer.
  m_browser_layer = std::make_shared<BrowserLayer>(m_device);

  // Set up the composition.
  m_composition = std::make_shared<d3d11::Composition>(m_device, m_viewWidth, m_viewHeight);
  m_composition->add_layer(m_browser_layer);

  // Size to the whole composition.
  m_browser_layer->move(0.0f, 0.0f, 1.0f, 1.0f);

  return true;
}

void CRendererClientDirectX::Deinitialize()
{
  m_composition.reset();
  m_browser_layer.reset();
  m_device.reset();
}

void CRendererClientDirectX::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
  CEF_REQUIRE_UI_THREAD();

  if (show) 
  {
    DCHECK(!m_popup_layer);

    // Create a new layer.
    m_popup_layer = std::make_shared<PopupLayer>(m_device);
    m_composition->add_layer(m_popup_layer);
  }
  else 
  {
    DCHECK(m_popup_layer);

    m_composition->remove_layer(m_popup_layer);
    m_popup_layer = nullptr;

    Render();
  }
}

void CRendererClientDirectX::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
  CEF_REQUIRE_UI_THREAD();
  m_popup_layer->set_bounds(rect);
}

void CRendererClientDirectX::OnAcceleratedPaint(CefBrowserHost::PaintElementType type,
                                                const CefRenderHandler::RectList& dirtyRects, 
                                                void* shared_handle)
{
  CEF_REQUIRE_UI_THREAD();
  if (type == PET_POPUP) 
  {
    m_popup_layer->on_paint(shared_handle);
  }
  else 
  {
    m_browser_layer->on_paint(shared_handle);
  }

  SetDirty();
}

void CRendererClientDirectX::Render()
{
  // Resize the composition and swap chain to match the texture if necessary.
  const auto texture_size = m_browser_layer->texture_size();
  m_composition->resize(!m_external_begin_frame_enabled, texture_size.first, texture_size.second);

  // Render the scene.
  auto ctx = m_device->immedidate_context();
  m_composition->render(ctx);
}

void CRendererClientDirectX::ScreenSizeChange(float x, float y, float width, float height)
{
}
