// Copyright 2018 The Chromium Embedded Framework Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.
//
// Portions Copyright (c) 2018 Daktronics with the following MIT License:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

#include "d3d11.h"

#include <iomanip>  // For std::setw.

#include <d3dcompiler.h>
#include <directxmath.h>
#include <kodi/gui/General.h>

#include "include/base/cef_logging.h"
#include "include/internal/cef_string.h"

namespace
{
  LARGE_INTEGER g_qiFreq = {};
}  // namespace

uint64_t GetTimeNow()
{
  if (!g_qiFreq.HighPart && !g_qiFreq.LowPart) 
  {
    QueryPerformanceFrequency(&g_qiFreq);
  }
  LARGE_INTEGER t = {};
  QueryPerformanceCounter(&t);
  return static_cast<uint64_t>((t.QuadPart / double(g_qiFreq.QuadPart)) * 1000000);
}

namespace d3d11
{

namespace
{

// Wrap a raw COM pointer in a shared_ptr for auto Release().
template <class T>
std::shared_ptr<T> to_com_ptr(T* obj)
{
  return std::shared_ptr<T>(obj, [](T* p)
  {
    if (p)
      p->Release();
  });
}

}  // namespace

struct SimpleVertex
{
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT2 tex;
};

Context::Context(ID3D11DeviceContext* ctx) : m_ctx(to_com_ptr(ctx))
{
}

void Context::flush()
{
  m_ctx->Flush();
}

SwapChain::SwapChain(IDXGISwapChain* swapchain,
                     ID3D11RenderTargetView* rtv,
                     ID3D11SamplerState* sampler,
                     ID3D11BlendState* blender)
    : m_sampler(to_com_ptr(sampler)),
      m_blender(to_com_ptr(blender)),
      m_swapchain(to_com_ptr(swapchain)),
      m_rtv(to_com_ptr(rtv)),
      m_width(0),
      m_height(0)
{
}

void SwapChain::bind(const std::shared_ptr<Context>& ctx)
{
  m_ctx = ctx;
  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);

  ID3D11RenderTargetView* rtv[1] = {m_rtv.get()};
  d3d11_ctx->OMSetRenderTargets(1, rtv, nullptr);

  // Set default blending state.
  if (m_blender)
  {
    float factor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    d3d11_ctx->OMSetBlendState(m_blender.get(), factor, 0xffffffff);
  }

  // Set default sampler state.
  if (m_sampler)
  {
    ID3D11SamplerState* samplers[1] = {m_sampler.get()};
    d3d11_ctx->PSSetSamplers(0, 1, samplers);
  }
}

void SwapChain::unbind()
{
  m_ctx.reset();
}

void SwapChain::clear(float red, float green, float blue, float alpha)
{
  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);
  CHECK(d3d11_ctx);

  FLOAT color[4] = {red, green, blue, alpha};
  d3d11_ctx->ClearRenderTargetView(m_rtv.get(), color);
}

void SwapChain::present(int sync_interval)
{
  m_swapchain->Present(sync_interval, 0);
}

void SwapChain::resize(int width, int height)
{
  if (width <= 0 || height <= 0 || width == m_width || height == m_height)
  {
    return;
  }
  m_width = width;
  m_height = height;

  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);
  CHECK(d3d11_ctx);

  d3d11_ctx->OMSetRenderTargets(0, 0, 0);
  m_rtv.reset();

  DXGI_SWAP_CHAIN_DESC desc;
  m_swapchain->GetDesc(&desc);
  auto hr = m_swapchain->ResizeBuffers(0, width, height, desc.BufferDesc.Format,
                                      desc.Flags);
  if (FAILED(hr)) 
  {
    kodi::Log(ADDON_LOG_ERROR, "d3d11: Failed to resize swapchain (%ix%i)", width, height);
    return;
  }

  ID3D11Texture2D* buffer = nullptr;
  hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
  if (FAILED(hr))
  {
    kodi::Log(ADDON_LOG_ERROR, "d3d11: Failed to resize swapchain (%ix%i)", width, height);
    return;
  }

  ID3D11Device* dev = nullptr;
  d3d11_ctx->GetDevice(&dev);
  if (dev)
  {
    D3D11_RENDER_TARGET_VIEW_DESC vdesc = {};
    vdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    vdesc.Texture2D.MipSlice = 0;
    vdesc.Format = desc.BufferDesc.Format;

    ID3D11RenderTargetView* view = nullptr;
    hr = dev->CreateRenderTargetView(buffer, &vdesc, &view);
    if (SUCCEEDED(hr))
    {
      m_rtv = to_com_ptr(view);
      d3d11_ctx->OMSetRenderTargets(1, &view, nullptr);
    }
    dev->Release();
  }
  buffer->Release();

  D3D11_VIEWPORT vp;
  vp.Width = static_cast<float>(width);
  vp.Height = static_cast<float>(height);
  vp.MinDepth = D3D11_MIN_DEPTH;
  vp.MaxDepth = D3D11_MAX_DEPTH;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  d3d11_ctx->RSSetViewports(1, &vp);
}

