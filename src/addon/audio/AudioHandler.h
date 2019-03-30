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

#pragma once

#include "include/cef_audio_handler.h"
#include "include/cef_app.h"

#include <kodi/AudioEngine.h>

class CAudioHandler : public CefAudioHandler
{
public:
  CAudioHandler() = default;

  /// CefAudioHandler methods
  //@{
  void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser, int audio_stream_id, int channels, ChannelLayout channel_layout,
                            int sample_rate, int frames_per_buffer) override;
  void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, int audio_stream_id, const float** data, int frames, int64_t pts) override;
  void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser, int audio_stream_id) override;
  //@}

private:
  IMPLEMENT_REFCOUNTING(CAudioHandler);

  unsigned int m_frames;
  std::map<int, kodi::audioengine::CAddonAEStream*> m_audioStreams;
};
