#pragma once
/*
 *      Copyright (C) 2015-2018 Team KODI
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

#include <queue>
#include <p8-platform/threads/mutex.h>

class CWebBrowserClient;

#define MAX_MESSAGE_QUEUE_FILL_SIZE 100

class IRenderer;

typedef struct
{
  IRenderer* thisClass;
  CefBrowserHost::PaintElementType type;
  CefRenderHandler::RectList dirtyRects;
  const void* buffer;
  int width;
  int height;
} CPaintMessage;
  
class IRenderer
{
public:
  IRenderer(CWebBrowserClient const* client);
  virtual ~IRenderer();

  void OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height);
  CPaintMessage* GetPaintMessage();
  bool Dirty();
  
  inline bool TransparentBackground() { return m_useTransparentBackground; }
  inline const float* BackgroundColor() { return m_backgroundColor; }
  
  virtual bool Initialize() { return false; }
  virtual void Render() { }
  virtual void ScreenSizeChange(float x, float y, float width, float height) { }
  
  CWebBrowserClient const* m_client;
  
protected:
  bool m_useTransparentBackground;
  float m_backgroundColor[4];

private:
  std::queue <CPaintMessage*> m_paintQueue;
  P8PLATFORM::CMutex m_mutex;
};

