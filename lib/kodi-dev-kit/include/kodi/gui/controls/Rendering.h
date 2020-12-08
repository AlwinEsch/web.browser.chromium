/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <string>

namespace kodi
{

using HardwareContext = void*;

namespace gui
{

class CWindow;

namespace controls
{

class CRendering
{
public:
  CRendering(CWindow* window, int controlId);
  ~CRendering();

  virtual bool Create(int x, int y, int w, int h, kodi::HardwareContext device) { return false; }
  virtual void Render() {}
  virtual void Stop() {}
  virtual bool Dirty() { return false; }

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
