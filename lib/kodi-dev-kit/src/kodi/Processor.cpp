/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../include/kodi/Processor.h"

#include "../../include/kodi/General.h"
#include "../../include/kodi/gui/Window.h"
#include "../../include/kodi/gui/controls/Rendering.h"
#include "../sandbox/ShareProcessReceiver.h"
#include "../sandbox/ShareProcessTransmitter.h"
#include "../sandbox/kodi/Processor.h"
#include "../sandbox/kodi/gui/Window.h"
#include "../sandbox/kodi/gui/controls/Rendering.h"
#include "../sandbox/shared_process_func_child.h"
#include "../sandbox/shared_process_func_group.h"
#include "../sandbox/shared_process_func_parent.h"
#include "../sandbox/IMsgHdl.h"

#include "AddonBaseInternal.h"
#include "addon-instance/Web.h"

namespace kodi
{
namespace sandbox
{

std::string CheckSandbox(int argc, char* argv[])
{
  const char* value = "--sandbox=";
  std::string sandbox;

  for (int i = 1; i < argc; ++i)
  {
    if (argv[i] == nullptr)
      break;

    std::string arg = argv[i];
    if (arg.rfind(value, 0) == 0)
    {
      sandbox = arg.substr(strlen(value));
      break;
    }
    else if ((arg == "-r") || (arg == "--sandbox"))
    {
      if (i + 1 < argc) // Make sure we aren't at the end of argv!
      {
        sandbox = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
        break;
      }
    }
  }

  return sandbox;
}

std::string CheckMainShared(int argc, char* argv[])
{
  const char* value = "--main-shared=";
  std::string main_shared;

  for (int i = 1; i < argc; ++i)
  {
    if (argv[i] == nullptr)
      break;

    std::string arg = argv[i];
    if (arg.rfind(value, 0) == 0)
    {
      main_shared = arg.substr(strlen(value));
      break;
    }
    else if ((arg == "-m") || (arg == "--main-shared"))
    {
      if (i + 1 < argc) // Make sure we aren't at the end of argv!
      {
        main_shared = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
        break;
      }
    }
  }

  return main_shared;
}

uint64_t CheckBaseHandle(int argc, char* argv[])
{
  const char* value = "--basehandle=";
  std::string base_handle;

  for (int i = 1; i < argc; ++i)
  {
    if (argv[i] == nullptr)
      break;

    std::string arg = argv[i];
    if (arg.rfind(value, 0) == 0)
    {
      base_handle = arg.substr(strlen(value));
      break;
    }
    else if ((arg == "-b") || (arg == "--basehandle"))
    {
      if (i + 1 < argc) // Make sure we aren't at the end of argv!
      {
        base_handle = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
        break;
      }
    }
  }

  return strtoll(base_handle.c_str(), nullptr, 16);
}

std::thread::id main_thread_id;
std::array<IMsgHdl*, funcGroup_Max> message_handlers = {nullptr};
std::shared_ptr<::kodi::sandbox::CShareProcessReceiver> CChildProcessor::g_mainIn;
std::shared_ptr<::kodi::sandbox::CShareProcessReceiver> CChildProcessor::g_otherIn;
std::shared_ptr<::kodi::sandbox::CShareProcessReceiver> CChildProcessor::g_otherIn2;
std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> CChildProcessor::g_mainOut;
std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> CChildProcessor::g_otherOut;
std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> CChildProcessor::g_otherOut2;
std::mutex CChildProcessor::g_lock;

CChildProcessor::CChildProcessor(const std::string& main_shared, bool viaMainThread)
  : m_mainShared(main_shared)
{
  main_thread_id = std::this_thread::get_id();

  std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> transmitter =
      std::make_shared<::kodi::sandbox::CShareProcessTransmitter>(m_mainShared, true, true);
  if (!transmitter->Create(false))
  {
    fprintf(stderr,
            "FATAL: Failed to init main transmit process of sandbox, process not usable!\n");
    exit(EXIT_FAILURE);
  }

  g_mainOut = transmitter;

  std::shared_ptr<::kodi::sandbox::CShareProcessReceiver> receiver =
      std::make_shared<::kodi::sandbox::CShareProcessReceiver>(m_mainShared + "-receiver", g_mainOut, true);
  receiver->RegisterReceiver(
      std::bind(&CChildProcessor::HandleMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                std::placeholders::_5, std::placeholders::_6, std::placeholders::_7));
  if (!receiver->Create(false, !viaMainThread))
  {
    fprintf(stderr, "FATAL: Failed to init main receive process of sandbox, process not usable!\n");
    exit(EXIT_FAILURE);
  }

  g_mainIn = receiver;

  message_handlers[funcGroup_AddonBase_h] = new child::C_AddonBase_h(*this);
  message_handlers[funcGroup_addoninstance_Web_h] = new child::C_addoninstance_Web_h(*this);

  ::kodi::Log(ADDON_LOG_INFO, "Inform for start of sandbox process id '%s'", m_mainShared.c_str());
}

CChildProcessor::~CChildProcessor()
{
  for (auto& handler : message_handlers)
    delete handler;

  g_mainOut.reset();
  g_otherOut.reset();
  g_otherOut2.reset();
}

bool CChildProcessor::ProcessOutside()
{
  if (!g_mainIn || !m_active)
    return false;

  return g_mainIn->ProcessOutside();
}

std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> CChildProcessor::GetCurrentProcessor()
{
  // BIG TODO in this work way!!!
  std::thread::id threadId = std::this_thread::get_id();

  if (main_thread_id == threadId)
  {
    return g_mainOut;
  }

  std::unique_lock<std::mutex> lock(g_lock);

  if (!g_otherOut)
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(in, msgIdentifier(funcGroup_Main, kodi_processor_CreateForNewThread));
    msgpack::pack(in, msgParent__IN_kodi_processor_CreateForNewThread("other"));
    g_mainOut->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgParent_OUT_kodi_processor_CreateForNewThread t = ident.get().as<decltype(t)>();

    std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> transmitter =
        std::make_shared<::kodi::sandbox::CShareProcessTransmitter>(std::get<0>(t), true, false);
    if (!transmitter->Create(false))
    {
      fprintf(stderr, "FATAL: Failed to init other process of sandbox, process not usable!\n");
      return g_mainOut;
    }

    g_otherOut = transmitter;
  }
  else if (g_otherOut->IsActive())
  {
    if (!g_otherOut2)
    {
      msgpack::sbuffer in;
      msgpack::sbuffer out;
      msgpack::pack(in, msgIdentifier(funcGroup_Main, kodi_processor_CreateForNewThread2));
      msgpack::pack(in, msgParent__IN_kodi_processor_CreateForNewThread("other2"));
      g_mainOut->SendMessage(in, out);
      msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
      msgParent_OUT_kodi_processor_CreateForNewThread t = ident.get().as<decltype(t)>();

      std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> transmitter =
          std::make_shared<::kodi::sandbox::CShareProcessTransmitter>(std::get<0>(t), true, false);
      if (!transmitter->Create(false))
      {
        fprintf(stderr, "FATAL: Failed to init other process of sandbox, process not usable!\n");
        return g_mainOut;
      }

      g_otherOut2 = transmitter;
    }

    return g_otherOut2;
  }

