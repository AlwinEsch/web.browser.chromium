/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "../../../../include/kodi/gui/input/Action.h"

#include "../../../../include/kodi/Processor.h"
#include "../../../sandbox/ShareProcessTransmitter.h"
#include "../../../sandbox/kodi/gui/input/Action.h"
#include "../../../sandbox/shared_process_func_group.h"

namespace kodi
{
namespace gui
{
namespace input
{

CAction::CAction(ADDON_ACTION actionID, uint32_t unicode) : m_id(actionID), m_unicode(unicode)
{
}

CAction::CAction(ADDON_ACTION actionID, const std::string& name) : m_id(actionID), m_name(name)
{
}

CAction::CAction(ADDON_ACTION actionID,
                 float amount1,
                 float amount2,
                 const std::string& name,
                 unsigned int holdTime)
  : m_id(actionID), m_name(name), m_holdTime(holdTime)
{
  m_amount[0] = amount1;
  m_amount[1] = amount2;
}

CAction::CAction(ADDON_ACTION actionID,
                 unsigned int state,
                 float posX,
                 float posY,
                 float offsetX,
                 float offsetY,
                 float velocityX,
                 float velocityY,
                 const std::string& name)
  : m_id(actionID), m_name(name), m_holdTime(state)
{
  m_amount[0] = posX;
  m_amount[1] = posY;
  m_amount[2] = offsetX;
  m_amount[3] = offsetY;
  m_amount[4] = velocityX;
  m_amount[5] = velocityY;
}

CAction::CAction(const CAction& other)
{
  *this = other;
}

CAction::CAction(const sandbox::gui_input_CAction& addonData)
{
  m_id = addonData.id;
  m_name = addonData.name;
  m_repeat = addonData.repeat;
  m_holdTime = addonData.holdTime;
  m_buttonCode = addonData.buttonCode;
  m_unicode = addonData.unicode;
  m_text = addonData.text;
  m_amount = addonData.amount;
}

CAction& CAction::operator=(const CAction& rhs)
{
  if (this != &rhs)
  {
    m_id = rhs.m_id;
    for (unsigned int i = 0; i < ADDON_ACTION_AMOUNT_MAX; i++)
      m_amount[i] = rhs.m_amount[i];
    m_name = rhs.m_name;
    m_repeat = rhs.m_repeat;
    m_buttonCode = rhs.m_buttonCode;
    m_unicode = rhs.m_unicode;
    m_holdTime = rhs.m_holdTime;
    m_text = rhs.m_text;
  }
  return *this;
}

ADDON_ACTION CAction::GetID() const
{
  return m_id;
}

bool CAction::IsMouse() const
{
  return (m_id >= ADDON_ACTION_MOUSE_START && m_id <= ADDON_ACTION_MOUSE_END);
}

bool CAction::IsGesture() const
{
  return (m_id >= ADDON_ACTION_GESTURE_NOTIFY && m_id <= ADDON_ACTION_GESTURE_END);
}

bool CAction::IsAnalog() const
{
  switch (m_id)
  {
    case ADDON_ACTION_ANALOG_SEEK_FORWARD:
    case ADDON_ACTION_ANALOG_SEEK_BACK:
    case ADDON_ACTION_SCROLL_UP:
    case ADDON_ACTION_SCROLL_DOWN:
    case ADDON_ACTION_ANALOG_FORWARD:
    case ADDON_ACTION_ANALOG_REWIND:
    case ADDON_ACTION_ANALOG_MOVE:
    case ADDON_ACTION_ANALOG_MOVE_X_LEFT:
    case ADDON_ACTION_ANALOG_MOVE_X_RIGHT:
    case ADDON_ACTION_ANALOG_MOVE_Y_UP:
    case ADDON_ACTION_ANALOG_MOVE_Y_DOWN:
    case ADDON_ACTION_CURSOR_LEFT:
    case ADDON_ACTION_CURSOR_RIGHT:
    case ADDON_ACTION_VOLUME_UP:
    case ADDON_ACTION_VOLUME_DOWN:
    case ADDON_ACTION_ZOOM_IN:
    case ADDON_ACTION_ZOOM_OUT:
      return true;
    default:
      return false;
  }
}

const std::string& CAction::GetName() const
{
  return m_name;
}

const std::string& CAction::GetText() const
{
  return m_text;
}

void CAction::SetText(const std::string& text)
{
  m_text = text;
}

float CAction::GetAmount(unsigned int index) const
{
  return (index < ADDON_ACTION_AMOUNT_MAX) ? m_amount[index] : 0;
}

void CAction::ClearAmount()
{
  for (float& amount : m_amount)
    amount = 0;
}

uint32_t CAction::GetUnicode() const
{
  return m_unicode;
}

unsigned int CAction::GetHoldTime() const
{
  return m_holdTime;
}

float CAction::GetRepeat() const
{
  return m_repeat;
}

unsigned int CAction::GetButtonCode() const
{
  return m_buttonCode;
}

} /* namespace input */
} /* namespace gui */
} /* namespace kodi */
