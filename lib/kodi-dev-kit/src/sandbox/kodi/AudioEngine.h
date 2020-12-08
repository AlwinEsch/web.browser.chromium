/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../../include/kodi/c-api/audio_engine.h"

#include <string>
#include <tuple>
#include <vector>

namespace kodi
{
namespace sandbox
{

typedef int DummyValue;

typedef enum funcParent_AudioEngine_h : int
{
  kodi_audioengine_GetCurrentSinkFormat,
  kodi_audioengine_CAEStream_constructor,
  kodi_audioengine_CAEStream_destructor,
  kodi_audioengine_CAEStream_GetSpace,
  kodi_audioengine_CAEStream_AddData,
  kodi_audioengine_CAEStream_GetDelay,
  kodi_audioengine_CAEStream_IsBuffering,
  kodi_audioengine_CAEStream_GetCacheTime,
  kodi_audioengine_CAEStream_GetCacheTotal,
  kodi_audioengine_CAEStream_Pause,
  kodi_audioengine_CAEStream_Resume,
  kodi_audioengine_CAEStream_Drain,
  kodi_audioengine_CAEStream_IsDraining,
  kodi_audioengine_CAEStream_IsDrained,
  kodi_audioengine_CAEStream_Flush,
  kodi_audioengine_CAEStream_GetVolume,
  kodi_audioengine_CAEStream_SetVolume,
  kodi_audioengine_CAEStream_GetAmplification,
  kodi_audioengine_CAEStream_SetAmplification,
  kodi_audioengine_CAEStream_GetFrameSize,
  kodi_audioengine_CAEStream_GetChannelCount,
  kodi_audioengine_CAEStream_GetSampleRate,
  kodi_audioengine_CAEStream_GetDataFormat,
  kodi_audioengine_CAEStream_GetResampleRatio,
  kodi_audioengine_CAEStream_SetResampleRatio,
} funcParent_AudioEngine_h;

// clang-format off
typedef std::tuple<int, unsigned int, unsigned int, std::vector<int>, unsigned int, unsigned int> kodi_audioengine_FORMAT;
typedef std::tuple<kodi_audioengine_FORMAT>                                     msgParent__IN_kodi_audioengine_GetCurrentSinkFormat;
typedef std::tuple<bool, kodi_audioengine_FORMAT>                               msgParent_OUT_kodi_audioengine_GetCurrentSinkFormat;
typedef std::tuple<uint64_t, kodi_audioengine_FORMAT, unsigned int>             msgParent__IN_kodi_audioengine_CAEStream_constructor;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_audioengine_CAEStream_constructor;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_destructor;
typedef std::tuple<uint64_t>                                                    msgParent_OUT_kodi_audioengine_CAEStream_destructor;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetSpace;
typedef std::tuple<unsigned int>                                                msgParent_OUT_kodi_audioengine_CAEStream_GetSpace;
typedef std::tuple<uint64_t, unsigned int, unsigned int, unsigned int, double, bool, double> msgParent__IN_kodi_audioengine_CAEStream_AddData;
typedef std::tuple<unsigned int>                                                msgParent_OUT_kodi_audioengine_CAEStream_AddData;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetDelay;
typedef std::tuple<double>                                                      msgParent_OUT_kodi_audioengine_CAEStream_GetDelay;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_IsBuffering;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_audioengine_CAEStream_IsBuffering;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetCacheTime;
typedef std::tuple<double>                                                      msgParent_OUT_kodi_audioengine_CAEStream_GetCacheTime;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetCacheTotal;
typedef std::tuple<double>                                                      msgParent_OUT_kodi_audioengine_CAEStream_GetCacheTotal;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_Pause;
typedef std::tuple<DummyValue>                                                  msgParent_OUT_kodi_audioengine_CAEStream_Pause;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_Resume;
typedef std::tuple<DummyValue>                                                  msgParent_OUT_kodi_audioengine_CAEStream_Resume;
typedef std::tuple<uint64_t, bool>                                              msgParent__IN_kodi_audioengine_CAEStream_Drain;
typedef std::tuple<DummyValue>                                                  msgParent_OUT_kodi_audioengine_CAEStream_Drain;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_IsDraining;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_audioengine_CAEStream_IsDraining;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_IsDrained;
typedef std::tuple<bool>                                                        msgParent_OUT_kodi_audioengine_CAEStream_IsDrained;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_Flush;
typedef std::tuple<DummyValue>                                                  msgParent_OUT_kodi_audioengine_CAEStream_Flush;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetVolume;
typedef std::tuple<float>                                                       msgParent_OUT_kodi_audioengine_CAEStream_GetVolume;
typedef std::tuple<uint64_t, float>                                             msgParent__IN_kodi_audioengine_CAEStream_SetVolume;
typedef std::tuple<DummyValue>                                                  msgParent_OUT_kodi_audioengine_CAEStream_SetVolume;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetAmplification;
typedef std::tuple<float>                                                       msgParent_OUT_kodi_audioengine_CAEStream_GetAmplification;
typedef std::tuple<uint64_t, float>                                             msgParent__IN_kodi_audioengine_CAEStream_SetAmplification;
typedef std::tuple<DummyValue>                                                  msgParent_OUT_kodi_audioengine_CAEStream_SetAmplification;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetFrameSize;
typedef std::tuple<unsigned int>                                                msgParent_OUT_kodi_audioengine_CAEStream_GetFrameSize;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetChannelCount;
typedef std::tuple<unsigned int>                                                msgParent_OUT_kodi_audioengine_CAEStream_GetChannelCount;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetSampleRate;
typedef std::tuple<unsigned int>                                                msgParent_OUT_kodi_audioengine_CAEStream_GetSampleRate;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetDataFormat;
typedef std::tuple<int>                                                         msgParent_OUT_kodi_audioengine_CAEStream_GetDataFormat;
typedef std::tuple<uint64_t>                                                    msgParent__IN_kodi_audioengine_CAEStream_GetResampleRatio;
typedef std::tuple<double>                                                      msgParent_OUT_kodi_audioengine_CAEStream_GetResampleRatio;
typedef std::tuple<uint64_t, double>                                            msgParent__IN_kodi_audioengine_CAEStream_SetResampleRatio;
typedef std::tuple<DummyValue>                                                  msgParent_OUT_kodi_audioengine_CAEStream_SetResampleRatio;
// clang-format on

//==============================================================================

typedef enum funcChild_AudioEngine_h : int
{
  funcIdAddon_unused_AudioEngine_h
} funcChild_Filesystem_h;

} /* namespace sandbox */
} /* namespace kodi */
