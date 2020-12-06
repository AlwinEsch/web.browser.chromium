/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "ShareProcessReceiver.h"
#include "ShareProcessTransmitter.h"
#include "SharedMemControlPosixMMAP.h"

namespace kodi
{
namespace sandbox
{

CShareProcessReceiver::CShareProcessReceiver(const std::string& identifier, std::shared_ptr<CShareProcessTransmitter> mainTransmitter, bool child, size_t size /* = SHARED_DEFAULT_MEM_SIZE*/)
  : m_child(child),
    m_mainTransmitter(mainTransmitter)
{
  m_memControl = std::make_shared<CSharedMemControlPosix>(child, identifier, size);
}

bool CShareProcessReceiver::Create(bool initial, bool useThread)
{
  if (!m_memControl->Create(initial))
  {
    fprintf(stderr, "Failed to reciever, error '%s'\n", m_memControl->GetLastErrorText().c_str());
    return false;
  }

  m_running = true;
  if (useThread)
    m_thread = std::thread([&]{ Process(); });

  return true;
}

void CShareProcessReceiver::Destroy()
{
  if (!m_running)
    return;

  m_running = false;
  m_memControl->Unlock_Target();

  if (m_thread.joinable())
  {
    m_thread.join();
  }

  m_memControl->Destroy();
}

void CShareProcessReceiver::RegisterReceiver(SandboxProcess func)
{
  m_receivers.emplace_back(func);
}

bool CShareProcessReceiver::HandleMessage(int group, int func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                                          const char* data, std::size_t size, std::size_t offset)
{
  bool ret = false;
  m_memControl->SetActive();

  out.clear();

  for (const auto& receiver : m_receivers)
  {
    if (receiver(group, func, in, out, data, size, offset))
    {
      ret = true;
      break;
    }
  }

  m_memControl->SetInactive();

  return ret;
}

bool CShareProcessReceiver::ProcessMemory(apiShareData* sharedMem)
{
  msgpack::sbuffer ret;

  std::size_t offset = 0;
  msgpack::unpacked ident = msgpack::unpack(sharedMem->data, sharedMem->data_size, offset);
  msgIdentifier id = ident.get().as<decltype(id)>();

  auto input = msgpack::unpack(sharedMem->data, sharedMem->data_size, offset);
  if (!HandleMessage(id.first, id.second, input, ret, sharedMem->data, sharedMem->data_size, offset))
  {
    return true;
  }

  sharedMem->data_size = ret.size();
  memcpy(sharedMem->data, ret.data(), sharedMem->data_size);
  return true;
}

bool CShareProcessReceiver::ProcessOutside()
{
  bool ret = false;

  if (m_memControl->Lock_Target())
  {
    if (!m_running)
    {
      return false;
    }

    apiShareData* sharedMem = m_memControl->GetSharedMem();

    if (!m_child && m_mainTransmitter->IsActive() && sharedMem->main_thread)
    {
      ret = m_mainTransmitter->ProcessMainThreadReceive(sharedMem, this);
    }
    else
    {
      ret = ProcessMemory(sharedMem);
    }
  }

  m_memControl->Unlock_Caller();
  return ret;
}


void CShareProcessReceiver::Process()
{
  while (m_running)
  {
    ProcessOutside();
  }
}

} /* namespace sandbox */
} /* namespace kodi */
