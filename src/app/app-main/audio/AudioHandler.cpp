/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "AudioHandler.h"

// Dev-kit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"

/*
TODO:
- Sync between audio and video not correct now
  - Add way to synchronize chromiums video playback with PTS of Kodi's given audio
- Check by current sandbox shared memory size match in all cases and if not add ways to pass more in several calls
*/

namespace chromium
{
namespace app
{
namespace main
{
namespace audio
{

namespace
{
static std::atomic_int m_ctorcount{0}; // For debug purposes and to see destructs done
}

CAudioHandler::CAudioHandler(CMainCEFProcess& addonMain, bool mute)
  : m_addonMain(addonMain), m_mute(mute)
{
  fprintf(stderr, "CAudioHandler START (%p) count open %i\n", this, ++m_ctorcount);
}

CAudioHandler::~CAudioHandler()
{
  fprintf(stderr, "CAudioHandler STOP (%p) count open %i\n", this, --m_ctorcount);
}

bool CAudioHandler::GetAudioParameters(CefRefPtr<CefBrowser> browser, CefAudioParameters& params)
{
  kodi::audioengine::AudioEngineFormat format;
  if (!kodi::audioengine::GetCurrentSinkFormat(format))
    return false;

  std::vector<AudioEngineChannel> layout = format.GetChannelLayout();

  params.channel_layout = CEF_CHANNEL_LAYOUT_STEREO; // default
  switch (layout.size())
  {
    case 1:
    {
      params.channel_layout = CEF_CHANNEL_LAYOUT_MONO;
      break;
    }
    case 2:
    {
      params.channel_layout = CEF_CHANNEL_LAYOUT_STEREO;
      break;
    }
    case 3:
    {
      if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
          layout[2] == AUDIOENGINE_CH_BC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_2_1;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_SURROUND;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_LFE)
        params.channel_layout = CEF_CHANNEL_LAYOUT_2POINT1;

      break;
    }
    case 4:
    {
      if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FC &&
          layout[2] == AUDIOENGINE_CH_FR && layout[3] == AUDIOENGINE_CH_BC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_4_0;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_SL && layout[3] == AUDIOENGINE_CH_SR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_2_2;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_BL && layout[3] == AUDIOENGINE_CH_BR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_QUAD;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE)
        params.channel_layout = CEF_CHANNEL_LAYOUT_3_1;

      break;
    }
    case 5:
    {
      if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
          layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_SL &&
          layout[4] == AUDIOENGINE_CH_SR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_5_0;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_BL &&
               layout[4] == AUDIOENGINE_CH_BR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_5_0_BACK;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_SL && layout[3] == AUDIOENGINE_CH_SR &&
               layout[4] == AUDIOENGINE_CH_LFE)
        params.channel_layout = CEF_CHANNEL_LAYOUT_4_1_QUAD_SIDE;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_BC &&
               layout[4] == AUDIOENGINE_CH_LFE)
        params.channel_layout = CEF_CHANNEL_LAYOUT_4_1;

      break;
    }
    case 6:
    {
      if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
          layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE &&
          layout[4] == AUDIOENGINE_CH_SL && layout[5] == AUDIOENGINE_CH_SR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_5_1;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE &&
               layout[4] == AUDIOENGINE_CH_BL && layout[5] == AUDIOENGINE_CH_BR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_5_1_BACK;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_SL &&
               layout[4] == AUDIOENGINE_CH_SR && layout[5] == AUDIOENGINE_CH_BC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_6_0;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_SL && layout[3] == AUDIOENGINE_CH_SR &&
               layout[4] == AUDIOENGINE_CH_FLOC && layout[5] == AUDIOENGINE_CH_FROC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_6_0_FRONT;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_BL &&
               layout[4] == AUDIOENGINE_CH_BR && layout[5] == AUDIOENGINE_CH_BC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_HEXAGONAL;

      break;
    }
    case 7:
    {
      if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
          layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_SL &&
          layout[4] == AUDIOENGINE_CH_SR && layout[5] == AUDIOENGINE_CH_BL &&
          layout[6] == AUDIOENGINE_CH_BR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_7_0;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE &&
               layout[4] == AUDIOENGINE_CH_SL && layout[5] == AUDIOENGINE_CH_SR &&
               layout[6] == AUDIOENGINE_CH_BC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_6_1;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE &&
               layout[4] == AUDIOENGINE_CH_BL && layout[5] == AUDIOENGINE_CH_BR &&
               layout[6] == AUDIOENGINE_CH_BC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_6_1_BACK;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_SL && layout[3] == AUDIOENGINE_CH_SR &&
               layout[4] == AUDIOENGINE_CH_FLOC && layout[5] == AUDIOENGINE_CH_FROC &&
               layout[6] == AUDIOENGINE_CH_LFE)
        params.channel_layout = CEF_CHANNEL_LAYOUT_6_1_FRONT;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_SL &&
               layout[4] == AUDIOENGINE_CH_SR && layout[5] == AUDIOENGINE_CH_FLOC &&
               layout[6] == AUDIOENGINE_CH_FROC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_7_0_FRONT;

