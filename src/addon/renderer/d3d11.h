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

#pragma once

#include <d3d11_1.h>

#include <memory>
#include <string>
#include <vector>

#include "include/base/cef_macros.h"

namespace d3d11 
{

class Composition;
class Context;
class Effect;
class Geometry;
class SwapChain;
class Texture2D;

// Basic rect for floats.
struct Rect 
{
  float x;
  float y;
  float width;
  float height;
};

template <class T>
class ScopedBinder 
{
 public:
  ScopedBinder(const std::shared_ptr<Context>& ctx, const std::shared_ptr<T>& target)
    : m_target(target) 
  {
    if (m_target) 
    {
      m_target->bind(ctx);
    }
  }
  ~ScopedBinder() 
  {
    if (m_target)
    {
      m_target->unbind();
    }
  }

 private:
  const std::shared_ptr<T> m_target;

  DISALLOW_COPY_AND_ASSIGN(ScopedBinder);
};

class Context 
{
 public:
  Context(ID3D11DeviceContext*);

  void flush();

  operator ID3D11DeviceContext*() { return m_ctx.get(); }

 private:
  const std::shared_ptr<ID3D11DeviceContext> m_ctx;
};

// Encapsulate a D3D11 Device object.
class Device 
{
 public:
  Device(ID3D11Device*, ID3D11DeviceContext*);

  std::string adapter_name() const;

  operator ID3D11Device*() { return m_device.get(); }

  static std::shared_ptr<Device> create();

  std::shared_ptr<Context> immedidate_context();
  std::shared_ptr<SwapChain> create_swapchain(HWND, int width = 0, int height = 0);
  std::shared_ptr<Geometry> create_quad(float x, float y, float width, float height, bool flip = false);
  std::shared_ptr<Texture2D> create_texture(int width, int height, DXGI_FORMAT format, const void* data, size_t row_stride);
  std::shared_ptr<Texture2D> open_shared_texture(void*);

  // Create some basic shaders so we can draw a textured-quad.
  std::shared_ptr<Effect> create_default_effect();
  std::shared_ptr<Effect> create_effect(const std::string& vertex_code,
                                        const std::string& vertex_entry,
                                        const std::string& vertex_model,
                                        const std::string& pixel_code,
                                        const std::string& pixel_entry,
                                        const std::string& pixel_model);

 private:
  std::shared_ptr<ID3DBlob> compile_shader(const std::string& source_code, const std::string& entry_point, const std::string& model);

  HMODULE m_libCompiler;

  const std::shared_ptr<ID3D11Device> m_device;
  const std::shared_ptr<Context> m_ctx;

  DISALLOW_COPY_AND_ASSIGN(Device);
};

// Encapsulate a DXGI swapchain for a window.
class SwapChain 
{
 public:
  SwapChain(IDXGISwapChain*,
            ID3D11RenderTargetView*,
            ID3D11SamplerState*,
            ID3D11BlendState*);

  void bind(const std::shared_ptr<Context>& ctx);
  void unbind();

  void clear(float red, float green, float blue, float alpha);

  void present(int sync_interval);
  void resize(int width, int height);

  int width() const { return m_width; }
  int height() const { return m_height; }

 private:
  const std::shared_ptr<ID3D11SamplerState> m_sampler;
  const std::shared_ptr<ID3D11BlendState> m_blender;
  const std::shared_ptr<IDXGISwapChain> m_swapchain;
  std::shared_ptr<ID3D11RenderTargetView> m_rtv;
  std::shared_ptr<Context> m_ctx;
  int m_width;
  int m_height;

  DISALLOW_COPY_AND_ASSIGN(SwapChain);
};

class Texture2D 
{
 public:
  Texture2D(ID3D11Texture2D* tex, ID3D11ShaderResourceView* srv);

  void bind(std::shared_ptr<Context> const& ctx);
  void unbind();

  uint32_t width() const;
  uint32_t height() const;
  DXGI_FORMAT format() const;

  bool has_mutex() const;

  bool lock_key(uint64_t key, uint32_t timeout_ms);
  void unlock_key(uint64_t key);

  void* share_handle() const;