Effect::Effect(ID3D11VertexShader* vsh,
               ID3D11PixelShader* psh,
               ID3D11InputLayout* layout)
  : m_vsh(to_com_ptr(vsh)),
    m_psh(to_com_ptr(psh)),
    m_layout(to_com_ptr(layout))
{
}

void Effect::bind(const std::shared_ptr<Context>& ctx)
{
  m_ctx = ctx;
  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);

  d3d11_ctx->IASetInputLayout(m_layout.get());
  d3d11_ctx->VSSetShader(m_vsh.get(), nullptr, 0);
  d3d11_ctx->PSSetShader(m_psh.get(), nullptr, 0);
}

void Effect::unbind() 
{
}

Geometry::Geometry(D3D_PRIMITIVE_TOPOLOGY primitive,
                   uint32_t vertices,
                   uint32_t stride,
                   ID3D11Buffer* buffer)
  : m_primitive(primitive),
    m_vertices(vertices),
    m_stride(stride),
    m_buffer(to_com_ptr(buffer))
{
}

void Geometry::bind(const std::shared_ptr<Context>& ctx) 
{
  m_ctx = ctx;
  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);

  // TODO: Handle offset.
  uint32_t offset = 0;

  ID3D11Buffer* buffers[1] = {m_buffer.get()};
  d3d11_ctx->IASetVertexBuffers(0, 1, buffers, &m_stride, &offset);
  d3d11_ctx->IASetPrimitiveTopology(m_primitive);
}

void Geometry::unbind() {}

void Geometry::draw() 
{
  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);
  CHECK(d3d11_ctx);

  // TODO: Handle offset.
  d3d11_ctx->Draw(m_vertices, 0);
}

Texture2D::Texture2D(ID3D11Texture2D* tex, ID3D11ShaderResourceView* srv)
  : m_texture(to_com_ptr(tex)), 
    m_srv(to_com_ptr(srv)) 
{
  m_share_handle = nullptr;

  IDXGIResource* res = nullptr;
  if (SUCCEEDED(m_texture->QueryInterface(__uuidof(IDXGIResource), reinterpret_cast<void**>(&res)))) 
  {
    res->GetSharedHandle(&m_share_handle);
    res->Release();
  }

  // Are we using a keyed mutex?
  IDXGIKeyedMutex* mutex = nullptr;
  if (SUCCEEDED(m_texture->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&mutex))) 
  {
    m_keyed_mutex = to_com_ptr(mutex);
  }
}

uint32_t Texture2D::width() const 
{
  D3D11_TEXTURE2D_DESC desc;
  m_texture->GetDesc(&desc);
  return desc.Width;
}

uint32_t Texture2D::height() const 
{
  D3D11_TEXTURE2D_DESC desc;
  m_texture->GetDesc(&desc);
  return desc.Height;
}

DXGI_FORMAT Texture2D::format() const 
{
  D3D11_TEXTURE2D_DESC desc;
  m_texture->GetDesc(&desc);
  return desc.Format;
}

bool Texture2D::has_mutex() const 
{
  return (m_keyed_mutex.get() != nullptr);
}

bool Texture2D::lock_key(uint64_t key, uint32_t timeout_ms) 
{
  if (m_keyed_mutex) 
  {
    const auto hr = m_keyed_mutex->AcquireSync(key, timeout_ms);
    return SUCCEEDED(hr);
  }
  return true;
}

void Texture2D::unlock_key(uint64_t key)
{
  if (m_keyed_mutex)
  {
    m_keyed_mutex->ReleaseSync(key);
  }
}

