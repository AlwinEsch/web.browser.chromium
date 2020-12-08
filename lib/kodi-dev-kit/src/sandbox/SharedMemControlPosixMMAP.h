/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "SharedMemControl.h"

namespace kodi
{
namespace sandbox
{

class CSharedMemControlPosix : public CSharedMemControl
{
public:
  CSharedMemControlPosix(bool child, const std::string& identifier, size_t size = SHARED_DEFAULT_MEM_SIZE);
  ~CSharedMemControlPosix() override;

  bool Create(bool initial) override;
  bool Destroy() override;

  bool Lock_Caller() override;
  void Unlock_Caller() override;

  bool Lock_Target() override;
  void Unlock_Target() override;

private:
  int m_fd{-1};
  bool m_creator{false};
};

} /* namespace sandbox */
} /* namespace kodi */
