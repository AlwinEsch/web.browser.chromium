/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "../../c-api/gui/input/action.h"
#include "ActionIDs.h"

#ifdef __cplusplus

#include <msgpack.hpp>
#include <string>

namespace kodi
{
namespace sandbox
{
struct gui_input_CAction;
} /* namespace sandbox */
} /* namespace kodi */

namespace kodi
{
namespace gui
{

class CWindow;

namespace input
{

class CAction
{
public:
  CAction() = default;
  CAction(ADDON_ACTION actionID, uint32_t unicode);
  CAction(ADDON_ACTION actionID, const std::string& name);
  CAction(ADDON_ACTION actionID,
          float amount1 = 1.0f,
          float amount2 = 0.0f,
          const std::string& name = "",
          unsigned int holdTime = 0);
  CAction(ADDON_ACTION actionID,
          unsigned int state,
          float posX,
          float posY,
          float offsetX,
          float offsetY,
          float velocityX = 0.0f,
          float velocityY = 0.0f,
          const std::string& name = "");
  CAction(const CAction& other);
  CAction(const sandbox::gui_input_CAction& addonData);

  CAction& operator=(const CAction& rhs);
  ADDON_ACTION GetID() const;
  bool IsMouse() const;
  bool IsGesture() const;
  bool IsAnalog() const;
  const std::string& GetName() const;
  const std::string& GetText() const;
  void SetText(const std::string& text);
  float GetAmount(unsigned int index = 0) const;
  void ClearAmount();
  uint32_t GetUnicode() const;
  unsigned int GetHoldTime() const;
  float GetRepeat() const;
  unsigned int GetButtonCode() const;

private:
  ADDON_ACTION m_id = ADDON_ACTION_NONE;
  std::string m_name;

  std::array<float, ADDON_ACTION_AMOUNT_MAX> m_amount = {};

  float m_repeat = 0.0f;
  unsigned int m_holdTime = 0;
  unsigned int m_buttonCode = 0;
  uint32_t m_unicode = 0; /* wchar_t */
  std::string m_text;
};

} /* namespace input */
} /* namespace gui */
} /* namespace kodi */

#endif /* __cplusplus */
