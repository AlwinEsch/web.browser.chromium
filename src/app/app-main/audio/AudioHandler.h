/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/AudioEngine.h"

// CEF
#include "include/cef_app.h"
#include "include/cef_audio_handler.h"

// Global
#include <atomic>
#include <memory>

namespace chromium
{
namespace app
{
namespace main
{

class CMainCEFProcess;

namespace audio
{

class CAudioHandler : public CefAudioHandler
{
public:
  CAudioHandler(CMainCEFProcess& addonMain, bool mute);
  ~CAudioHandler();

  /// CefAudioHandler methods
  //@{
  bool GetAudioParameters(CefRefPtr<CefBrowser> browser, CefAudioParameters& params) override;
  void OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
                            const CefAudioParameters& params,
                            int channels) override;
  void OnAudioStreamPacket(CefRefPtr<CefBrowser> browser,
                           const float** data,
                           int frames,
                           int64_t pts) override;
  void OnAudioStreamStopped(CefRefPtr<CefBrowser> browser) override;
  void OnAudioStreamError(CefRefPtr<CefBrowser> browser, const CefString& message) override;
  //@}

  void SetMute(bool mute) { m_mute = mute; }

private:
  IMPLEMENT_REFCOUNTING(CAudioHandler);

  CMainCEFProcess& m_addonMain;
  std::map<int, std::unique_ptr<kodi::audioengine::CAEStream>> m_audioStreams;
  std::atomic_bool m_mute;
};

} /* namespace audio */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
