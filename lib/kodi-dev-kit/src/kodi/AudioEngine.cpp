/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../sandbox/kodi/AudioEngine.h"

#include "../../include/kodi/AudioEngine.h"
#include "../../include/kodi/General.h"
#include "../../include/kodi/Processor.h"
#include "../sandbox/ShareProcessTransmitter.h"

#include <kodi/tools/StringUtils.h>

using namespace kodi::sandbox;

namespace kodi
{
namespace audioengine
{

CAEStream::CAEStream(AudioEngineFormat& format, unsigned int options) : m_format(format)
{
  std::vector<int> channelLayout;
  for (const auto& entry : format.GetChannelLayout())
    channelLayout.emplace_back(entry);
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_AudioEngine_h,
                              funcParent_AudioEngine_h::kodi_audioengine_CAEStream_constructor));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_constructor(
                        uint64_t(this),
                        kodi_audioengine_FORMAT(format.GetDataFormat(), format.GetSampleRate(),
                                                format.GetEncodedRate(), channelLayout,
                                                format.GetFramesAmount(), format.GetFrameSize()),
                        options));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_constructor t = ident.get().as<decltype(t)>();
  m_handle = std::get<0>(t);
  m_planes = IsPlanar(format.GetDataFormat()) ? channelLayout.size() : 1;
}

CAEStream::~CAEStream()
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_destructor));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_destructor(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

unsigned int CAEStream::GetSpace()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetSpace));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetSpace(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetSpace t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

unsigned int CAEStream::AddData(uint8_t* const* data,
                                unsigned int offset,
                                unsigned int frames,
                                double pts,
                                bool hasDownmix,
                                double centerMixLevel)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_AddData));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_AddData(
                        m_handle, m_planes, offset, frames, pts, hasDownmix, centerMixLevel));

  size_t size = frames * m_format.GetFrameSize();
  for (unsigned int i = 0; i < m_planes; ++i)
  {
    msgpack::packer<msgpack::sbuffer> packer(in);
    packer.pack_bin(size);
    packer.pack_bin_body(reinterpret_cast<const char*>(data[i]), size);
  }
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_AddData t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

double CAEStream::GetDelay()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetDelay));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetDelay(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetDelay t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CAEStream::IsBuffering()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_AudioEngine_h,
                              funcParent_AudioEngine_h::kodi_audioengine_CAEStream_IsBuffering));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_IsBuffering(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_IsBuffering t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

double CAEStream::GetCacheTime()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_AudioEngine_h,
                              funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetCacheTime));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetCacheTime(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetCacheTime t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

double CAEStream::GetCacheTotal()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_AudioEngine_h,
                              funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetCacheTotal));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetCacheTotal(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetCacheTotal t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CAEStream::Pause()
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Pause));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_Pause(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

void CAEStream::Resume()
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Resume));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_Resume(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

void CAEStream::Drain(bool wait)
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Drain));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_Drain(m_handle, wait));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

bool CAEStream::IsDraining()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_IsDraining));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_IsDraining(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_IsDraining t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

bool CAEStream::IsDrained()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_IsDrained));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_IsDrained(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_IsDrained t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CAEStream::Flush()
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_Flush));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_Flush(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

float CAEStream::GetVolume()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetVolume));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetVolume(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetVolume t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CAEStream::SetVolume(float volume)
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_CAEStream_SetVolume));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_SetVolume(m_handle, volume));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

float CAEStream::GetAmplification()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_AudioEngine_h,
                        funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetAmplification));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetAmplification(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetAmplification t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CAEStream::SetAmplification(float amplify)
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_AudioEngine_h,
                        funcParent_AudioEngine_h::kodi_audioengine_CAEStream_SetAmplification));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_SetAmplification(m_handle, amplify));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

unsigned int CAEStream::GetFrameSize() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_AudioEngine_h,
                              funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetFrameSize));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetFrameSize(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetFrameSize t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

unsigned int CAEStream::GetChannelCount() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_AudioEngine_h,
                        funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetChannelCount));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetChannelCount(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetChannelCount t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

unsigned int CAEStream::GetSampleRate() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_AudioEngine_h,
                              funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetSampleRate));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetSampleRate(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetSampleRate t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

AudioEngineDataFormat CAEStream::GetDataFormat() const
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in,
                msgIdentifier(funcGroup_AudioEngine_h,
                              funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetDataFormat));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetDataFormat(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetDataFormat t = ident.get().as<decltype(t)>();
  return AudioEngineDataFormat(std::get<0>(t));
}

double CAEStream::GetResampleRatio()
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_AudioEngine_h,
                        funcParent_AudioEngine_h::kodi_audioengine_CAEStream_GetResampleRatio));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_GetResampleRatio(m_handle));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_CAEStream_GetResampleRatio t = ident.get().as<decltype(t)>();
  return std::get<0>(t);
}

void CAEStream::SetResampleRatio(double ratio)
{
  msgpack::unpacked ident;
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(
      in, msgIdentifier(funcGroup_AudioEngine_h,
                        funcParent_AudioEngine_h::kodi_audioengine_CAEStream_SetResampleRatio));
  msgpack::pack(in, msgParent__IN_kodi_audioengine_CAEStream_SetResampleRatio(m_handle, ratio));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
}

bool GetCurrentSinkFormat(AudioEngineFormat& format)
{
  msgpack::sbuffer in;
  msgpack::sbuffer out;
  msgpack::pack(in, msgIdentifier(funcGroup_AudioEngine_h,
                                  funcParent_AudioEngine_h::kodi_audioengine_GetCurrentSinkFormat));

  std::vector<int> channelLayout;
  for (const auto& entry : format.GetChannelLayout())
    channelLayout.emplace_back(entry);

  msgpack::pack(in, msgParent__IN_kodi_audioengine_GetCurrentSinkFormat(kodi_audioengine_FORMAT(
                        format.GetDataFormat(), format.GetSampleRate(), format.GetEncodedRate(),
                        channelLayout, format.GetFramesAmount(), format.GetFrameSize())));
  CChildProcessor::GetCurrentProcessor()->SendMessage(in, out);
  msgpack::unpacked ident = msgpack::unpack(out.data(), out.size());
  msgParent_OUT_kodi_audioengine_GetCurrentSinkFormat t = ident.get().as<decltype(t)>();

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
  return std::get<0>(t);
}

bool IsPlanar(AudioEngineDataFormat format)
{
  if (format >= AUDIOENGINE_FMT_U8P || format <= AUDIOENGINE_FMT_FLOATP)
    return true;
  return false;
}

} /* namespace audioengine */
} /* namespace kodi */
