/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "include/cef_audio_handler.h"
#include "include/cef_app.h"

#include <atomic>
#include <kodi/AudioEngine.h>

class CWebBrowser;

class ATTRIBUTE_HIDDEN CAudioHandler : public CefAudioHandler
{
public:
  CAudioHandler(CWebBrowser* addonMain, bool mute) : m_addonMain(addonMain), m_mute(mute) { }

  /// CefAudioHandler methods
  //@{
  bool GetAudioParameters(CefRefPtr<CefBrowser> browser, CefAudioParameters& params) override;
  void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
                            const CefAudioParameters& params,
                            int channels) override;
  void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser, const float** data, int frames, int64_t pts) override;
  void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser) override;
  void OnAudioStreamError(CefRefPtr<CefBrowser> browser, const CefString& message) override;
  //@}

  void SetMute(bool mute) { m_mute = mute; }

private:
  IMPLEMENT_REFCOUNTING(CAudioHandler);

  CWebBrowser* m_addonMain;
  std::map<int, kodi::audioengine::CAEStream*> m_audioStreams;
  std::atomic_bool m_mute;
};
