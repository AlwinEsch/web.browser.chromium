/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <functional>
#include <msgpack.hpp>

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>

#if (defined TARGET_WINDOWS)
typedef HANDLE SEMAPHORE_ID;
typedef unsigned long THREAD_ID;
#elif (defined TARGET_POSIX)
#include <semaphore.h>

typedef sem_t SEMAPHORE_ID;
typedef pthread_t THREAD_ID;
#else
#error Not supported OS used
#endif

#define SHARED_DEFAULT_MEM_SIZE 32768*4
#define DEFAULT_SHARED_MEM_ARRAY_SIZE SHARED_DEFAULT_MEM_SIZE - (sizeof(uint32_t) * 10)

typedef struct apiShareData
{
  bool inited;
  SEMAPHORE_ID sem_segment_caller;
  SEMAPHORE_ID sem_segment_target;

  bool main_thread;
  THREAD_ID calling_thread;

  bool bigger;
  size_t data_size;
  char data[SHARED_DEFAULT_MEM_SIZE];
} apiShareData;

using SandboxProcess = std::function<bool(int group, int func, const msgpack::unpacked& in, msgpack::sbuffer& out,
                                          const char* data, std::size_t size, std::size_t offset)>;
typedef std::pair<uint32_t, uint32_t> msgIdentifier;

struct texture_storage_metadata_t
{
  int width;
  int height;
  int fourcc;
  EGLint offset;
  EGLint stride;
  int num_planes;
  EGLuint64KHR modifiers;
};
