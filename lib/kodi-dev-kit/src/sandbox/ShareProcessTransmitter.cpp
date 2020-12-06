/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "ShareProcessTransmitter.h"
#include "ShareProcessReceiver.h"
#include "SharedMemControlPosixMMAP.h"

namespace kodi
{
namespace sandbox
{

CShareProcessTransmitter::CShareProcessTransmitter(const std::string& identifier, bool child, bool mainThread, size_t size /* = SHARED_DEFAULT_MEM_SIZE*/)
  : m_identifier(identifier),
    m_child(child),
    m_mainThread(mainThread)
{
  m_memControl = std::make_shared<CSharedMemControlPosix>(child, identifier, size);
}

bool CShareProcessTransmitter::Create(bool initial)
{
  if (!m_memControl->Create(initial))
    return false;

  return true;
}

void CShareProcessTransmitter::Destroy()
{
  m_memControl->Destroy();
}

void CShareProcessTransmitter::SendMessage(const msgpack::sbuffer& in)
{
  std::unique_lock<std::mutex> lock(m_lock);

  apiShareData* sharedMem = m_memControl->GetSharedMem();

  size_t size = in.size();
  sharedMem->bigger = size > DEFAULT_SHARED_MEM_ARRAY_SIZE;
  sharedMem->data_size = sharedMem->bigger ? DEFAULT_SHARED_MEM_ARRAY_SIZE : size;
  sharedMem->main_thread = m_mainThread;
  memcpy(sharedMem->data, in.data(), sharedMem->data_size);
  m_active = true;
back:
  m_memControl->Unlock_Target();
  m_memControl->Lock_Caller();

  if (m_mainThreadReceive)
  {
    m_mainReceiver->ProcessMemory(m_mainReceiveSharedMem);
    goto back;
  }
  m_active = false;
}

void CShareProcessTransmitter::SendMessage(const msgpack::sbuffer& in, msgpack::sbuffer& ret)
{
  std::unique_lock<std::mutex> lock(m_lock);
  msgpack::unpacked ident = msgpack::unpack(in.data(), in.size());

  apiShareData* sharedMem = m_memControl->GetSharedMem();

  size_t size = in.size();
  sharedMem->bigger = size > DEFAULT_SHARED_MEM_ARRAY_SIZE;
  sharedMem->data_size = sharedMem->bigger ? DEFAULT_SHARED_MEM_ARRAY_SIZE : size;
  sharedMem->main_thread = m_mainThread;
  memcpy(sharedMem->data, in.data(), sharedMem->data_size);
  m_active = true;
back:
  m_memControl->Unlock_Target();
  m_memControl->Lock_Caller();

  if (m_mainThreadReceive)
  {
    m_mainReceiver->ProcessMemory(m_mainReceiveSharedMem);
    goto back;
  }
  m_active = false;
  ret.write(sharedMem->data, sharedMem->data_size);
}

bool CShareProcessTransmitter::ProcessMainThreadReceive(apiShareData* sharedMem, CShareProcessReceiver* receiver)
{
  if (m_mainThreadReceive)
  {
    fprintf(stderr, "FATAL: A main thread call should never be called twice at same time!\n");
    return false;
  }

  m_mainThreadReceive = true;
  m_mainReceiveSharedMem = sharedMem;
  m_mainReceiver = receiver;

  m_memControl->Unlock_Caller();
  m_memControl->Lock_Target();

  m_mainThreadReceive = false;
  return true;
}

} /* namespace sandbox */
} /* namespace kodi */