void Texture2D::bind(const std::shared_ptr<Context>& ctx) 
{
  m_ctx = ctx;
  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);
  if (m_srv) 
  {
    ID3D11ShaderResourceView* views[1] = {m_srv.get()};
    d3d11_ctx->PSSetShaderResources(0, 1, views);
  }
}

void Texture2D::unbind()
{
}

void* Texture2D::share_handle() const 
{
  return m_share_handle;
}

void Texture2D::copy_from(const std::shared_ptr<Texture2D>& other) 
{
  ID3D11DeviceContext* d3d11_ctx = (ID3D11DeviceContext*)(*m_ctx);
  CHECK(d3d11_ctx);
  if (other) 
  {
    d3d11_ctx->CopyResource(m_texture.get(), other->m_texture.get());
  }
}

Device::Device(ID3D11Device* pdev, ID3D11DeviceContext* pctx)
  : m_device(to_com_ptr(pdev)), m_ctx(std::make_shared<Context>(pctx)) 
{
  m_libCompiler = LoadLibrary(L"d3dcompiler_47.dll");
}

// static
std::shared_ptr<Device> Device::create() 
{
  ID3D11Device* device = nullptr;
  ID3D11DeviceContext* context = static_cast<ID3D11DeviceContext*>(kodi::gui::GetHWContext());
  context->GetDevice(&device);

  const auto dev = std::make_shared<Device>(device, context);

  kodi::Log(ADDON_LOG_INFO, "d3d11: Selected adapter '%s'", dev->adapter_name().c_str());

  return dev;
}

std::string Device::adapter_name() const 
{
  IDXGIDevice* dxgi_dev = nullptr;
  auto hr = m_device->QueryInterface(__uuidof(dxgi_dev), (void**)&dxgi_dev);
  if (SUCCEEDED(hr))
  {
    IDXGIAdapter* dxgi_adapt = nullptr;
    hr = dxgi_dev->GetAdapter(&dxgi_adapt);
    dxgi_dev->Release();
    if (SUCCEEDED(hr))
    {
      DXGI_ADAPTER_DESC desc;
      hr = dxgi_adapt->GetDesc(&desc);
      dxgi_adapt->Release();
      if (SUCCEEDED(hr))
      {
        return CefString(desc.Description);
      }
    }
  }

  return "n/a";
}

std::shared_ptr<Context> Device::immedidate_context() 
{
  return m_ctx;
}