      break;
    }
    case 8:
    {
      if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
          layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE &&
          layout[4] == AUDIOENGINE_CH_SL && layout[5] == AUDIOENGINE_CH_SR &&
          layout[6] == AUDIOENGINE_CH_BL && layout[7] == AUDIOENGINE_CH_BR)
        params.channel_layout = CEF_CHANNEL_LAYOUT_7_1;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE &&
               layout[4] == AUDIOENGINE_CH_SL && layout[5] == AUDIOENGINE_CH_SR &&
               layout[6] == AUDIOENGINE_CH_FLOC && layout[7] == AUDIOENGINE_CH_FROC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_7_1_WIDE;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_LFE &&
               layout[4] == AUDIOENGINE_CH_BL && layout[5] == AUDIOENGINE_CH_BR &&
               layout[6] == AUDIOENGINE_CH_FLOC && layout[7] == AUDIOENGINE_CH_FROC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_7_1_WIDE_BACK;
      else if (layout[0] == AUDIOENGINE_CH_FL && layout[1] == AUDIOENGINE_CH_FR &&
               layout[2] == AUDIOENGINE_CH_FC && layout[3] == AUDIOENGINE_CH_SL &&
               layout[4] == AUDIOENGINE_CH_SR && layout[5] == AUDIOENGINE_CH_BL &&
               layout[6] == AUDIOENGINE_CH_BR && layout[7] == AUDIOENGINE_CH_BC)
        params.channel_layout = CEF_CHANNEL_LAYOUT_OCTAGONAL;

      break;
    }
    default:
      break;
  }

  params.sample_rate = format.GetSampleRate();
  params.frames_per_buffer = format.GetFramesAmount();
  return true;
}


