/*
 *      Copyright (C) 2015 Team KODI
 *      http:/kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "include/cef_app.h"
#include "main.h"
#include "client_app_renderer.h"
#include "client_app_other.h"

namespace client {

// Entry point function for sub-processes.
int RunMain(int argc, char* argv[])
{
  // Provide CEF with command-line arguments.
  CefMainArgs main_args(argc, argv);

  // Parse command-line arguments.
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
  command_line->InitFromArgv(argc, argv);

  // Create a ClientApp of the correct type.
  CefRefPtr<CefApp> app;
//   ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
// //   if (process_type == ClientApp::RendererProcess)
// {
//
// fprintf(stderr, "11111111111------------------------------ %s\n", __FUNCTION__);
//     app = new ClientAppRenderer();
// }
// //   else if (process_type == ClientApp::OtherProcess)
// // {
// // fprintf(stderr, "-222222222222222----------------------------- %s\n", __FUNCTION__);
// //     app = new ClientAppOther();
// // }
// fprintf(stderr, "111111111111111111111 --------------------------- process_type %i command_line '%s'\n", process_type, command_line->GetCommandLineString().ToString().c_str());

  // Execute the sub-process.
  return CefExecuteProcess(main_args, app, NULL);
}

}  // namespace client

// Process entry point.
int main(int argc, char* argv[])
{
  return client::RunMain(argc, argv);
}

/*
command_line '/home/alwin/Development/build/addons/web.browser.chromium/kodichromium
--type=zygote
--lang=en
--locales-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/resources/cef/locales
--log-file=debug.log
--resources-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/resources/cef/
--user-agent=Chrome/60.0.3112.40
--kodi-addon-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/kodichromium
--ppapi-flash-path="/home/alwin/Development/addons/web/web.browser.chromium/web.browser.chromium/flash_player/libpepflashplayer.so"
--ppapi-flash-version=26.0.0.151'

command_line '/home/alwin/Development/build/addons/web.browser.chromium/kodichromium
--type=gpu-process
--lang=en
--locales-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/resources/cef/locales
--log-file=debug.log
--resources-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/resources/cef/
--user-agent=Chrome/60.0.3112.40
--kodi-addon-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/kodichromium
--supports-dual-gpus=false
--gpu-driver-bug-workarounds=7,24,77
--disable-gl-extensions=GL_KHR_blend_equation_advanced GL_KHR_blend_equation_advanced_coherent
--gpu-vendor-id=0x10de
--gpu-device-id=0x139b
--gpu-driver-vendor=Nvidia
--gpu-driver-version=375.66
--gpu-driver-date
--gpu-secondary-vendor-ids=0x8086
--gpu-secondary-device-ids=0x191b
--lang=en
--locales-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/resources/cef/locales
--log-file=debug.log --resources-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/resources/cef/
--user-agent=Chrome/60.0.3112.40
--kodi-addon-dir-path=/home/alwin/Development/build/addons/web.browser.chromium/kodichromium
--service-request-channel-token=AD1FE5CB097C80043C11C2D157A09E5F'
*/
