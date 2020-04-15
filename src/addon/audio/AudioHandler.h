/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_audio_handler.h"
#include "include/cef_app.h"

#include <kodi/AudioEngine.h>

class CWebBrowser;

class ATTRIBUTE_HIDDEN CAudioHandler : public CefAudioHandler
{
public:
  CAudioHandler(CWebBrowser* addonMain, bool mute) : m_addonMain(addonMain), m_mute(mute) { }

  /// CefAudioHandler methods
  //@{
  bool GetAudioParameters(CefRefPtr<CefBrowser> browser, int audio_stream_id, CefAudioParameters& params) override;
  void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser, int audio_stream_id, int channels, ChannelLayout channel_layout,
                            int sample_rate, int frames_per_buffer) override;
  void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, int audio_stream_id, const float** data, int frames, int64_t pts) override;
  void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser, int audio_stream_id) override;
  void OnAudioStreamError(CefRefPtr<CefBrowser> browser, int audio_stream_id, const CefString& message) override;
  //@}

  void SetMute(bool mute) { m_mute = mute; }

private:
  IMPLEMENT_REFCOUNTING(CAudioHandler);

  CWebBrowser* m_addonMain;
  std::map<int, kodi::audioengine::CAEStream*> m_audioStreams;
  bool m_mute;
};