  void copy_from(const std::shared_ptr<Texture2D>&);

 private:
  HANDLE m_share_handle;

  const std::shared_ptr<ID3D11Texture2D> m_texture;
  const std::shared_ptr<ID3D11ShaderResourceView> m_srv;
  std::shared_ptr<IDXGIKeyedMutex> m_keyed_mutex;
  std::shared_ptr<Context> m_ctx;

  DISALLOW_COPY_AND_ASSIGN(Texture2D);
};

class Effect 
{
 public:
  Effect(ID3D11VertexShader* vsh,
         ID3D11PixelShader* psh,
         ID3D11InputLayout* layout);

  void bind(const std::shared_ptr<Context>& ctx);
  void unbind();

 private:
  const std::shared_ptr<ID3D11VertexShader> m_vsh;
  const std::shared_ptr<ID3D11PixelShader> m_psh;
  const std::shared_ptr<ID3D11InputLayout> m_layout;
  std::shared_ptr<Context> m_ctx;

  DISALLOW_COPY_AND_ASSIGN(Effect);
};

class Geometry 
{
 public:
  Geometry(D3D_PRIMITIVE_TOPOLOGY primitive,
           uint32_t vertices,
           uint32_t stride,
           ID3D11Buffer*);

  void bind(const std::shared_ptr<Context>& ctx);
  void unbind();

  void draw();

 private:
  D3D_PRIMITIVE_TOPOLOGY m_primitive;
  uint32_t m_vertices;
  uint32_t m_stride;
  const std::shared_ptr<ID3D11Buffer> m_buffer;
  std::shared_ptr<Context> m_ctx;

  DISALLOW_COPY_AND_ASSIGN(Geometry);
};

// Abstraction for a 2D layer within a composition.
class Layer 
{
 public:
  Layer(const std::shared_ptr<Device>& device, bool flip);
  virtual ~Layer();

  void attach(const std::shared_ptr<Composition>&);

  // Uses normalized 0-1.0 coordinates.
  virtual void move(float x, float y, float width, float height);

  virtual void tick(double t);
  virtual void render(const std::shared_ptr<Context>& ctx) = 0;

  Rect bounds() const;

  std::shared_ptr<Composition> composition() const;

 protected:
  // Helper method for derived classes to draw a textured-quad.
  void render_texture(const std::shared_ptr<Context>& ctx,
                      const std::shared_ptr<Texture2D>& texture);

  const std::shared_ptr<Device> m_device;
  const bool m_flip;

  Rect m_bounds;
  std::shared_ptr<Geometry> m_geometry;
  std::shared_ptr<Effect> m_effect;

 private:
  std::weak_ptr<Composition> m_composition;

  DISALLOW_COPY_AND_ASSIGN(Layer);
};

// A collection of layers. Will render 1-N layers to a D3D11 device.
class Composition : public std::enable_shared_from_this<Composition> {
 public:
  Composition(const std::shared_ptr<Device>& device,
              int width = 0,
              int height = 0);

  int width() const { return m_width; }
  int height() const { return m_height; }

  bool is_vsync() const;
  void render(const std::shared_ptr<Context>&);

  void add_layer(const std::shared_ptr<Layer>& layer);
  bool remove_layer(const std::shared_ptr<Layer>& layer);
  void resize(bool vsync, int width, int height);

 private:
  int m_width;
  int m_height;
  bool m_vsync;

  const std::shared_ptr<Device> m_device;
  std::vector<std::shared_ptr<Layer>> m_layers;

  DISALLOW_COPY_AND_ASSIGN(Composition);
};

class FrameBuffer
{
 public:
  explicit FrameBuffer(const std::shared_ptr<Device>& device);

  // Called in response to CEF's OnAcceleratedPaint notification.
  void on_paint(void* shared_handle);

  // Returns what should be considered the front buffer.
  std::shared_ptr<Texture2D> texture() const { return m_shared_buffer; }

 private:
  const std::shared_ptr<Device> m_device;
  std::shared_ptr<Texture2D> m_shared_buffer;

  DISALLOW_COPY_AND_ASSIGN(FrameBuffer);
};

}  // namespace d3d11