void CAudioHandler::OnAudioStreamStarted(CefRefPtr<CefBrowser> browser,
                                         const CefAudioParameters& params,
                                         int channels)
{
  std::vector<AudioEngineChannel> layout;
  switch (params.channel_layout)
  {
    case CEF_CHANNEL_LAYOUT_MONO:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      break;

    // Front L, Front R
    case CEF_CHANNEL_LAYOUT_STEREO:
    case CEF_CHANNEL_LAYOUT_STEREO_DOWNMIX:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      break;

    // Front L, Front R, Back C
    case CEF_CHANNEL_LAYOUT_2_1:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      break;

    // Front L, Front R, Front C
    case CEF_CHANNEL_LAYOUT_SURROUND:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      break;

    // Front L, Front R, Front C, Back C
    case CEF_CHANNEL_LAYOUT_4_0:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      break;

    // Front L, Front R, Side L, Side R
    case CEF_CHANNEL_LAYOUT_2_2:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      break;

    // Front L, Front R, Back L, Back R
    case CEF_CHANNEL_LAYOUT_QUAD:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      break;

    // Front L, Front R, Front C, Side L, Side R
    case CEF_CHANNEL_LAYOUT_5_0:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      break;

    // Front L, Front R, Front C, LFE, Side L, Side R
    case CEF_CHANNEL_LAYOUT_5_1:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      break;

    // Front L, Front R, Front C, Back L, Back R
    case CEF_CHANNEL_LAYOUT_5_0_BACK:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      break;

    // Front L, Front R, Front C, LFE, Back L, Back R
    case CEF_CHANNEL_LAYOUT_5_1_BACK:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      break;

    // Front L, Front R, Front C, Side L, Side R, Back L, Back R
    case CEF_CHANNEL_LAYOUT_7_0:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      break;

    // Front L, Front R, Front C, LFE, Side L, Side R, Back L, Back R
    case CEF_CHANNEL_LAYOUT_7_1:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      break;

    // Front L, Front R, Front C, LFE, Side L, Side R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_7_1_WIDE:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_FLOC);
      layout.emplace_back(AUDIOENGINE_CH_FROC);
      break;

    // Stereo L, Stereo R, LFE
    case CEF_CHANNEL_LAYOUT_2POINT1:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      break;

    // Stereo L, Stereo R, Front C, LFE
    case CEF_CHANNEL_LAYOUT_3_1:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      break;

    // Stereo L, Stereo R, Front C, Rear C, LFE
    case CEF_CHANNEL_LAYOUT_4_1:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      break;

    // Stereo L, Stereo R, Front C, Side L, Side R, Back C
    case CEF_CHANNEL_LAYOUT_6_0:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      break;

    // Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_6_0_FRONT:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_FLOC);
      layout.emplace_back(AUDIOENGINE_CH_FROC);
      break;

    // Stereo L, Stereo R, Front C, Rear L, Rear R, Rear C
    case CEF_CHANNEL_LAYOUT_HEXAGONAL:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      break;

    // Stereo L, Stereo R, Front C, LFE, Side L, Side R, Rear Center
    case CEF_CHANNEL_LAYOUT_6_1:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      break;

    // Stereo L, Stereo R, Front C, LFE, Back L, Back R, Rear Center
    case CEF_CHANNEL_LAYOUT_6_1_BACK:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      break;

    // Stereo L, Stereo R, Side L, Side R, Front LofC, Front RofC, LFE
    case CEF_CHANNEL_LAYOUT_6_1_FRONT:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_FLOC);
      layout.emplace_back(AUDIOENGINE_CH_FROC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      break;

    // Front L, Front R, Front C, Side L, Side R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_7_0_FRONT:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_FLOC);
      layout.emplace_back(AUDIOENGINE_CH_FROC);
      break;

    // Front L, Front R, Front C, LFE, Back L, Back R, Front LofC, Front RofC
    case CEF_CHANNEL_LAYOUT_7_1_WIDE_BACK:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      layout.emplace_back(AUDIOENGINE_CH_FLOC);
      layout.emplace_back(AUDIOENGINE_CH_FROC);
      break;

    // Front L, Front R, Front C, Side L, Side R, Rear L, Back R, Back C.
    case CEF_CHANNEL_LAYOUT_OCTAGONAL:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_FC);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_BL);
      layout.emplace_back(AUDIOENGINE_CH_BR);
      layout.emplace_back(AUDIOENGINE_CH_BC);
      break;

    // Front L, Front R, Side L, Side R, LFE
    case CEF_CHANNEL_LAYOUT_4_1_QUAD_SIDE:
      layout.emplace_back(AUDIOENGINE_CH_FL);
      layout.emplace_back(AUDIOENGINE_CH_FR);
      layout.emplace_back(AUDIOENGINE_CH_SL);
      layout.emplace_back(AUDIOENGINE_CH_SR);
      layout.emplace_back(AUDIOENGINE_CH_LFE);
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
    default:;
  }

  kodi::audioengine::AudioEngineFormat format;
  format.SetDataFormat(AUDIOENGINE_FMT_FLOATP);
  format.SetChannelLayout(layout);
  format.SetSampleRate(params.sample_rate);
  format.SetFrameSize(sizeof(float)); //bytes_per_frame;
  format.SetFramesAmount(params.frames_per_buffer);
  m_audioStreams[browser->GetIdentifier()] =
      std::unique_ptr<kodi::audioengine::CAEStream>(new kodi::audioengine::CAEStream(format));
}

void CAudioHandler::OnAudioStreamPacket(CefRefPtr<CefBrowser> browser,
                                        const float** data,
                                        int frames,
                                        int64_t pts)
{
  if (m_mute)
    return;

  const auto& handler = m_audioStreams.find(browser->GetIdentifier());
  if (handler != m_audioStreams.end())
    handler->second->AddData((uint8_t* const*)(data), 0, frames, pts);
}

void CAudioHandler::OnAudioStreamStopped(CefRefPtr<CefBrowser> browser)
{
  const auto& handler = m_audioStreams.find(browser->GetIdentifier());
  if (handler != m_audioStreams.end())
  {
    m_audioStreams.erase(browser->GetIdentifier());
  }
}

void CAudioHandler::OnAudioStreamError(CefRefPtr<CefBrowser> browser, const CefString& message)
{
  CefRefPtr<CefFrame> frame = browser->GetMainFrame();
  std::string url = frame->GetURL();
  kodi::Log(ADDON_LOG_ERROR, "CAudioHandler::%s: Audio stream error: %s (on %s)",
            message.ToString().c_str(), url.c_str());
}

} /* namespace audio */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
