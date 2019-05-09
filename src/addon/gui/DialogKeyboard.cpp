/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DialogKeyboard.h"
#include "utils/SystemTranslator.h"

#include <kodi/General.h>
#include <kodi/XBMC_vkeys.h>
#include <kodi/gui/controls/Button.h>

#include "include/cef_browser.h"
#include "include/views/cef_textfield.h"

#include <locale>
#include <codecvt>

#define KEY_VKEY            0xF000 // a virtual key/functional key e.g. cursor left
#define KEY_ASCII           0xF100 // a printable character in the range of TRUE ASCII (from 0 to 127) // FIXME make it clean and pure unicode! remove the need for KEY_ASCII
#define KEY_UNICODE         0xF200 // another printable character whose range is not included in this KEY code

#define BUTTON_ID_OFFSET      100
#define BUTTONS_PER_ROW        20
#define BUTTONS_MAX_ROWS        4

#define CTL_BUTTON_DONE       300
#define CTL_BUTTON_CANCEL     301
#define CTL_BUTTON_SHIFT      302
#define CTL_BUTTON_CAPS       303
#define CTL_BUTTON_SYMBOLS    304
#define CTL_BUTTON_LEFT       305
#define CTL_BUTTON_RIGHT      306
#define CTL_BUTTON_LAYOUT     309
#define CTL_BUTTON_POS_SWAP   310

#define CTL_LABEL_HEADING     311
#define CTL_EDIT              312
#define CTL_LABEL_HZCODE      313
#define CTL_LABEL_HZLIST      314

#define CTL_BUTTON_BACKSPACE    8
#define CTL_BUTTON_SPACE       32

#define CTL_BUTTON_DUMMY    20000

CBrowserDialogKeyboard::CBrowserDialogKeyboard()
  : CWindow("BrowserDialogKeyboard.xml", "skin.estuary", true),
    m_client(nullptr)
{
}

void CBrowserDialogKeyboard::Show(CefRefPtr<CWebBrowserClient> client, cef_text_input_mode_t input_mode)
{
  m_client = client;
  m_input_mode = input_mode;

  switch (m_input_mode)
  {
    case CEF_TEXT_INPUT_MODE_TEXT:
      m_strHeading = kodi::GetLocalizedString(30326);
      break;
    case CEF_TEXT_INPUT_MODE_TEL:
      m_strHeading = kodi::GetLocalizedString(30327);
      break;
    case CEF_TEXT_INPUT_MODE_URL:
      m_strHeading = kodi::GetLocalizedString(30328);
      break;
    case CEF_TEXT_INPUT_MODE_EMAIL:
      m_strHeading = kodi::GetLocalizedString(30329);
      break;
    case CEF_TEXT_INPUT_MODE_NUMERIC:
      m_strHeading = kodi::GetLocalizedString(30330);
      break;
    case CEF_TEXT_INPUT_MODE_DECIMAL:
      m_strHeading = kodi::GetLocalizedString(30331);
      break;
    case CEF_TEXT_INPUT_MODE_SEARCH:
      m_strHeading = kodi::GetLocalizedString(30332);
      break;
    case CEF_TEXT_INPUT_MODE_DEFAULT:
    case CEF_TEXT_INPUT_MODE_NONE:
    default:
      break;
  }

  CWindow::Show();
}

bool CBrowserDialogKeyboard::OnInit()
{
  // set alphabetic (capitals)
  UpdateButtons();

  // set heading
  if (!m_strHeading.empty())
  {
    SetControlLabel(CTL_LABEL_HEADING, m_strHeading);
    SetControlVisible(CTL_LABEL_HEADING, true);
  }
  else
  {
    SetControlVisible(CTL_LABEL_HEADING, false);
  }

  kodi::gui::controls::CButton button(this, CTL_BUTTON_POS_SWAP);
  button.SetLabel(kodi::GetLocalizedString(30325));

  return false;
}

