/*
 *      Copyright (C) 2015-2019 Team KODI
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

#include "AudioHandler.h"

void CAudioHandler::OnAudioStreamStarted(CefRefPtr<CefBrowser> browser, int audio_stream_id, int channels, ChannelLayout channel_layout,
                                         int sample_rate, int frames_per_buffer)
{
  m_frames = 0;
  AudioEngineFormat format;
  format.m_dataFormat = AE_FMT_FLOAT;
  format.m_channelCount = channels;
  int ptr = 0;
  switch (channel_layout)
  {
    case CEF_CHANNEL_LAYOUT_MONO:
      format.m_channels[ptr++] = AE_CH_FL;
      break;

    // Front L, Front R
    case CEF_CHANNEL_LAYOUT_STEREO:
    case CEF_CHANNEL_LAYOUT_STEREO_DOWNMIX:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      break;

    // Front L, Front R, Back C
    case CEF_CHANNEL_LAYOUT_2_1:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_BC;
      break;

    // Front L, Front R, Front C
    case CEF_CHANNEL_LAYOUT_SURROUND:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      break;

    // Front L, Front R, Front C, Back C
    case CEF_CHANNEL_LAYOUT_4_0:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_BC;
      break;

    // Front L, Front R, Side L, Side R
    case CEF_CHANNEL_LAYOUT_2_2:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      break;

    // Front L, Front R, Back L, Back R
    case CEF_CHANNEL_LAYOUT_QUAD:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      break;

    // Front L, Front R, Front C, Side L, Side R
    case CEF_CHANNEL_LAYOUT_5_0:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      break;

    // Front L, Front R, Front C, LFE, Side L, Side R
    case CEF_CHANNEL_LAYOUT_5_1:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      break;

    // Front L, Front R, Front C, Back L, Back R
    case CEF_CHANNEL_LAYOUT_5_0_BACK:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      break;

    // Front L, Front R, Front C, LFE, Back L, Back R
    case CEF_CHANNEL_LAYOUT_5_1_BACK:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      break;

    // Front L, Front R, Front C, Side L, Side R, Back L, Back R
    case CEF_CHANNEL_LAYOUT_7_0:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      break;

    // Front L, Front R, Front C, LFE, Side L, Side R, Back L, Back R
    case CEF_CHANNEL_LAYOUT_7_1:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      break;

    // Front L, Front R, Front C, LFE, Side L, Side R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_7_1_WIDE:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_FLOC;
      format.m_channels[ptr++] = AE_CH_FROC;
      break;

    // Stereo L, Stereo R, LFE
    case CEF_CHANNEL_LAYOUT_2POINT1:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_LFE;
      break;

    // Stereo L, Stereo R, Front C, LFE
    case CEF_CHANNEL_LAYOUT_3_1:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      break;

    // Stereo L, Stereo R, Front C, Rear C, LFE
    case CEF_CHANNEL_LAYOUT_4_1:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_BC;
      format.m_channels[ptr++] = AE_CH_LFE;
      break;

    // Stereo L, Stereo R, Front C, Side L, Side R, Back C
    case CEF_CHANNEL_LAYOUT_6_0:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_BC;
      break;

    // Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_6_0_FRONT:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_FLOC;
      format.m_channels[ptr++] = AE_CH_FROC;
      break;

    // Stereo L, Stereo R, Front C, Rear L, Rear R, Rear C
    case CEF_CHANNEL_LAYOUT_HEXAGONAL:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      format.m_channels[ptr++] = AE_CH_BC;
      break;

    // Stereo L, Stereo R, Front C, LFE, Side L, Side R, Rear Center
    case CEF_CHANNEL_LAYOUT_6_1:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_BC;
      break;

    // Stereo L, Stereo R, Front C, LFE, Back L, Back R, Rear Center
    case CEF_CHANNEL_LAYOUT_6_1_BACK:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      format.m_channels[ptr++] = AE_CH_BC;
      break;

    // Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC, LFE
    case CEF_CHANNEL_LAYOUT_6_1_FRONT:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_FLOC;
      format.m_channels[ptr++] = AE_CH_FROC;
      format.m_channels[ptr++] = AE_CH_LFE;
      break;

    // Front L, Front R, Front C, Side L, Side R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_7_0_FRONT:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_FLOC;
      format.m_channels[ptr++] = AE_CH_FROC;
      break;

    // Front L, Front R, Front C, LFE, Back L, Back R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_7_1_WIDE_BACK:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_LFE;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      format.m_channels[ptr++] = AE_CH_FLOC;
      format.m_channels[ptr++] = AE_CH_FROC;
      break;

    // Front L, Front R, Front C, Side L, Side R, Rear L, Back R, Back C.
    case CEF_CHANNEL_LAYOUT_OCTAGONAL:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_FC;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_BL;
      format.m_channels[ptr++] = AE_CH_BR;
      format.m_channels[ptr++] = AE_CH_BC;
      break;

    // Front L, Front R, Side L, Side R, LFE
    case CEF_CHANNEL_LAYOUT_4_1_QUAD_SIDE:
      format.m_channels[ptr++] = AE_CH_FL;
      format.m_channels[ptr++] = AE_CH_FR;
      format.m_channels[ptr++] = AE_CH_SL;
      format.m_channels[ptr++] = AE_CH_SR;
      format.m_channels[ptr++] = AE_CH_LFE;
      break;

    // Channels are not explicitly mapped to speakers.
    case CEF_CHANNEL_LAYOUT_DISCRETE:
      break;

    // Front L, Front R, Front C. Front C contains the keyboard mic audio. This
    // layout is only intended for input for WebRTC. The Front C channel
    // is stripped away in the WebRTC audio input pipeline and never seen outside
    // of that.
    case CEF_CHANNEL_LAYOUT_STEREO_AND_KEYBOARD_MIC:
      break;

    case CEF_CHANNEL_LAYOUT_NONE:
    case CEF_CHANNEL_LAYOUT_UNSUPPORTED:
    default: ;
  }
  format.m_channels[ptr++] = AE_CH_NULL;
  format.m_sampleRate = sample_rate;
  format.m_frameSize = sizeof(float)*channels;//bytes_per_frame;
  format.m_frames = frames_per_buffer;
  m_audioStreams[audio_stream_id] = new kodi::audioengine::CAddonAEStream(format);
}

void CAudioHandler::OnAudioStreamPacket(CefRefPtr<CefBrowser> browser,
                                        int audio_stream_id,
                                        const float** data,
                                        int frames, int64_t pts)
{
  const auto& handler = m_audioStreams.find(audio_stream_id);
  if (handler != m_audioStreams.end())
    handler->second->AddData((uint8_t* const *)(&data), 0, frames, pts);
}

void CAudioHandler::OnAudioStreamStopped(CefRefPtr<CefBrowser> browser,
                                         int audio_stream_id)
{
  const auto& handler = m_audioStreams.find(audio_stream_id);
  if (handler != m_audioStreams.end())
  {
    delete handler->second;
    m_audioStreams.erase(audio_stream_id);
  }
}
