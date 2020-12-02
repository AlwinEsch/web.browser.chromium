/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include <kodi/c-api/audio_engine.h>

#include <vector>

namespace kodi
{
namespace audioengine
{

class AudioEngineFormat
{
public:
  AudioEngineFormat() = default;
  AudioEngineFormat(AudioEngineDataFormat dataFormat,
                    unsigned int sampleRate,
                    unsigned int encodedRate,
                    const std::vector<AudioEngineChannel>& channels,
                    unsigned int frames,
                    unsigned int frameSize)
    : m_dataFormat(dataFormat),
      m_sampleRate(sampleRate),
      m_encodedRate(encodedRate),
      m_channels(channels),
      m_frames(frames),
      m_frameSize(frameSize)
  {
  }

  void SetDataFormat(AudioEngineDataFormat format) { m_dataFormat = format; }
  AudioEngineDataFormat GetDataFormat() const { return m_dataFormat; }

  void SetSampleRate(unsigned int rate) { m_sampleRate = rate; }
  unsigned int GetSampleRate() const { return m_sampleRate; }

  void SetEncodedRate(unsigned int rate) { m_encodedRate = rate; }
  unsigned int GetEncodedRate() const { return m_encodedRate; }

  void SetChannelLayout(const std::vector<AudioEngineChannel>& layout)
  {
    // Reset first all to empty values to AUDIOENGINE_CH_NULL, in case given list is empty
    m_channels = layout;
  }
  std::vector<AudioEngineChannel> GetChannelLayout() const { return m_channels; }

  void SetFramesAmount(unsigned int frames) { m_frames = frames; }
  unsigned int GetFramesAmount() const { return m_frames; }

  void SetFrameSize(unsigned int frameSize) { m_frameSize = frameSize; }
  unsigned int GetFrameSize() const { return m_frameSize; }

  bool CompareFormat(const AudioEngineFormat* fmt)
  {
    if (!fmt)
    {
      return false;
    }

    if (m_dataFormat != fmt->m_dataFormat || m_sampleRate != fmt->m_sampleRate ||
        m_encodedRate != fmt->m_encodedRate || m_frames != fmt->m_frames ||
        m_frameSize != fmt->m_frameSize || m_channels.size() != fmt->m_channels.size())
    {
      return false;
    }

    for (unsigned int ch = 0; ch < AUDIOENGINE_CH_MAX; ++ch)
    {
      if (fmt->m_channels[ch] != m_channels[ch])
      {
        return false;
      }
    }

    return true;
  }

private:
  AudioEngineDataFormat m_dataFormat = AUDIOENGINE_FMT_INVALID;

  /*! The stream's sample rate (eg, 48000) */
  unsigned int m_sampleRate = 0;

  /*! The encoded streams sample rate if a bitstream, otherwise undefined */
  unsigned int m_encodedRate = 0;

  /*! The stream's channel layout */
  std::vector<AudioEngineChannel> m_channels;

  /*! The number of frames per period */
  unsigned int m_frames = 0;

  /*! The size of one frame in bytes */
  unsigned int m_frameSize = 0;
};

class CAEStream
{
public:
  CAEStream(AudioEngineFormat& format, unsigned int options = 0);
  ~CAEStream();

  unsigned int GetSpace();
  unsigned int AddData(uint8_t* const* data,
                       unsigned int offset,
                       unsigned int frames,
                       double pts = 0,
                       bool hasDownmix = false,
                       double centerMixLevel = 1.0);
  double GetDelay();
  bool IsBuffering();
  double GetCacheTime();
  double GetCacheTotal();
  void Pause();
  void Resume();
  void Drain(bool wait = true);
  bool IsDraining();
  bool IsDrained();
  void Flush();
  float GetVolume();
  void SetVolume(float volume);
  float GetAmplification();
  void SetAmplification(float amplify);
  unsigned int GetFrameSize() const;
  unsigned int GetChannelCount() const;
  unsigned int GetSampleRate() const;
  AudioEngineDataFormat GetDataFormat() const;
  double GetResampleRatio();
  void SetResampleRatio(double ratio);

private:
  AudioEngineFormat m_format;
  uint64_t m_handle = 0;
  int m_planes = 1;
};

bool GetCurrentSinkFormat(AudioEngineFormat& format);
bool IsPlanar(AudioEngineDataFormat format);

} /* namespace audioengine */
} /* namespace kodi */