bool CBrowserDialogKeyboard::OnAction(int actionId, uint32_t buttoncode, wchar_t unicode)
{
  switch (actionId)
  {
    case ACTION_PREVIOUS_MENU:
      Close();
      return true;
    case KEY_VKEY:
    case XBMCVK_BACK:
      Backspace();
      return true;
    case ACTION_ENTER:
      if (GetFocusId() != CTL_BUTTON_DUMMY)
      {
        OnOK();
        return true;
      }
      break;
    case ACTION_SHIFT:
      OnShift();
      return true;
    case ACTION_MOVE_LEFT:
    case ACTION_MOVE_RIGHT:
    case ACTION_SELECT_ITEM:
      return false;
    case ACTION_SYMBOLS:
      OnSymbols();
      return true;
    case ACTION_MOVE_UP:
    case ACTION_MOVE_DOWN:
      if (GetFocusId() != CTL_BUTTON_DUMMY)
        return false;
    default:
      break;
  }

  if (m_client && buttoncode & KEY_VKEY)
  {
    CefKeyEvent key_event;
    key_event.modifiers = CSystemTranslator::ButtonCodeToModifier(buttoncode);
    key_event.windows_key_code = CSystemTranslator::ButtonCodeToKeyboardCode(buttoncode);
    key_event.native_key_code = 0;
    key_event.is_system_key = false;
    key_event.character = unicode;
    key_event.unmodified_character = CSystemTranslator::ButtonCodeToUnmodifiedCharacter(buttoncode);
    key_event.focus_on_editable_field = true;
    if (key_event.windows_key_code == VKEY_RETURN)
    {
      // We need to treat the enter key as a key press of character \r.  This
      // is apparently just how webkit handles it and what it expects.
      key_event.unmodified_character = '\r';
    }

    CefRefPtr<CefBrowserHost> host = m_client->GetBrowser()->GetHost();
    key_event.type = KEYEVENT_RAWKEYDOWN;
    host->SendKeyEvent(key_event);
    key_event.type = KEYEVENT_CHAR;
    host->SendKeyEvent(key_event);
    key_event.type = KEYEVENT_KEYUP;
    host->SendKeyEvent(key_event);
    return true;
  }

  return CWindow::OnAction(actionId, buttoncode, unicode);
}

bool CBrowserDialogKeyboard::OnClick(int controlId)
{
  switch (controlId)
  {
    case CTL_BUTTON_DONE:
    case CTL_BUTTON_DUMMY:
      OnOK();
      return true;
    case CTL_BUTTON_CANCEL:
      Close();
      return true;
    case CTL_BUTTON_BACKSPACE:
      Backspace();
      return true;
    case CTL_BUTTON_SPACE:
      Character(" ");
      return true;
    case CTL_BUTTON_SHIFT:
      OnShift();
      return true;
    case CTL_BUTTON_CAPS:
      if (m_keyType == LOWER)
        m_keyType = CAPS;
      else if (m_keyType == CAPS)
        m_keyType = LOWER;
      UpdateButtons();
      return true;
    case CTL_BUTTON_LAYOUT:
      OnLayout();
      return true;
    case CTL_BUTTON_SYMBOLS:
      OnSymbols();
      return true;
    case CTL_BUTTON_LEFT:
      MoveCursor(-1);
      return true;
    case CTL_BUTTON_RIGHT:
      MoveCursor(1);
      return true;
    case CTL_BUTTON_POS_SWAP:
      m_keyboardPos++;
      if (m_keyboardPos == 0)
        SetProperty("kbpos", "down");
      else if (m_keyboardPos == 1)
      {
        SetProperty("kbpos", "up");
        m_keyboardPos = -1;
      }
      return true;
    default:
    {
      kodi::gui::controls::CButton button(this, controlId);
      // Do not register input for buttons with id >= 500
      if (controlId < 500)
      {
        Character(button.GetLabel());
        // reset the shift keys
        if (m_bShift)
          OnShift();
        return false;
      }
      break;
    }
  }

  return false;
}

