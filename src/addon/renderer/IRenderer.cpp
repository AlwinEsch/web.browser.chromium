/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "IRenderer.h"

#include "WebBrowserClient.h"

#include <kodi/General.h>

IRenderer::IRenderer(CefRefPtr<CWebBrowserClient> client)
  : m_client(client),
    m_viewWidth(0),
    m_viewHeight(0)
{
  uint32_t color = m_client->GetBackgroundColorARGB();
  m_backgroundColor[3] = float(CefColorGetA(color)) / 255.0f;
  m_backgroundColor[2] = float(CefColorGetR(color)) / 255.0f;
  m_backgroundColor[1] = float(CefColorGetG(color)) / 255.0f;
  m_backgroundColor[0] = float(CefColorGetB(color)) / 255.0f;
  m_useTransparentBackground = m_client->UseTransparentBackground();
}

void IRenderer::ClearClient()
{
  m_client = nullptr;
}

void IRenderer::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
  if (!show)
  {
    // Clear the popup rectangle.
    ClearPopupRects();
  }
}

void IRenderer::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
  if (rect.width <= 0 || rect.height <= 0)
    return;
  m_originalPopupRect = rect;
  m_popupRect = GetPopupRectInWebView(m_originalPopupRect);
}

CefRect IRenderer::GetPopupRectInWebView(const CefRect& original_rect)
{
  CefRect rc(original_rect);
  // if x or y are negative, move them to 0.
  if (rc.x < 0)
    rc.x = 0;
  if (rc.y < 0)
    rc.y = 0;
  // if popup goes outside the view, try to reposition origin
  if (rc.x + rc.width > m_viewWidth)
    rc.x = m_viewWidth - rc.width;
  if (rc.y + rc.height > m_viewHeight)
    rc.y = m_viewHeight - rc.height;
  // if x or y became negative, move them to 0 again.
  if (rc.x < 0)
    rc.x = 0;
  if (rc.y < 0)
    rc.y = 0;
  return rc;
}

void IRenderer::ClearPopupRects()
{
  m_popupRect.Set(0, 0, 0, 0);
  m_originalPopupRect.Set(0, 0, 0, 0);
}

bool IRenderer::Dirty()
{
  bool ret = m_dirty;
  m_dirty = false;
  return ret;
}

