/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <string>

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace Keyboard
{

bool ShowAndGetInput(std::string& text,
                     const std::string& heading,
                     bool allowEmptyResult,
                     bool hiddenInput = false,
                     unsigned int autoCloseMs = 0);
bool ShowAndGetInput(std::string& text, bool allowEmptyResult, unsigned int autoCloseMs = 0);
bool ShowAndGetNewPassword(std::string& newPassword,
                           const std::string& heading,
                           bool allowEmptyResult,
                           unsigned int autoCloseMs = 0);
bool ShowAndGetNewPassword(std::string& newPassword, unsigned int autoCloseMs = 0);
bool ShowAndVerifyNewPassword(std::string& newPassword,
                              const std::string& heading,
                              bool allowEmptyResult,
                              unsigned int autoCloseMs = 0);
bool ShowAndVerifyNewPassword(std::string& newPassword, unsigned int autoCloseMs = 0);
int ShowAndVerifyPassword(std::string& password,
                          const std::string& heading,
                          int retries,
                          unsigned int autoCloseMs = 0);
bool ShowAndGetFilter(std::string& text, bool searching, unsigned int autoCloseMs = 0);
bool SendTextToActiveKeyboard(const std::string& text, bool closeKeyboard = false);
bool IsKeyboardActivated();

} /* namespace Keyboard */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