void CBrowserDialogKeyboard::UpdateButtons()
{
  SetControlSelected(CTL_BUTTON_SHIFT, m_bShift);
  SetControlSelected(CTL_BUTTON_CAPS, m_keyType == CAPS);
  SetControlSelected(CTL_BUTTON_SYMBOLS, m_keyType == SYMBOLS);

  unsigned int modifiers = STD_KB_MODIFIER_KEY_NONE;
  if ((m_keyType == CAPS && !m_bShift) || (m_keyType == LOWER && m_bShift))
    modifiers |= STD_KB_MODIFIER_KEY_SHIFT;
  if (m_keyType == SYMBOLS)
  {
    modifiers |= STD_KB_MODIFIER_KEY_SYMBOL;
    if (m_bShift)
      modifiers |= STD_KB_MODIFIER_KEY_SHIFT;
  }

  std::vector<std::vector<std::string>> layout;
  kodi::GetKeyboardLayout(modifiers, m_currentLayoutName, layout);

  SetControlLabel(CTL_BUTTON_LAYOUT, m_currentLayoutName);

  for (unsigned int row = 0; row < STD_KB_BUTTONS_MAX_ROWS; row++)
  {
    for (unsigned int column = 0; column < STD_KB_BUTTONS_PER_ROW; column++)
    {
      int buttonID = (row * BUTTONS_PER_ROW) + column + BUTTON_ID_OFFSET;
      std::string label = layout[row][column];
      SetControlLabel(buttonID, label);
      SetControlVisible(buttonID, !label.empty());
    }
  }
}

void CBrowserDialogKeyboard::Backspace()
{
  CefKeyEvent key_event;
  key_event.modifiers = 0;
  key_event.windows_key_code = VKEY_BACK;
  key_event.native_key_code = 0;
  key_event.is_system_key = false;
  key_event.character = '\b';
  key_event.unmodified_character = 0;
  key_event.focus_on_editable_field = true;

  CefRefPtr<CefBrowserHost> host = m_client->GetBrowser()->GetHost();
  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);
}

void CBrowserDialogKeyboard::OnOK()
{
  CefKeyEvent key_event;
  key_event.modifiers = VKEY_RETURN;
  key_event.windows_key_code = VKEY_RETURN;
  key_event.native_key_code = 0;
  key_event.is_system_key = false;
  key_event.unmodified_character = '\r';
  key_event.focus_on_editable_field = true;

  CefRefPtr<CefBrowserHost> host = m_client->GetBrowser()->GetHost();
  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);

  Close();
}

void CBrowserDialogKeyboard::OnSymbols()
{
  if (m_keyType == SYMBOLS)
    m_keyType = LOWER;
  else
    m_keyType = SYMBOLS;
  UpdateButtons();
}

void CBrowserDialogKeyboard::OnShift()
{
  m_bShift = !m_bShift;
  UpdateButtons();
}

void CBrowserDialogKeyboard::Character(const std::string& ch)
{
  if (ch.empty())
    return;

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert; // conversion between UTF-16 and UTF-8
  std::wstring dest = convert.from_bytes(ch); // convert UTF-8 std::string to UTF-16 std::wstring

  //std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
  //std::u16string dest = convert.from_bytes(ch);

  CefKeyEvent key_event;
  key_event.native_key_code = 0;
  key_event.is_system_key = false;
  key_event.character = dest.data()[0];
  key_event.focus_on_editable_field = true;
  if (key_event.windows_key_code == VKEY_RETURN)
  {
    // We need to treat the enter key as a key press of character \r.  This
    // is apparently just how webkit handles it and what it expects.
    key_event.unmodified_character = '\r';
  }

  CefRefPtr<CefBrowserHost> host = m_client->GetBrowser()->GetHost();
  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);
}

void CBrowserDialogKeyboard::OnLayout()
{
  std::string layout_name;
  kodi::ChangeKeyboardLayout(layout_name);
  if (m_currentLayoutName != layout_name)
    UpdateButtons();
}

void CBrowserDialogKeyboard::MoveCursor(int amount)
{
  CefKeyEvent key_event;
  key_event.modifiers = 0;
  key_event.windows_key_code = amount > 0 ? VKEY_RIGHT : VKEY_LEFT;
  key_event.native_key_code = 0;
  key_event.is_system_key = false;
  key_event.unmodified_character = 0;
  key_event.focus_on_editable_field = true;

  CefRefPtr<CefBrowserHost> host = m_client->GetBrowser()->GetHost();
  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);
}
