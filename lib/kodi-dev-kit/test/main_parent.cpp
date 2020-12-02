#include "../src/server/LauncherPosix.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  kodi::sandbox::CChildLauncherPosix launcher;

  std::vector<std::string> arg;
  arg.push_back("./kodichild");
  arg.push_back("--sandbox=webbrowser");
  launcher.Launch(arg, false);


  sleep(15);

  return 0;
}
