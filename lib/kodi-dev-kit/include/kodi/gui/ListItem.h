/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <memory>
#include <string>

namespace kodi
{
namespace gui
{

class CWindow;

class CListItem
{
public:

  CListItem(const std::string& label = "",
            const std::string& label2 = "",
            const std::string& path = "");
  explicit CListItem(uint64_t listItemHandle) : m_controlHandle(listItemHandle)
  {
  }
  virtual ~CListItem();

  std::string GetLabel() const;
  void SetLabel(const std::string& label);
  std::string GetLabel2() const;
  void SetLabel2(const std::string& label);
  std::string GetArt(const std::string& type) const;
  void SetArt(const std::string& type, const std::string& url);
  std::string GetPath() const;
  void SetPath(const std::string& path);
  std::string GetProperty(const std::string& key) const;
  void SetProperty(const std::string& key, const std::string& value);
  void Select(bool selected);
  bool IsSelected() const;

private:
  friend class CWindow;

  uint64_t m_controlHandle;
};

} /* namespace gui */
} /* namespace kodi */
