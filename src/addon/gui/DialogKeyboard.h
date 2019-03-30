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

#include "include/cef_base.h"

#include <kodi/gui/Window.h>

enum KEYBOARD {CAPS, LOWER, SYMBOLS};

class CWebBrowserClient;

class CBrowserDialogKeyboard : public kodi::gui::CWindow
{
public:
  CBrowserDialogKeyboard();

  void Show(CWebBrowserClient* client, cef_text_input_mode_t input_mode);

  bool OnInit() override;
  bool OnAction(int actionId, uint32_t buttoncode, wchar_t unicode) override;
  bool OnClick(int controlId) override;

private:
  void UpdateButtons();
  void Backspace();
  void OnOK();
  void OnSymbols();
  void OnShift();
  void Character(const std::string& ch);
  void NormalCharacter(const std::string& ch);
  void OnLayout();
  void MoveCursor(int amount);

  CWebBrowserClient* m_client = nullptr;
  cef_text_input_mode_t m_input_mode = CEF_TEXT_INPUT_MODE_DEFAULT;
  int m_keyboardPos = 0;
  std::string m_currentLayoutName;
  std::string m_strHeading;
  KEYBOARD m_keyType = LOWER;
  bool m_bShift = false;
};
