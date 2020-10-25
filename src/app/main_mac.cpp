/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AppOther.h"
#include "renderer/AppRenderer.h"

#include "include/base/cef_logging.h"
#include "include/base/cef_scoped_ptr.h"
#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_library_loader.h"

// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically. Pass -DUSE_SANDBOX=OFF to the CMake command-line to disable
// use of the sandbox.
#if defined(CEF_USE_SANDBOX)
#include "include/cef_sandbox_mac.h"
#endif

// These flags must match the Chromium values.
std::string kProcessType = "type";
std::string kRendererProcess = "renderer";
std::string kZygoteProcess = "zygote";
std::string kGPUProcess = "gpu-process";

int main(int argc, char* argv[])
{
  int ret = -1;

#if defined(CEF_USE_SANDBOX)
  // Initialize the macOS sandbox for this helper process.
  CefScopedSandboxContext sandbox_context;
  if (!sandbox_context.Initialize(argc, argv))
  {
    fprintf(stderr, "FATAL: kodichromium sandbox initialize failed!)");
    return ret;
  }
#endif

  // Load the CEF framework library at runtime instead of linking directly
  // as required by the macOS sandbox implementation.
  CefScopedLibraryLoader library_loader;
  if (!library_loader.LoadInHelper())
  {
    fprintf(stderr, "FATAL: kodichromium cef library load failed!)");
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
    fprintf(stderr, "FATAL: kodichromium sandbox seems to start as browser type (process type undefined!)");
  }

#ifdef DEBUG
  fprintf(stderr, "DEBUG_INFO: Sandbox Kodi Chromium ended for process_type %s\n", process_type.c_str());
#endif

  return ret;
}
