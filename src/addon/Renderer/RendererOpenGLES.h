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

#include <queue>

#include "include/cef_render_handler.h"

class CWebBrowserClient;

class CRendererClientOpenGLES :
  public CefRenderHandler
{
public:
  CRendererClientOpenGLES(CWebBrowserClient* client);
  virtual ~CRendererClientOpenGLES();

  virtual bool Initialize();

  void Render();
  bool Dirty();

  /*!
   * Kodi add-on render functions
   */
  virtual void Cleanup();

  /*!
   * CEF class handler alignment
   */
  IMPLEMENT_REFCOUNTING(CRendererClientOpenGLES);

  /*!
   * CefRenderHandler functions
   */
  virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
  virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) override;
  virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;

private:
  CWebBrowserClient* m_client;
  bool m_initialized; /*!< Render initialization done flag, becomes called on first OnPaint call */
  unsigned int m_textureId;
  int m_viewWidth;
  int m_viewHeight;
  float m_fSpinX;
  float m_fSpinY;
  CefRect m_popupRect;
  CefRect m_updateRect;
  bool m_dirty;
  bool m_useTransparentBackground;
  float m_backgroundColor[4];

  typedef struct
  {
    CRendererClientOpenGLES* thisClass;
    CefRefPtr<CefBrowser> browser;
    CefBrowserHost::PaintElementType type;
    CefRenderHandler::RectList dirtyRects;
    const void* buffer;
    int width;
    int height;
  } CPaintMessage;
  std::queue <CPaintMessage*> m_paintQueue;
};
