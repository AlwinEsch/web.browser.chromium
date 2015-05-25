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

namespace client {

// Entry point function for sub-processes.
int RunMain(int argc, char* argv[])
{
  // Provide CEF with command-line arguments.
  CefMainArgs main_args(argc, argv);

  // Execute the sub-process.
  return CefExecuteProcess(main_args, NULL, NULL);
}

}  // namespace client

// Process entry point.
int main(int argc, char* argv[])
{
  return client::RunMain(argc, argv);
}
