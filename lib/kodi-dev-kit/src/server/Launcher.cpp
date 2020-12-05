/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "Launcher.h"

#include "../sandbox/shared_process_func_group.h"
#include "../sandbox/kodi/Processor.h"
#include "kodi/AudioEngine.h"
#include "kodi/Filesystem.h"
#include "kodi/General.h"
#include "kodi/Network.h"
#include "kodi/addon-instance/Web.h"
#include "kodi/gui/General.h"
#include "kodi/gui/ListItem.h"
#include "kodi/gui/Window.h"
#include "kodi/gui/controls/Button.h"
#include "kodi/gui/controls/Edit.h"
#include "kodi/gui/controls/FadeLabel.h"
#include "kodi/gui/controls/Image.h"
#include "kodi/gui/controls/Label.h"
#include "kodi/gui/controls/Progress.h"
#include "kodi/gui/controls/RadioButton.h"
#include "kodi/gui/controls/Rendering.h"
#include "kodi/gui/controls/SettingsSlider.h"
#include "kodi/gui/controls/Slider.h"
#include "kodi/gui/controls/Spin.h"
#include "kodi/gui/controls/TextBox.h"
#include "kodi/gui/dialogs/ContextMenu.h"
#include "kodi/gui/dialogs/ExtendedProgress.h"
#include "kodi/gui/dialogs/FileBrowser.h"
#include "kodi/gui/dialogs/Keyboard.h"
#include "kodi/gui/dialogs/Numeric.h"
#include "kodi/gui/dialogs/OK.h"
#include "kodi/gui/dialogs/Progress.h"
#include "kodi/gui/dialogs/Select.h"
#include "kodi/gui/dialogs/TextViewer.h"
#include "kodi/gui/dialogs/YesNo.h"
#include "kodi/gui/input/Action.h"
#include "utils/Log.h"
#include "utils/uuid.h"

#include <kodi/gui/General.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

std::thread::id main_thread_id;

CChildLauncher::CChildLauncher()
  : m_identifier("kodi-sandproc-" + generateUUID()),
    m_mainThreadTransmit(std::make_shared<CShareProcessTransmitter>(m_identifier + "-receiver", false, true)),
    m_mainThreadReceive(std::make_shared<CShareProcessReceiver>(m_identifier, m_mainThreadTransmit, false))
{
  for (auto& handler : m_handlers)
    handler = nullptr;
  m_handlers[funcGroup_Main] = this;
  m_handlers[funcGroup_AudioEngine_h] = new CAudioEngine_h;
  m_handlers[funcGroup_Filesystem_h] = new CFilesystem_h;
  m_handlers[funcGroup_General_h] = new CGeneral_h;
  m_handlers[funcGroup_Network_h] = new CNetwork_h;
  m_handlers[funcGroup_addoninstance_Web_h] = new C_addoninstance_Web_h(*this);
  m_handlers[funcGroup_gui_General_h] = new C_gui_General_h;
  m_handlers[funcGroup_gui_ListItem_h] = new C_gui_ListItem_h;
  m_handlers[funcGroup_gui_Window_h] = new C_gui_Window_h(*this);
  m_handlers[funcGroup_gui_controls_Button_h] = new C_gui_controls_Button_h;
  m_handlers[funcGroup_gui_controls_Edit_h] = new C_gui_controls_Edit_h;
  m_handlers[funcGroup_gui_controls_FadeLabel_h] = new C_gui_controls_FadeLabel_h;
  m_handlers[funcGroup_gui_controls_Image_h] = new C_gui_controls_Image_h;
  m_handlers[funcGroup_gui_controls_Label_h] = new C_gui_controls_Label_h;
  m_handlers[funcGroup_gui_controls_Progress_h] = new C_gui_controls_Progress_h;
  m_handlers[funcGroup_gui_controls_RadioButton_h] = new C_gui_controls_RadioButton_h;
  m_handlers[funcGroup_gui_controls_Rendering_h] = new C_gui_controls_Rendering_h(*this);
  m_handlers[funcGroup_gui_controls_SettingsSlider_h] = new C_gui_controls_SettingsSlider_h;
  m_handlers[funcGroup_gui_controls_Slider_h] = new C_gui_controls_Slider_h;
  m_handlers[funcGroup_gui_controls_Spin_h] = new C_gui_controls_Spin_h;
  m_handlers[funcGroup_gui_controls_TextBox_h] = new C_gui_controls_TextBox_h;
  m_handlers[funcGroup_gui_dialogs_ContextMenu_h] = new C_gui_dialogs_ContextMenu_h;
  m_handlers[funcGroup_gui_dialogs_ExtendedProgress_h] = new C_gui_dialogs_ExtendedProgress_h;
  m_handlers[funcGroup_gui_dialogs_FileBrowser_h] = new C_gui_dialogs_FileBrowser_h;
  m_handlers[funcGroup_gui_dialogs_Keyboard_h] = new C_gui_dialogs_Keyboard_h;
  m_handlers[funcGroup_gui_dialogs_Numeric_h] = new C_gui_dialogs_Numeric_h;
  m_handlers[funcGroup_gui_dialogs_OK_h] = new C_gui_dialogs_OK_h;
  m_handlers[funcGroup_gui_dialogs_Progress_h] = new C_gui_dialogs_Progress_h;
  m_handlers[funcGroup_gui_dialogs_Select_h] = new C_gui_dialogs_Select_h;
  m_handlers[funcGroup_gui_dialogs_TextViewer_h] = new C_gui_dialogs_TextViewer_h;
  m_handlers[funcGroup_gui_dialogs_YesNo_h] = new C_gui_dialogs_YesNo_h;
  m_handlers[funcGroup_gui_input_Action_h] = new C_gui_input_Action_h;
}