std::shared_ptr<SwapChain> Device::create_swapchain(HWND window, int width, int height) 
{
  HRESULT hr;
  IDXGIFactory1* dxgi_factory = nullptr;

  // Default size to the window size unless specified.
  RECT rc_bounds;
  GetClientRect(window, &rc_bounds);
  if (width <= 0)
  {
    width = rc_bounds.right - rc_bounds.left;
  }
  if (height <= 0)
  {
    height = rc_bounds.bottom - rc_bounds.top;
  }

  {
    IDXGIDevice* dxgi_dev = nullptr;
    hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgi_dev));
    if (FAILED(hr))
    {
      return nullptr;
    }

    IDXGIAdapter* adapter = nullptr;
    hr = dxgi_dev->GetAdapter(&adapter);
    dxgi_dev->Release();
    if (FAILED(hr))
    {
      return nullptr;
    }

    hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgi_factory));
    adapter->Release();
  }

  if (!dxgi_factory)
  {
    return nullptr;
  }

  IDXGISwapChain* swapchain = nullptr;

  // Create swap chain.
  IDXGIFactory2* dxgi_factory2 = nullptr;
  hr = dxgi_factory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgi_factory2));
  if (dxgi_factory2)
  {
    DXGI_SWAP_CHAIN_DESC1 sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.Width = width;
    sd.Height = height;
    sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;

    IDXGISwapChain1* swapchain1 = nullptr;
    hr = dxgi_factory2->CreateSwapChainForHwnd(m_device.get(), window, &sd, nullptr, nullptr, &swapchain1);
    if (SUCCEEDED(hr))
    {
      hr = swapchain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&swapchain));
      swapchain1->Release();
    }

    dxgi_factory2->Release();
  }
  else
  {
    // DirectX 11.0 systems.
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    hr = dxgi_factory->CreateSwapChain(m_device.get(), &sd, &swapchain);
  }

  // We don't handle full-screen swapchains so we block the ALT+ENTER shortcut.
  dxgi_factory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER);
  dxgi_factory->Release();

  if (!swapchain)
  {
    return nullptr;
  }

  ID3D11Texture2D* back_buffer = nullptr;
  hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
  if (FAILED(hr))
  {
    swapchain->Release();
    return nullptr;
  }

  ID3D11RenderTargetView* rtv = nullptr;
  hr = m_device->CreateRenderTargetView(back_buffer, nullptr, &rtv);
  back_buffer->Release();
  if (FAILED(hr))
  {
    swapchain->Release();
    return nullptr;
  }

  const auto ctx = (ID3D11DeviceContext*)(*m_ctx);

  ctx->OMSetRenderTargets(1, &rtv, nullptr);

  // Setup the viewport.
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  ctx->RSSetViewports(1, &vp);

  // Create a default sampler to use.
  ID3D11SamplerState* sampler = nullptr;
  {
    D3D11_SAMPLER_DESC desc = {};
    desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    desc.MinLOD = 0.0f;
    desc.MaxLOD = D3D11_FLOAT32_MAX;
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    m_device->CreateSamplerState(&desc, &sampler);
  }

  // Create a default blend state to use (pre-multiplied alpha).
  ID3D11BlendState* blender = nullptr;
  {
    D3D11_BLEND_DESC desc;
    desc.AlphaToCoverageEnable = FALSE;
    desc.IndependentBlendEnable = FALSE;
    const auto count = sizeof(desc.RenderTarget) / sizeof(desc.RenderTarget[0]);
    for (size_t n = 0; n < count; ++n) {
      desc.RenderTarget[n].BlendEnable = TRUE;
      desc.RenderTarget[n].SrcBlend = D3D11_BLEND_ONE;
      desc.RenderTarget[n].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
      desc.RenderTarget[n].SrcBlendAlpha = D3D11_BLEND_ONE;
      desc.RenderTarget[n].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
      desc.RenderTarget[n].BlendOp = D3D11_BLEND_OP_ADD;
      desc.RenderTarget[n].BlendOpAlpha = D3D11_BLEND_OP_ADD;
      desc.RenderTarget[n].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    m_device->CreateBlendState(&desc, &blender);
  }

  return std::make_shared<SwapChain>(swapchain, rtv, sampler, blender);
}

std::shared_ptr<Geometry> Device::create_quad(float x,
                                              float y,
                                              float width,
                                              float height,
                                              bool flip)
{
  x = (x * 2.0f) - 1.0f;
  y = 1.0f - (y * 2.0f);
  width = width * 2.0f;
  height = height * 2.0f;
  float z = 1.0f;

  SimpleVertex vertices[] =
  {
    {DirectX::XMFLOAT3(x, y, z),                  DirectX::XMFLOAT2(0.0f, 0.0f)},
    {DirectX::XMFLOAT3(x + width, y, z),          DirectX::XMFLOAT2(1.0f, 0.0f)},
    {DirectX::XMFLOAT3(x, y - height, z),         DirectX::XMFLOAT2(0.0f, 1.0f)},
    {DirectX::XMFLOAT3(x + width, y - height, z), DirectX::XMFLOAT2(1.0f, 1.0f)}
  };

  if (flip)
  {
    DirectX::XMFLOAT2 tmp(vertices[2].tex);
    vertices[2].tex = vertices[0].tex;
    vertices[0].tex = tmp;

    tmp = vertices[3].tex;
    vertices[3].tex = vertices[1].tex;
    vertices[1].tex = tmp;
  }

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = sizeof(SimpleVertex) * 4;
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  desc.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA srd = {};
  srd.pSysMem = vertices;

  ID3D11Buffer* buffer = nullptr;
  const auto hr = m_device->CreateBuffer(&desc, &srd, &buffer);
  if (SUCCEEDED(hr))
  {
    return std::make_shared<Geometry>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, static_cast<uint32_t>(sizeof(SimpleVertex)), buffer);
  }

  return nullptr;
}

std::shared_ptr<Texture2D> Device::open_shared_texture(void* handle)
{
  ID3D11Texture2D* tex = nullptr;
  auto hr = m_device->OpenSharedResource(handle, __uuidof(ID3D11Texture2D), (void**)(&tex));
  if (FAILED(hr))
    return nullptr;

  D3D11_TEXTURE2D_DESC td;
  tex->GetDesc(&td);

  ID3D11ShaderResourceView* srv = nullptr;

  if (td.BindFlags & D3D11_BIND_SHADER_RESOURCE)
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = td.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    hr = m_device->CreateShaderResourceView(tex, &srvDesc, &srv);
    if (FAILED(hr))
    {
      tex->Release();
      return nullptr;
    }
  }

  return std::make_shared<Texture2D>(tex, srv);
}

