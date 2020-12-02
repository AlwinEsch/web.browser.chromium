#include "kodi/General.h"
#include "kodi/Processor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <unistd.h>

int main(int argc, char* argv[])
{
  const std::string sandbox = kodi::sandbox::CheckSandbox(argc, argv);
  const std::string main_shared = kodi::sandbox::CheckMainShared(argc, argv);
  if (sandbox.empty() || main_shared.empty())
  {
    fprintf(stderr, "%s called independent, this is a sandbox only and started by Kodi itself!\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  kodi::sandbox::CChildProcessor processor(main_shared, true);

  kodi::Log(ADDON_LOG_ERROR, "Hello World");

  std::thread thread = std::thread([&] { kodi::Log(ADDON_LOG_ERROR, "Hello another World"); });
  thread.detach();

  std::thread thread2 = std::thread([&] { kodi::Log(ADDON_LOG_ERROR, "Hello another World"); });
  thread2.detach();


  while (1)
  {
    if (!processor.ProcessOutside())
      break;
  }


  sleep(3);

  return EXIT_SUCCESS;
}
