#pragma once
/*
 *      Copyright (C) 2015-2017 Team KODI
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

#include <string>
#include <memory>
#include <vector>

#include <p8-platform/threads/mutex.h>

struct MessageCallback
{
  void (*callback)(void *userptr);
  void *userptr;
};

typedef struct
{
  unsigned int dwMessage;
  int param1;
  int param2;
  int param3;
  std::string strParam;
  std::vector<std::string> params;
  std::shared_ptr<P8PLATFORM::CEvent> waitEvent;
  void* lpVoid;
} Message;
