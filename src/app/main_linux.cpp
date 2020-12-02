/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "app-main/AppBrowser.h"
#include "app-main/MainCEFProcess.h"
#include "LibraryLoader.h"
#include "app-other/AppOther.h"
#include "app-renderer/AppRenderer.h"

// Dev-kit
#include "../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../lib/kodi-dev-kit/include/kodi/Processor.h"

// Global
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define DEBUG

namespace client
{

// These flags must match the Chromium values.
std::string kProcessType = "type";
std::string kRendererProcess = "renderer";
std::string kZygoteProcess = "zygote";
std::string kGPUProcess = "gpu-process";

void SignalHandler(int sig)
{
}

int RunMain(int argc, char* argv[])
{
  const std::string main_shared = kodi::sandbox::CheckMainShared(argc, argv);
  if (main_shared.empty())
  {
    fprintf(stderr,
            "%s called independent, this is a sandbox only and started by Kodi "
            "itself!\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  // Startup the shared memory interface
  kodi::sandbox::CChildProcessor processor(main_shared, true);

  kodi::Log(ADDON_LOG_INFO, "%s: Creating the Google Chromium Internet Browser add-on", __func__);

  // Load CEF library by self
  CCEFLibraryLoader library_loader;
  if (!library_loader.LoadInMain())
  {
    kodi::Log(ADDON_LOG_FATAL, "%s: cef library load failed!", __func__);
    return EXIT_FAILURE;
  }

#if defined(CEF_USE_SANDBOX)
  // Check set of sandbox and if needed ask user about root password to set correct rights of them
  if (CefSandboxNeedRoot() && !SandboxControl::SetSandbox())
    return EXIT_FAILURE;
#endif

  // Provide CEF with command-line arguments.
  CefMainArgs main_args(argc, argv);

  // Parse command-line arguments for use in this method.
  CefRefPtr<CefCommandLine> commandLine = CefCommandLine::CreateCommandLine();
  commandLine->InitFromArgv(argc, argv);

  const uint64_t base_handle = kodi::sandbox::CheckBaseHandle(argc, argv);
  if (!base_handle)
  {
    fprintf(stderr,
            "%s called without base handle\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  std::shared_ptr<chromium::app::main::CMainCEFProcess> mainProcess =
      std::make_shared<chromium::app::main::CMainCEFProcess>(base_handle, processor.GetIdentifier());
  mainProcess->ParseCmdLine(commandLine);

  CefSettings settings;
  mainProcess->SetSettings(settings);

  CefRefPtr<CefApp> app = new chromium::app::main::CClientAppBrowser(mainProcess);
  if (!mainProcess->Initialize(main_args, settings, app, nullptr))
  {
    kodi::Log(ADDON_LOG_FATAL, "%s: Web browser start failed", __func__);
    return EXIT_FAILURE;
  }

  // Set up signal handler so we clean up after CEF change.
  struct sigaction signalHandler;
  std::memset(&signalHandler, 0, sizeof(signalHandler));
  signalHandler.sa_handler = &SignalHandler;
  signalHandler.sa_flags = SA_RESTART;
  sigaction(SIGINT, &signalHandler, nullptr);
  sigaction(SIGTERM, &signalHandler, nullptr);

  if (!mainProcess->MainInitialize())
  {
    kodi::Log(ADDON_LOG_FATAL, "%s: Failed to initialize main process system", __func__);
    return EXIT_FAILURE;
  }

  while (1)
  {
    if (!processor.ProcessOutside())
      break;
  }

  mainProcess->Shutdown();
  mainProcess.reset();

  kodi::Log(ADDON_LOG_INFO, "%s: Stopped Google Chromium Internet Browser add-on successfully", __func__);

  return EXIT_SUCCESS;
}

int RunChild(int argc, char* argv[])
{
  int ret = -1;

  CCEFLibraryLoader library_loader;
  if (!library_loader.LoadInMain())
  {
    fprintf(stderr, "FATAL: Failed to load CEF library: '%s'", argv[0]);
    return ret;
  }

  CefMainArgs main_args(argc, argv);

  // Parse command-line arguments.
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
  command_line->InitFromArgv(argc, argv);

  // Create a ClientApp of the correct type.
  CefRefPtr<CefApp> app;
  const std::string& process_type = command_line->GetSwitchValue(kProcessType);

#ifdef DEBUG
  fprintf(stderr, "DEBUG_INFO: Sandbox Kodi Chromium started for process_type %s\n", process_type.c_str());
#endif

  if (process_type != "")
  {
    if (process_type == kRendererProcess ||
        process_type == kZygoteProcess)
    {
      // On Linux the zygote process is used to spawn other process types. Since
      // we don't know what type of process it will be give it the renderer
      // client.
      app = new CWebAppRenderer();
    }
    else
    {
      app = new CWebAppOther();
    }

    // Execute the secondary process, if any.
    ret = CefExecuteProcess(main_args, app, nullptr);
  }
  else
  {
    fprintf(stderr, "FATAL: kodichromium sandbox seems to start as browser type (process type undefined!)\n");
  }

#ifdef DEBUG
  fprintf(stderr, "DEBUG_INFO: Sandbox Kodi Chromium ended for process_type %s\n", process_type.c_str());
#endif

  return ret;
}

} /* namespace client */

int main(int argc, char* argv[])
{
  const std::string sandbox = kodi::sandbox::CheckSandbox(argc, argv);
  const std::string main_shared = kodi::sandbox::CheckMainShared(argc, argv);
  const uint64_t base_handle = kodi::sandbox::CheckBaseHandle(argc, argv);
  if (sandbox.empty() || main_shared.empty() || !base_handle)
    return client::RunChild(argc, argv);

  return client::RunMain(argc, argv);
}
