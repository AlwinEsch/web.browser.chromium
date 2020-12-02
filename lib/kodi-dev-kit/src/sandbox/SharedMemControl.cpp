/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "SharedMemControl.h"

namespace kodi
{
namespace sandbox
{

CSharedMemControl::CSharedMemControl(const std::string& identifier,
                                     size_t size /* = DEFAULT_SHARED_MEM_SIZE*/)
  : m_identifier(identifier),
    m_sharedMemSize(size)
{
}

void CSharedMemControl::SetActive()
{
  m_active = true;
}

void CSharedMemControl::SetInactive()
{
  m_active = false;
}

} /* namespace sandbox */
} /* namespace kodi */
