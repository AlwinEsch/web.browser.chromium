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
namespace gui
{

class CWindow;

namespace controls
{

class CImage
{
public:
  CImage(CWindow* window, int controlId);
  ~CImage();

  void SetVisible(bool visible);
  void SetFileName(const std::string& filename, bool useCache = true);
  void SetColorDiffuse(uint32_t colorDiffuse);

private:
  uint64_t m_handle;
  uint64_t m_handleWindow;
};

} /* namespace controls */
} /* namespace gui */
} /* namespace kodi */
