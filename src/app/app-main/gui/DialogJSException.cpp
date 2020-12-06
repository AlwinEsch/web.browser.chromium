/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogJSException.h"

// DevKit
#include "../../../../lib/kodi-dev-kit/include/kodi/General.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/gui/dialogs/TextViewer.h"
#include "../../../../lib/kodi-dev-kit/include/kodi/tools/StringUtils.h"

// Global
#include <thread>

using kodi::tools::StringUtils;

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

namespace JSException
{

void ReportJSException(CefRefPtr<CefProcessMessage> message)
{
  std::string header = kodi::GetLocalizedString(30043);
  std::string text = kodi::GetLocalizedString(30044);
  auto argList = message->GetArgumentList();

  int64 identifier;
  auto binaryValue = argList->GetBinary(0);
  binaryValue->GetData(&identifier, sizeof(int64), 1);
  std::string excMessage = argList->GetString(1);
  std::string sourceLine = argList->GetString(2);
  std::string scriptResourceName = argList->GetString(3);

  text += StringUtils::Format(kodi::GetLocalizedString(30045).c_str(), excMessage.c_str());
  text += StringUtils::Format(kodi::GetLocalizedString(30046).c_str(), sourceLine.c_str());
  text += StringUtils::Format(kodi::GetLocalizedString(30047).c_str(), scriptResourceName.c_str());
  text += kodi::GetLocalizedString(30048);

  auto argFrames = argList->GetList(4);
  for (auto i = 0; i < static_cast<int>(argFrames->GetSize()); i++)
  {
    auto frame = argFrames->GetList(i);

    std::string functionName = frame->GetString(0);
    int lineNumber = frame->GetInt(1);
    int columnNumber = frame->GetInt(2);
    std::string sourceName = frame->GetString(3);

    text += StringUtils::Format(kodi::GetLocalizedString(30049).c_str(), lineNumber, columnNumber, functionName.c_str(), sourceName.c_str());
  }

  std::thread([header, text] {
    kodi::gui::dialogs::TextViewer::Show(header, text);
  }).detach();
}

} /* namespace JSException */

} /* namespace gui */
} /* main */
} /* app */
} /* chromium */



