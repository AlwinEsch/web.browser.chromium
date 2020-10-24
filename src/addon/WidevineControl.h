/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <chrono>
#include <kodi/tools/DllHelper.h>
#include <string>

class CWebBrowser;

class ATTRIBUTE_HIDDEN CWidewineControl : public kodi::tools::CDllHelper
{
public:
  CWidewineControl(CWebBrowser& instance) : m_instance(instance) {}
  ~CWidewineControl() override = default;

  bool InitializeWidevine();
  void DeinitializeWidevine();

  bool CryticalInstallDone() const { return m_cryticalInstallDone; }

private:
  bool CreateManifestAndInstall(bool force);

  inline bool ends_with(std::string const& value, std::string const& ending)
  {
    if (ending.size() > value.size())
      return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
  }

  const char* (*GetCdmVersion)() = nullptr;

  bool m_cryticalInstallDone = false;
  std::chrono::system_clock::time_point m_installTime;
  CWebBrowser& m_instance;
};
