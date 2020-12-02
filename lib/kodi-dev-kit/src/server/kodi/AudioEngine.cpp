/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "AudioEngine.h"

#include "../../sandbox/kodi/AudioEngine.h"
#include "../utils/Log.h"

#include <kodi/AudioEngine.h>

namespace kodi
{
namespace sandbox
{
namespace parent
{

bool CAudioEngine_h::HandleMessage(int funcGroup,
                                   int func,
                                   const msgpack::unpacked& in,
                                   msgpack::sbuffer& out,
                                   const char* data,
                                   std::size_t size,
                                   std::size_t offset)
{
  if (funcGroup != funcGroup_AudioEngine_h)
    return false;

  switch (func)
  {
    case funcParent_AudioEngine_h::kodi_audioengine_GetCurrentSinkFormat:
    {
      msgParent__IN_kodi_audioengine_GetCurrentSinkFormat t = in.get().as<decltype(t)>();

      kodi::audioengine::AudioEngineFormat format;
      bool ret = kodi::audioengine::GetCurrentSinkFormat(format);

      std::vector<int> channelLayout;
      for (const auto& entry : format.GetChannelLayout())
        channelLayout.emplace_back(entry);

      msgpack::pack(
          out, msgParent_OUT_kodi_audioengine_GetCurrentSinkFormat(
                   ret, kodi_audioengine_FORMAT(format.GetDataFormat(), format.GetSampleRate(),
                                                format.GetEncodedRate(), channelLayout,
                                                format.GetFramesAmount(), format.GetFrameSize())));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_constructor:
    {
      msgParent__IN_kodi_audioengine_CAEStream_constructor t = in.get().as<decltype(t)>();

      kodi::audioengine::AudioEngineFormat format;

      kodi_audioengine_FORMAT formatRet = std::get<1>(t);
      format.SetDataFormat(AudioEngineDataFormat(std::get<0>(formatRet)));
      format.SetSampleRate(std::get<1>(formatRet));
      format.SetEncodedRate(std::get<2>(formatRet));
      std::vector<AudioEngineChannel> channelLayoutRet;
      for (const auto& entry : std::get<3>(formatRet))
        channelLayoutRet.emplace_back(AudioEngineChannel(entry));
      format.SetChannelLayout(channelLayoutRet);
      format.SetFramesAmount(std::get<4>(formatRet));
      format.SetFrameSize(std::get<5>(formatRet));

      kodi::audioengine::CAEStream* audioStream = new kodi::audioengine::CAEStream(format);
      msgpack::pack(out,
                    msgParent_OUT_kodi_audioengine_CAEStream_constructor(uint64_t(audioStream)));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_destructor:
    {
      msgParent__IN_kodi_audioengine_CAEStream_destructor t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      delete audioStream;
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetSpace:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetSpace t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_audioengine_CAEStream_GetSpace(audioStream->GetSpace()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_AddData:
    {
      msgParent__IN_kodi_audioengine_CAEStream_AddData t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;

      unsigned int offsetStream = std::get<2>(t);
      unsigned int frames = std::get<3>(t);
      double pts = std::get<4>(t);
      bool hasDownmix = std::get<5>(t);
      double centerMixLevel = std::get<6>(t);

      uint8_t* dataStream[AUDIOENGINE_CH_MAX] = {nullptr};
      for (size_t i = 0; i < std::get<1>(t); ++i)
      {
        auto input = msgpack::unpack(data, size, offset);
        if (input->type == msgpack::type::BIN)
        {
          dataStream[i] = (uint8_t *)&input->via.bin.ptr[0];
        }
      }

      unsigned int ret =
          audioStream->AddData(dataStream, offsetStream, frames, pts, hasDownmix, centerMixLevel);
      msgpack::pack(out, msgParent_OUT_kodi_audioengine_CAEStream_AddData(ret));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetDelay:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetDelay t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_audioengine_CAEStream_GetDelay(audioStream->GetDelay()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_IsBuffering:
    {
      msgParent__IN_kodi_audioengine_CAEStream_IsBuffering t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(
          out, msgParent_OUT_kodi_audioengine_CAEStream_IsBuffering(audioStream->IsBuffering()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetCacheTime:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetCacheTime t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(
          out, msgParent_OUT_kodi_audioengine_CAEStream_GetCacheTime(audioStream->GetCacheTime()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetCacheTotal:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetCacheTotal t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_audioengine_CAEStream_GetCacheTotal(
                             audioStream->GetCacheTotal()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Pause:
    {
      msgParent__IN_kodi_audioengine_CAEStream_Pause t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      audioStream->Pause();
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Resume:
    {
      msgParent__IN_kodi_audioengine_CAEStream_Resume t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      audioStream->Resume();
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Drain:
    {
      msgParent__IN_kodi_audioengine_CAEStream_Drain t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      audioStream->Drain(std::get<1>(t));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_IsDraining:
    {
      msgParent__IN_kodi_audioengine_CAEStream_IsDraining t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_audioengine_CAEStream_IsDraining(audioStream->IsDraining()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_IsDrained:
    {
      msgParent__IN_kodi_audioengine_CAEStream_IsDrained t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_audioengine_CAEStream_IsDrained(audioStream->IsDrained()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Flush:
    {
      msgParent__IN_kodi_audioengine_CAEStream_Flush t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      audioStream->Flush();
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetVolume:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetVolume t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out,
                    msgParent_OUT_kodi_audioengine_CAEStream_GetVolume(audioStream->GetVolume()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_SetVolume:
    {
      msgParent__IN_kodi_audioengine_CAEStream_SetVolume t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      audioStream->SetVolume(std::get<1>(t));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetAmplification:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetAmplification t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_audioengine_CAEStream_GetAmplification(
                             audioStream->GetAmplification()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_SetAmplification:
    {
      msgParent__IN_kodi_audioengine_CAEStream_SetAmplification t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      audioStream->SetAmplification(std::get<1>(t));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetFrameSize:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetFrameSize t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(
          out, msgParent_OUT_kodi_audioengine_CAEStream_GetFrameSize(audioStream->GetFrameSize()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetChannelCount:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetChannelCount t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_audioengine_CAEStream_GetChannelCount(
                             audioStream->GetChannelCount()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetSampleRate:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetSampleRate t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_audioengine_CAEStream_GetSampleRate(
                             audioStream->GetSampleRate()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetDataFormat:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetDataFormat t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_audioengine_CAEStream_GetDataFormat(
                             audioStream->GetDataFormat()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetResampleRatio:
    {
      msgParent__IN_kodi_audioengine_CAEStream_GetResampleRatio t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      msgpack::pack(out, msgParent_OUT_kodi_audioengine_CAEStream_GetResampleRatio(
                             audioStream->GetResampleRatio()));
      break;
    }
    case funcParent_AudioEngine_h::kodi_audioengine_CAEStream_SetResampleRatio:
    {
      msgParent__IN_kodi_audioengine_CAEStream_SetResampleRatio t = in.get().as<decltype(t)>();
      kodi::audioengine::CAEStream* audioStream =
          reinterpret_cast<kodi::audioengine::CAEStream*>(std::get<0>(t));
      if (audioStream == nullptr)
        break;
      audioStream->SetResampleRatio(std::get<1>(t));
      break;
    }
    default:
      kodi::utils::LOG_MESSAGE(
          ADDON_LOG_ERROR,
          "CAudioEngine_h::%s: addon called with unknown function id '%i' on group 'kodi'",
          __func__, func);
      return false;
  }

  return true;
}

} // namespace parent
} // namespace sandbox
} /* namespace kodi */