std::shared_ptr<Texture2D> Device::create_texture(int width,
                                                  int height,
                                                  DXGI_FORMAT format,
                                                  const void* data,
                                                  size_t row_stride)
{
  D3D11_TEXTURE2D_DESC td;
  td.ArraySize = 1;
  td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  td.CPUAccessFlags = 0;
  td.Format = format;
  td.Width = width;
  td.Height = height;
  td.MipLevels = 1;
  td.MiscFlags = 0;
  td.SampleDesc.Count = 1;
  td.SampleDesc.Quality = 0;
  td.Usage = D3D11_USAGE_DEFAULT;

  D3D11_SUBRESOURCE_DATA srd;
  srd.pSysMem = data;
  srd.SysMemPitch = static_cast<uint32_t>(row_stride);
  srd.SysMemSlicePitch = 0;

  ID3D11Texture2D* tex = nullptr;
  auto hr = m_device->CreateTexture2D(&td, data ? &srd : nullptr, &tex);
  if (FAILED(hr))
    return nullptr;

  D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
  srv_desc.Format = td.Format;
  srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srv_desc.Texture2D.MostDetailedMip = 0;
  srv_desc.Texture2D.MipLevels = 1;

  ID3D11ShaderResourceView* srv = nullptr;
  hr = m_device->CreateShaderResourceView(tex, &srv_desc, &srv);
  if (FAILED(hr))
  {
    tex->Release();
    return nullptr;
  }

  return std::make_shared<Texture2D>(tex, srv);
}

std::shared_ptr<ID3DBlob> Device::compile_shader(const std::string& source_code,
                                                 const std::string& entry_point,
                                                 const std::string& model)
{
  if (!m_libCompiler)
  {
    return nullptr;
  }

  typedef HRESULT(WINAPI * PFN_D3DCOMPILE)(
      LPCVOID, SIZE_T, LPCSTR, const D3D_SHADER_MACRO*, ID3DInclude*, LPCSTR,
      LPCSTR, UINT, UINT, ID3DBlob**, ID3DBlob**);

  const auto fnc_compile = reinterpret_cast<PFN_D3DCOMPILE>(GetProcAddress(m_libCompiler, "D3DCompile"));
  if (!fnc_compile)
  {
    return nullptr;
  }

  DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(NDEBUG)
// flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
// flags |= D3DCOMPILE_AVOID_FLOW_CONTROL;
#else
  flags |= D3DCOMPILE_DEBUG;
  flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  ID3DBlob* blob = nullptr;
  ID3DBlob* blob_err = nullptr;

  const auto psrc = source_code.c_str();
  const auto len = source_code.size() + 1;

  const auto hr =
      fnc_compile(psrc, len, nullptr, nullptr, nullptr, entry_point.c_str(),
                  model.c_str(), flags, 0, &blob, &blob_err);

  if (FAILED(hr))
  {
    if (blob_err)
    {
      // TODO: Log the error.
      blob_err->Release();
    }
    return nullptr;
  }

  if (blob_err)
  {
    blob_err->Release();
  }

  return std::shared_ptr<ID3DBlob>(blob, [](ID3DBlob* p) {
    if (p)
      p->Release();
  });
}

std::shared_ptr<Effect> Device::create_default_effect()
{
  const auto vsh =
      R"--(struct VS_INPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = input.pos;
	output.tex = input.tex;
	return output;
})--";

  const auto psh =
      R"--(Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	return tex0.Sample(samp0, input.tex);
})--";

  return create_effect(vsh, "main", "vs_4_0", psh, "main", "ps_4_0");
}

