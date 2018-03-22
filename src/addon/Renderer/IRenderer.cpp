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

#include "IRenderer.h"

#include "WebBrowserClient.h"

#include <kodi/General.h>

IRenderer::IRenderer(CWebBrowserClient const* client)
  : m_client(client)
{
  uint32_t color = m_client->GetBackgroundColorARGB();
  m_backgroundColor[3] = float(CefColorGetA(color)) / 255.0f;
  m_backgroundColor[2] = float(CefColorGetR(color)) / 255.0f;
  m_backgroundColor[1] = float(CefColorGetG(color)) / 255.0f;
  m_backgroundColor[0] = float(CefColorGetB(color)) / 255.0f;
  m_useTransparentBackground = m_client->UseTransparentBackground();
}

IRenderer::~IRenderer()
{
  while (!m_paintQueue.empty())
  {
    delete m_paintQueue.front();
    m_paintQueue.pop();
  }
}

void IRenderer::OnPaint(CefBrowserHost::PaintElementType type, const CefRenderHandler::RectList& dirtyRects, const void* buffer, int width, int height)
{
  if (width <= 2 && height <= 2)
  {
    // Ignore really small buffer sizes while the widget is starting up.
    return;
  }
  
  P8PLATFORM::CLockObject lock(m_mutex);

  if (m_paintQueue.size() > MAX_MESSAGE_QUEUE_FILL_SIZE)
  {
    kodi::Log(ADDON_LOG_ERROR, "Renderer paint message flow reached max size, further calls becomes ignored!");
    return;
  }

  CPaintMessage* message = new CPaintMessage;
  message->thisClass = this;
  message->type = type;
  message->dirtyRects = dirtyRects;
  message->buffer = buffer;
  message->width = width;
  message->height = height;
  m_paintQueue.push(message);
}

bool IRenderer::Dirty()
{
  P8PLATFORM::CLockObject lock(m_mutex);
  return !m_paintQueue.empty();
}

CPaintMessage* IRenderer::GetPaintMessage()
{
  CPaintMessage* msg = nullptr;

  P8PLATFORM::CLockObject lock(m_mutex);

  if (!m_paintQueue.empty())
  {
    msg = m_paintQueue.front();
    m_paintQueue.pop();
  }
  return msg;
}