CChildLauncher::~CChildLauncher()
{
  for (auto& handler : m_handlers)
  {
    if (handler != this)
      delete handler;
  }
}

bool CChildLauncher::StartObserver()
{
  main_thread_id = std::this_thread::get_id();
  m_running = true;
  m_thread = std::thread([&] { ObserveProcess(); });

  m_mainThreadReceive->RegisterReceiver(
      std::bind(&CChildLauncher::HandleMainMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                std::placeholders::_5, std::placeholders::_6, std::placeholders::_7));

  return true;
}

bool CChildLauncher::StopObserver()
{
  m_running = false;

  if (GetLastChildStatus() == ChildStatus::Running)
  {
    msgpack::sbuffer in;
    msgpack::pack(in, msgIdentifier(funcGroup_Main, kodi_processor_KillChild));
    msgpack::pack(in, msgChild__IN_kodi_processor_KillChild("end"));
    m_mainThreadTransmit->SendMessage(in);
  }

  if (m_thread.joinable())
    m_thread.join();

  return true;
}

bool CChildLauncher::HandleMainMessage(int group,
                                       int func,
                                       const msgpack::unpacked& in,
                                       msgpack::sbuffer& out,
                                       const char* data,
                                       std::size_t size,
                                       std::size_t offset)
{
  if (m_handlers[group])
    return m_handlers[group]->HandleMessage(group, func, in, out, data, size, offset);
  return false;
}

const std::shared_ptr<CShareProcessTransmitter> CChildLauncher::GetCurrentTransmitter()
{
  std::thread::id threadId = std::this_thread::get_id();

  if (main_thread_id == threadId)
  {
    return m_mainThreadTransmit;
  }

//   std::unique_lock<std::mutex> lock(g_lock);

  auto it = std::find_if(m_childThreadTransmit.begin(),
               m_childThreadTransmit.end(),
               [](std::shared_ptr<kodi::sandbox::CShareProcessTransmitter> entry){ return !entry->m_active;}
              );
  if (it == m_childThreadTransmit.end())
  {
    msgpack::sbuffer in;
    msgpack::sbuffer out;
    msgpack::pack(in, msgIdentifier(funcGroup_Main, kodi_processor_CreateForNewChildThread));
    msgpack::pack(in, msgChild__IN_kodi_processor_CreateForNewThread("subthread" + std::to_string(m_nextChildIdentifier++)));
    m_mainThreadTransmit->SendMessage(in, out);
    msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
    msgParent_OUT_kodi_processor_CreateForNewThread t = ident.get().as<decltype(t)>();

    std::shared_ptr<::kodi::sandbox::CShareProcessTransmitter> transmitter =
        std::make_shared<::kodi::sandbox::CShareProcessTransmitter>(std::get<0>(t), true, false);
    if (!transmitter->Create(false))
    {
      fprintf(stderr, "FATAL: Failed to init other process of sandbox, process not usable!\n");
      return m_mainThreadTransmit;
    }

    m_childThreadTransmit.emplace_back(transmitter);
    return transmitter;
  }

  return *it;
}

bool CChildLauncher::HandleMessage(int group,
                                   int func,
                                   const msgpack::unpacked& in,
                                   msgpack::sbuffer& out,
                                   const char* data,
                                   std::size_t size,
                                   std::size_t offset)
{
  switch (func)
  {
    case kodi_processor_CreateForNewThread:
    {
      fprintf(stderr, "---------------------aa--> DATA: %i\n", in.get().is_nil());
      msgParent__IN_kodi_processor_CreateForNewThread t = in.get().as<decltype(t)>();
      const std::string identifier = m_identifier + "-subthread-" + std::get<0>(t);

      auto other = std::make_shared<CShareProcessReceiver>(identifier, m_mainThreadTransmit, true);
      other->RegisterReceiver(
          std::bind(&CChildLauncher::HandleMessage, this, std::placeholders::_1,
                    std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
                    std::placeholders::_5, std::placeholders::_6, std::placeholders::_7));
      if (!other->Create(true, true))
      {
        kodi::Log(ADDON_LOG_ERROR,
                                "CProcessReceiver::%s: Failed to create other second receiver",
                                __func__);
        return false;
      }

      msgpack::pack(out, msgParent_OUT_kodi_processor_CreateForNewThread(identifier));

      m_childThreadReceive.emplace_back(other);
      break;
    }

    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "CProcessReceiver::%s: addon called with unknown function id '%i' on group 'kodi'",
          __func__, func);
      return false;
  }

  return true;
}

void CChildLauncher::ObserveProcess()
{
  while (m_running)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    const ChildStatus status = ProcessActive();
    if (status == ChildStatus::Running)
    {

      continue;
    }
    else
    {

      break;
    }
  }
}

IMsgHdl* CChildLauncher::GetHdl(funcGroup group)
{
  return m_handlers[group];
}

} /* namespace parent */
} /* namespace sandbox */
} /* namespace kodi */