  return g_otherOut;
}

bool CChildProcessor::HandleMessage(uint32_t group,
                                    uint32_t func,
                                    const msgpack::unpacked& in,
                                    msgpack::sbuffer& out,
                                    const char* data,
                                    std::size_t size,
                                    std::size_t offset)
{
  if (message_handlers[group])
    return message_handlers[group]->HandleMessage(group, func, in, out,data, size, offset);

  if (group == funcGroup_Main)
  {
    switch (func)
    {
      case kodi_processor_KillChild:
      {
        ::kodi::Log(ADDON_LOG_INFO, "Informed for stop of sandbox process id '%s'",
                    m_mainShared.c_str());
        m_active = false;
        g_otherOut.reset();
        g_otherOut2.reset();
        return false;
      }
      case kodi_processor_CreateForNewChildThread:
      {
        msgParent__IN_kodi_processor_CreateForNewThread t = in.get().as<decltype(t)>();
        const std::string identifier = m_mainShared + "-subthread-child-" + std::get<0>(t);

        g_otherIn = std::make_shared<CShareProcessReceiver>(identifier, g_mainOut, true);
        g_otherIn->RegisterReceiver(
            std::bind(&CChildProcessor::HandleMessage, this, std::placeholders::_1,
                      std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                      std::placeholders::_5, std::placeholders::_6, std::placeholders::_7));
        if (!g_otherIn->Create(true, true))
        {
          kodi::Log(ADDON_LOG_ERROR,
                                  "CProcessReceiver::%s: Failed to create other second receiver",
                                  __func__);
          return false;
        }

        msgpack::pack(out, msgParent_OUT_kodi_processor_CreateForNewThread(identifier));
        return true;
      }
      case kodi_processor_CreateForNewChildThread2:
      {
        msgParent__IN_kodi_processor_CreateForNewThread t = in.get().as<decltype(t)>();
        const std::string identifier = m_mainShared + "-subthread-child-" + std::get<0>(t);

        g_otherIn2 = std::make_shared<CShareProcessReceiver>(identifier, g_mainOut, true);
        g_otherIn2->RegisterReceiver(
            std::bind(&CChildProcessor::HandleMessage, this, std::placeholders::_1,
                      std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                      std::placeholders::_5, std::placeholders::_6, std::placeholders::_7));
        if (!g_otherIn2->Create(true, true))
        {
          kodi::Log(ADDON_LOG_ERROR,
                                  "CProcessReceiver::%s: Failed to create other second receiver",
                                  __func__);
          return false;
        }

        msgpack::pack(out, msgParent_OUT_kodi_processor_CreateForNewThread(identifier));
        return true;
      }
      default:
        break;
    }
  }
  else if (group == funcGroup_gui_Window_h)
  {
    switch (func)
    {
      case kodi_gui_CWindow_OnInit:
      {
        msgChild__IN_kodi_gui_CWindow_OnInit t = in.get().as<decltype(t)>();
        ::kodi::gui::CWindow* window = reinterpret_cast<::kodi::gui::CWindow*>(std::get<0>(t));
        if (window == nullptr)
          break;

        msgpack::pack(out, msgChild_OUT_kodi_gui_CWindow_OnInit(window->OnInit()));
        return true;
      }
      case kodi_gui_CWindow_OnFocus:
      {
        msgChild__IN_kodi_gui_CWindow_OnFocus t = in.get().as<decltype(t)>();
        ::kodi::gui::CWindow* window = reinterpret_cast<::kodi::gui::CWindow*>(std::get<0>(t));
        if (window == nullptr)
          break;

        msgpack::pack(out, msgChild_OUT_kodi_gui_CWindow_OnFocus(window->OnFocus(std::get<1>(t))));
        return true;
      }
      case kodi_gui_CWindow_OnClick:
      {
        msgChild__IN_kodi_gui_CWindow_OnClick t = in.get().as<decltype(t)>();
        ::kodi::gui::CWindow* window = reinterpret_cast<::kodi::gui::CWindow*>(std::get<0>(t));
        if (window == nullptr)
          break;

        msgpack::pack(out, msgChild_OUT_kodi_gui_CWindow_OnClick(window->OnClick(std::get<1>(t))));
        return true;
      }
      case kodi_gui_CWindow_OnAction:
      {
        msgChild__IN_kodi_gui_CWindow_OnAction t = in.get().as<decltype(t)>();
        ::kodi::gui::CWindow* window = reinterpret_cast<::kodi::gui::CWindow*>(std::get<0>(t));
        if (window == nullptr)
          break;

        msgpack::pack(out,
                      msgChild_OUT_kodi_gui_CWindow_OnAction(window->OnAction(std::get<1>(t))));
        return true;
      }
      case kodi_gui_CWindow_GetContextButtons:
      {
        msgChild__IN_kodi_gui_CWindow_GetContextButtons t = in.get().as<decltype(t)>();
        ::kodi::gui::CWindow* window = reinterpret_cast<::kodi::gui::CWindow*>(std::get<0>(t));
        if (window == nullptr)
          break;

        std::vector<std::pair<unsigned int, std::string>>& buttons = std::get<2>(t);
        window->GetContextButtons(std::get<1>(t), buttons);
    for (const auto& a : buttons)
      fprintf(stderr, "XXX----------------- %s\n", a.second.c_str());
        msgpack::pack(out, msgChild_OUT_kodi_gui_CWindow_GetContextButtons(buttons));
        return true;
      }
      case kodi_gui_CWindow_OnContextButton:
      {
        msgChild__IN_kodi_gui_CWindow_OnContextButton t = in.get().as<decltype(t)>();
        ::kodi::gui::CWindow* window = reinterpret_cast<::kodi::gui::CWindow*>(std::get<0>(t));
        if (window == nullptr)
          break;

        msgpack::pack(out, msgChild_OUT_kodi_gui_CWindow_OnContextButton(
                               window->OnContextButton(std::get<1>(t), std::get<2>(t))));
        return true;
      }
      default:
        break;
    }
  }
  else if (group == funcGroup_gui_controls_Rendering_h)
  {
    switch (func)
    {
      case kodi_gui_controls_CRendering_Create:
      {
        msgChild__IN_kodi_gui_controls_CRendering_Create t = in.get().as<decltype(t)>();
        ::kodi::gui::controls::CRendering* control =
            reinterpret_cast<::kodi::gui::controls::CRendering*>(std::get<0>(t));
        if (control == nullptr)
          break;

        msgpack::pack(out, msgChild_OUT_kodi_gui_controls_CRendering_Create(control->Create(
                               std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t),
                               reinterpret_cast<kodi::HardwareContext>(std::get<4>(t)))));
        return true;
      }
      case kodi_gui_controls_CRendering_Render:
      {
        msgChild__IN_kodi_gui_controls_CRendering_Render t = in.get().as<decltype(t)>();
        ::kodi::gui::controls::CRendering* control =
            reinterpret_cast<::kodi::gui::controls::CRendering*>(std::get<0>(t));
        if (control == nullptr)
          break;

        control->Render();
        return true;
      }
      case kodi_gui_controls_CRendering_Stop:
      {
        msgChild__IN_kodi_gui_controls_CRendering_Stop t = in.get().as<decltype(t)>();
        ::kodi::gui::controls::CRendering* control =
            reinterpret_cast<::kodi::gui::controls::CRendering*>(std::get<0>(t));
        if (control == nullptr)
          break;

        control->Stop();
        return true;
      }
      case kodi_gui_controls_CRendering_Dirty:
      {
        msgChild__IN_kodi_gui_controls_CRendering_Dirty t = in.get().as<decltype(t)>();
        ::kodi::gui::controls::CRendering* control =
            reinterpret_cast<::kodi::gui::controls::CRendering*>(std::get<0>(t));
        if (control == nullptr)
          break;

        msgpack::pack(out, msgChild_OUT_kodi_gui_controls_CRendering_Dirty(control->Dirty()));
        return true;
      }
      default:
        break;
    }
  }

  ::kodi::Log(ADDON_LOG_FATAL, "CChildProcessor::%s: addon called with unknown group/function id '%i/%i'",
              __func__, group, func);
  return false;
}

} /* namespace sandbox */
} /* namespace kodi */
