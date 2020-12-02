/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "c-api/addon_base.h"
#include "versions.h"

#include <memory>
#include <msgpack.hpp>
#include <string>

namespace kodi
{

class CSettingValue
{
public:
  explicit CSettingValue(const std::string settingValue) : m_settingValue(settingValue) {}

  bool empty() const { return m_settingValue.empty(); }

  std::string GetString() const { return m_settingValue; }
  int32_t GetInt() const { return std::stoi(m_settingValue); }
  uint32_t GetUInt() const { return uint32_t(std::stoi(m_settingValue)); }
  bool GetBoolean() const { return m_settingValue == "1" | m_settingValue == "true"; }
  float GetFloat() const { return std::stof(m_settingValue); }
  template<typename enumType>
  enumType GetEnum() const
  {
    return static_cast<enumType>(std::stoi(m_settingValue));
  }

private:
  const std::string m_settingValue;
};

namespace gui
{
struct IRenderHelper;
} /* namespace gui */

using HardwareContext = ADDON_HARDWARE_CONTEXT;

namespace addon
{

class IAddonInstance
{
public:
  explicit IAddonInstance(ADDON_TYPE type, const std::string& version)
    : m_type(type), m_kodiVersion(version)
  {
  }
  virtual ~IAddonInstance() = default;

  virtual ADDON_STATUS CreateInstance(int instanceType,
                                      const std::string& instanceID,
                                      KODI_HANDLE instance,
                                      const std::string& version,
                                      KODI_HANDLE& addonInstance)
  {
    return ADDON_STATUS_NOT_IMPLEMENTED;
  }

  const ADDON_TYPE m_type;
  const std::string m_kodiVersion;
  std::string m_id;
};

class CAddonBase
{
public:
  CAddonBase();
  virtual ~CAddonBase();

  virtual ADDON_STATUS Create() { return ADDON_STATUS_OK; }
  virtual ADDON_STATUS SetSetting(const std::string& settingName,
                                  const kodi::CSettingValue& settingValue)
  {
    return ADDON_STATUS_UNKNOWN;
  }

  virtual ADDON_STATUS CreateInstance(int instanceType,
                                      const std::string& instanceID,
                                      KODI_HANDLE instance,
                                      const std::string& version,
                                      KODI_HANDLE& addonInstance)
  {
    return ADDON_STATUS_NOT_IMPLEMENTED;
  }

  virtual void DestroyInstance(int instanceType,
                               const std::string& instanceID,
                               KODI_HANDLE addonInstance)
  {
  }

  std::shared_ptr<kodi::gui::IRenderHelper> m_renderHelper;

private:
  uint64_t m_handle;
};

} /* namespace addon */
} /* namespace kodi */
