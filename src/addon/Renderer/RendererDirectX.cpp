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

#include <kodi/General.h>

#include "RendererDirectX.h"

#include "addon.h"
#include "Utils.h"
#include "WebBrowserClient.h"

#include "include/wrapper/cef_helpers.h"

CRendererClientDirectX::CRendererClientDirectX(CWebBrowserClient const* client)
  : IRenderer(client),
    m_texture(0)
{
}

CRendererClientDirectX::~CRendererClientDirectX()
{  
  m_texture = 0;
}

bool CRendererClientDirectX::Initialize()
{
  HRESULT hr = m_device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_texture, nullptr);
  if (hr != D3D_OK)
  {
    kodi::Log(ADDON_LOG_ERROR, "%s - failed to create texture", __FUNCTION__);
    return false;
  }
  return true;
}

void CRendererClientDirectX::Render()
{
  m_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0);

  D3DXMATRIX matProjection;
  D3DXMatrixIdentity(&matProjection);
  m_device->SetTransform(D3DTS_PROJECTION, &matProjection);

  D3DXMATRIX matView;
  D3DXMatrixIdentity(&matView);
  m_device->SetTransform(D3DTS_VIEW, &matView);

  D3DXMATRIX matWorld;
  D3DXMatrixIdentity(&matWorld);
  m_device->SetTransform(D3DTS_WORLD, &matWorld);

  CPaintMessage* msg = GetPaintMessage();
  if (msg)
  {
    CefBrowserHost::PaintElementType type = msg->type;
    const CefRenderHandler::RectList& dirtyRects = msg->dirtyRects;
    const void* buffer = msg->buffer;
    int width = msg->width;
    int height = msg->height;

    if (type == PET_VIEW)
    {
      int old_width = m_viewWidth;
      int old_height = m_viewHeight;
    
      m_viewWidth = width;
      m_viewHeight = height;
      
      // Update just the dirty rectangles.
      CefRenderHandler::RectList::const_iterator i = dirtyRects.begin();
      for (; i != dirtyRects.end(); ++i)
      {
        const CefRect& rect = *i;
        DCHECK(rect.x + rect.width <= m_viewWidth);
        DCHECK(rect.y + rect.height <= m_viewHeight);

        
        D3DLOCKED_RECT lockedRect;
        RECT dirtyRect;
        dirtyRect.bottom = rect.y+rect.height;
        dirtyRect.left = rect.x;
        dirtyRect.top = rect.y;
        dirtyRect.right = rect.x+rect.width;
        HRESULT hr = m_texture->LockRect(0, &lockedRect, &dirtyRect, 0);
        if (hr != D3D_OK)
        {
          kodi::Log(ADDON_LOG_ERROR, "%s - failed to lock texture", __FUNCTION__);
          continue;
        }
    
        uint8_t *source = (uint8_t*)m_osdTextures[i]->GetBuffer();
        uint8_t *dest = (uint8_t*)lockedRect.pBits;
        for (int y=y0; y<=y1; y++)
        {
          for (int x=x0; x<=x1; x++)
          {
            dest[y*lockedRect.Pitch+x*4] = source[y*width*4+x*4+2];  // blue
            dest[y*lockedRect.Pitch+x*4+1] = source[y*width*4+x*4+1];  // green
            dest[y*lockedRect.Pitch+x*4+2] = source[y*width*4+x*4];    // red
            dest[y*lockedRect.Pitch+x*4+3] = source[y*width*4+x*4+3];  // alpha
          }
        }
        m_texture->UnlockRect(0);
        if (hr != D3D_OK)
        {
          kodi::Log(ADDON_LOG_ERROR, "%s - failed to unlock texture", __FUNCTION__);
          continue;
        }
      }
    }
    else if (type == PET_POPUP && m_popupRect.width > 0 && m_popupRect.height > 0)
    {
      int skip_pixels = 0, x = m_popupRect.x;
      int skip_rows = 0, y = m_popupRect.y;
      int w = width;
      int h = height;
      
      
    }
    
    // render texture

    struct VERTEX
    {
      FLOAT x,y,z;
      DWORD color;
      FLOAT tu, tv;
    };

    VERTEX vertex[] =
    {
		  { 0.0f, 0.0f, 0.0f, 0xffffffff, 0.0f, 0.0f },
		  { 0.0f, 1.0f, 0.0f, 0xffffffff, 0.0f, 1.0f },
		  { 1.0f, 1.0f, 0.0f, 0xffffffff, 1.0f, 1.0f },
		  { 1.0f, 0.0f, 0.0f, 0xffffffff, 1.0f, 0.0f },
    };

    m_device->SetTexture(0, m_texture);
    HRESULT hr;
    hr = m_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
    hr = m_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    hr = m_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
    hr = m_device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    hr = m_device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
    hr = m_device->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

    hr = m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    hr = m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    hr = m_device->SetRenderState(D3DRS_ZENABLE, FALSE);
    hr = m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    hr = m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    hr = m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    hr = m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    hr = m_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED); 

    hr = m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    hr = m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    hr = m_device->SetPixelShader(nullptr);

    hr = m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    hr = m_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertex, sizeof(VERTEX));
    if (hr != D3D_OK)
	  {
	    kodi::Log(ADDON_LOG_ERROR, "%s - failed to render texture", __FUNCTION__);
    }
    m_device->SetTexture(0, nullptr);
    
    delete msg;
  }
}

void CRendererClientDirectX::ScreenSizeChange(float x, float y, float width, float height)
{
}