std::shared_ptr<Effect> Device::create_effect(const std::string& vertex_code,
                                              const std::string& vertex_entry,
                                              const std::string& vertex_model,
                                              const std::string& pixel_code,
                                              const std::string& pixel_entry,
                                              const std::string& pixel_model)
{
  const auto vs_blob = compile_shader(vertex_code, vertex_entry, vertex_model);

  ID3D11VertexShader* vshdr = nullptr;
  ID3D11InputLayout* layout = nullptr;

  if (vs_blob)
  {
    m_device->CreateVertexShader(vs_blob->GetBufferPointer(),
                                vs_blob->GetBufferSize(), nullptr, &vshdr);

    D3D11_INPUT_ELEMENT_DESC layout_desc[] =
    {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
        D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    UINT elements = ARRAYSIZE(layout_desc);

    // Create the input layout.
    m_device->CreateInputLayout(layout_desc, elements,
                               vs_blob->GetBufferPointer(),
                               vs_blob->GetBufferSize(), &layout);
  }

  const auto ps_blob = compile_shader(pixel_code, pixel_entry, pixel_model);
  ID3D11PixelShader* pshdr = nullptr;
  if (ps_blob)
  {
    m_device->CreatePixelShader(ps_blob->GetBufferPointer(),
                               ps_blob->GetBufferSize(), nullptr, &pshdr);
  }

  return std::make_shared<Effect>(vshdr, pshdr, layout);
}

Layer::Layer(const std::shared_ptr<Device>& device, bool flip)
  : m_device(device), m_flip(flip)
{
  m_bounds.x = m_bounds.y = m_bounds.width = m_bounds.height = 0.0f;
}

Layer::~Layer()
{
}

void Layer::attach(const std::shared_ptr<Composition>& parent)
{
  m_composition = parent;
}

std::shared_ptr<Composition> Layer::composition() const
{
  return m_composition.lock();
}

Rect Layer::bounds() const
{
  return m_bounds;
}

void Layer::move(float x, float y, float width, float height)
{
  m_bounds.x = x;
  m_bounds.y = y;
  m_bounds.width = width;
  m_bounds.height = height;

  // It's not efficient to create the quad everytime we move, but for now we're
  // just trying to get something on-screen.
  m_geometry.reset();
}

void Layer::tick(double)
{
  // Nothing to update in the base class.
}

void Layer::render_texture(const std::shared_ptr<Context>& ctx,
                           const std::shared_ptr<Texture2D>& texture)
{
  if (!m_geometry)
  {
    m_geometry = m_device->create_quad(m_bounds.x, m_bounds.y, m_bounds.width,
                                     m_bounds.height, m_flip);
  }

  if (m_geometry && texture)
  {
    // We need a shader.
    if (!m_effect)
    {
      m_effect = m_device->create_default_effect();
    }

    // Bind our states/resource to the pipeline.
    ScopedBinder<Geometry> quad_binder(ctx, m_geometry);
    ScopedBinder<Effect> fx_binder(ctx, m_effect);
    ScopedBinder<Texture2D> tex_binder(ctx, texture);

    // Draw the quad.
    m_geometry->draw();
  }
}

Composition::Composition(const std::shared_ptr<Device>& device,
                         int width,
                         int height)
  : m_width(width), m_height(height), m_vsync(true), m_device(device)
{
}

void Composition::add_layer(const std::shared_ptr<Layer>& layer)
{
  if (layer)
  {
    m_layers.push_back(layer);

    // Attach ourselves as the parent.
    layer->attach(shared_from_this());
  }
}

bool Composition::remove_layer(const std::shared_ptr<Layer>& layer)
{
  size_t match = 0;
  if (layer)
  {
    for (auto i = m_layers.begin(); i != m_layers.end();)
    {
      if ((*i).get() == layer.get())
      {
        i = m_layers.erase(i);
        match++;
      }
      else
      {
        i++;
      }
    }
  }
  return (match > 0);
}

void Composition::resize(bool vsync, int width, int height)
{
  m_vsync = vsync;
  m_width = width;
  m_height = height;
}

void Composition::render(const std::shared_ptr<Context>& ctx) 
{
  // Use painter's algorithm and render our layers in order (not doing any dept
  // or 3D here).
  for (const auto& layer : m_layers)
    layer->render(ctx);
}

FrameBuffer::FrameBuffer(const std::shared_ptr<Device>& device)
  : m_device(device) 
{
}

void FrameBuffer::on_paint(void* shared_handle) 
{
  // Did the shared texture change?
  if (m_shared_buffer && shared_handle != m_shared_buffer->share_handle()) 
    m_shared_buffer.reset();

  // Open the shared texture.
  if (!m_shared_buffer) 
  {
    m_shared_buffer = m_device->open_shared_texture((void*)shared_handle);
    if (!m_shared_buffer)
      kodi::Log(ADDON_LOG_ERROR, "d3d11: Could not open shared texture!");
  }
}

}  // namespace d3d11
