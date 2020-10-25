/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "SystemTranslator.h"

#include "include/base/cef_basictypes.h"

#include <kodi/XBMC_vkeys.h>

typedef enum Modifier
{
  MODIFIER_CTRL  = 0x00010000,
  MODIFIER_SHIFT = 0x00020000,
  MODIFIER_ALT   = 0x00040000,
  MODIFIER_RALT  = 0x00080000,
  MODIFIER_SUPER = 0x00100000,
  MODIFIER_META  = 0x00200000,
  MODIFIER_LONG  = 0x01000000
} Modifier;

#define KEY_VKEY            0xF000 // a virtual key/functional key e.g. cursor left
#define KEY_ASCII           0xF100 // a printable character in the range of TRUE ASCII (from 0 to 127) // FIXME make it clean and pure unicode
#define KEY_UNICODE         0xF200 // another printable character whose range is not included in this KEY code

int CSystemTranslator::ButtonCodeToKeyboardCode(int code)
{
  uint8_t character = code & 0xFF;
  switch (character)
  {
    case XBMCVK_BACK:                 return VKEY_BACK;
    case XBMCVK_TAB:                  return VKEY_TAB;
    case XBMCVK_RETURN:               return VKEY_RETURN;
    case XBMCVK_ESCAPE:               return VKEY_ESCAPE;

    case XBMCVK_SPACE:                return VKEY_SPACE;
//     case XBMCVK_EXCLAIM:              return VKEY_;
//     case XBMCVK_QUOTEDBL:             return VKEY_;
//     case XBMCVK_HASH:                 return VKEY_;
//     case XBMCVK_DOLLAR:               return VKEY_;
//     case XBMCVK_PERCENT:              return VKEY_;
//     case XBMCVK_AMPERSAND:            return VKEY_;
//     case XBMCVK_QUOTE:                return VKEY_;
//     case XBMCVK_LEFTPAREN:            return VKEY_;
//     case XBMCVK_RIGHTPAREN:           return VKEY_;
    case XBMCVK_ASTERISK:             return VKEY_MULTIPLY;
    case XBMCVK_PLUS:                 return VKEY_OEM_PLUS;
    case XBMCVK_COMMA:                return VKEY_OEM_COMMA;
    case XBMCVK_MINUS:                return VKEY_OEM_MINUS;
    case XBMCVK_PERIOD:               return VKEY_OEM_PERIOD;
    case XBMCVK_SLASH:                return VKEY_DIVIDE;

    case XBMCVK_UP:                   return VKEY_UP;
    case XBMCVK_DOWN:                 return VKEY_DOWN;
    case XBMCVK_LEFT:                 return VKEY_LEFT;
    case XBMCVK_RIGHT:                return VKEY_RIGHT;
    case XBMCVK_PAGEUP:               return VKEY_PRIOR;
    case XBMCVK_PAGEDOWN:             return VKEY_NEXT;
    case XBMCVK_INSERT:               return VKEY_INSERT;
    case XBMCVK_DELETE:               return VKEY_DELETE;
    case XBMCVK_HOME:                 return VKEY_HOME;
    case XBMCVK_END:                  return VKEY_END;

    case XBMCVK_F1:                   return VKEY_F1;
    case XBMCVK_F2:                   return VKEY_F2;
    case XBMCVK_F3:                   return VKEY_F3;
    case XBMCVK_F4:                   return VKEY_F4;
    case XBMCVK_F5:                   return VKEY_F5;
    case XBMCVK_F6:                   return VKEY_F6;
    case XBMCVK_F7:                   return VKEY_F7;
    case XBMCVK_F8:                   return VKEY_F8;
    case XBMCVK_F9:                   return VKEY_F9;
    case XBMCVK_F10:                  return VKEY_F10;
    case XBMCVK_F11:                  return VKEY_F11;
    case XBMCVK_F12:                  return VKEY_F12;
    case XBMCVK_F13:                  return VKEY_F13;
    case XBMCVK_F14:                  return VKEY_F14;
    case XBMCVK_F15:                  return VKEY_F15;
    case XBMCVK_F16:                  return VKEY_F16;
    case XBMCVK_F17:                  return VKEY_F17;
    case XBMCVK_F18:                  return VKEY_F18;
    case XBMCVK_F19:                  return VKEY_F19;
    case XBMCVK_F20:                  return VKEY_F20;
    case XBMCVK_F21:                  return VKEY_F21;
    case XBMCVK_F22:                  return VKEY_F22;
    case XBMCVK_F23:                  return VKEY_F23;
    case XBMCVK_F24:                  return VKEY_F24;

    case XBMCVK_BROWSER_BACK:         return VKEY_BROWSER_BACK;
    case XBMCVK_BROWSER_FORWARD:      return VKEY_BROWSER_FORWARD;
    case XBMCVK_BROWSER_REFRESH:      return VKEY_BROWSER_REFRESH;
    case XBMCVK_BROWSER_STOP:         return VKEY_BROWSER_STOP;
    case XBMCVK_BROWSER_SEARCH:       return VKEY_BROWSER_SEARCH;
    case XBMCVK_BROWSER_FAVORITES:    return VKEY_BROWSER_FAVORITES;
    case XBMCVK_BROWSER_HOME:         return VKEY_BROWSER_HOME;
    case XBMCVK_VOLUME_MUTE:          return VKEY_VOLUME_MUTE;
    case XBMCVK_VOLUME_DOWN:          return VKEY_VOLUME_DOWN;
    case XBMCVK_VOLUME_UP:            return VKEY_VOLUME_UP;
    case XBMCVK_MEDIA_NEXT_TRACK:     return VKEY_MEDIA_NEXT_TRACK;
    case XBMCVK_MEDIA_PREV_TRACK:     return VKEY_MEDIA_PREV_TRACK;
    case XBMCVK_MEDIA_STOP:           return VKEY_MEDIA_STOP;
    case XBMCVK_MEDIA_PLAY_PAUSE:     return VKEY_MEDIA_PLAY_PAUSE;
    case XBMCVK_LAUNCH_MAIL:          return VKEY_MEDIA_LAUNCH_MAIL;
    case XBMCVK_LAUNCH_MEDIA_SELECT:  return VKEY_MEDIA_LAUNCH_MEDIA_SELECT;
    case XBMCVK_LAUNCH_APP1:          return VKEY_MEDIA_LAUNCH_APP1;
    case XBMCVK_LAUNCH_APP2:          return VKEY_MEDIA_LAUNCH_APP2;
//     case XBMCVK_LAUNCH_FILE_BROWSER:  return VKEY_;
//     case XBMCVK_LAUNCH_MEDIA_CENTER:  return VKEY_;
//     case XBMCVK_MEDIA_REWIND:         return VKEY_;
//     case XBMCVK_MEDIA_FASTFORWARD:    return VKEY_;
//     case XBMCVK_MEDIA_RECORD:         return VKEY_;

    case XBMCVK_LCONTROL:             return VKEY_LCONTROL;
    case XBMCVK_RCONTROL:             return VKEY_RCONTROL;
    case XBMCVK_LSHIFT:               return VKEY_LSHIFT;
    case XBMCVK_RSHIFT:               return VKEY_RSHIFT;
    case XBMCVK_LMENU:                return VKEY_LMENU;
    case XBMCVK_RMENU:                return VKEY_RMENU;
    case XBMCVK_LWIN:                 return VKEY_LWIN;
    case XBMCVK_RWIN:                 return VKEY_RWIN;
    case XBMCVK_MENU:                 return VKEY_MENU;
    case XBMCVK_NUMLOCK:              return VKEY_NUMLOCK;

    default:
      break;
  }
  return character;
}

char16 CSystemTranslator::ButtonCodeToUnmodifiedCharacter(int code)
{
  uint8_t character = code & 0xFF;
  switch (character)
  {
    case XBMCVK_A:
      return 'a';
    case XBMCVK_B:
      return 'b';
    case XBMCVK_C:
      return 'c';
    case XBMCVK_D:
      return 'd';
    case XBMCVK_E:
      return 'e';
    case XBMCVK_F:
      return 'f';
    case XBMCVK_G:
      return 'g';
    case XBMCVK_H:
      return 'h';
    case XBMCVK_I:
      return 'i';
    case XBMCVK_J:
      return 'j';
    case XBMCVK_K:
      return 'k';
    case XBMCVK_L:
      return 'l';
    case XBMCVK_M:
      return 'm';
    case XBMCVK_N:
      return 'n';
    case XBMCVK_O:
      return 'o';
    case XBMCVK_P:
      return 'p';
    case XBMCVK_Q:
      return 'q';
    case XBMCVK_R:
      return 'r';
    case XBMCVK_S:
      return 's';
    case XBMCVK_T:
      return 't';
    case XBMCVK_U:
      return 'u';
    case XBMCVK_V:
      return 'v';
    case XBMCVK_W:
      return 'w';
    case XBMCVK_X:
      return 'x';
    case XBMCVK_Y:
      return 'y';
    case XBMCVK_Z:
      return 'z';
    case XBMCVK_NUMPAD0:
    case XBMCVK_0:
      return '0';
    case XBMCVK_NUMPAD1:
    case XBMCVK_1:
      return '1';
    case XBMCVK_NUMPAD2:
    case XBMCVK_2:
      return '2';
    case XBMCVK_NUMPAD3:
    case XBMCVK_3:
      return '3';
    case XBMCVK_NUMPAD4:
    case XBMCVK_4:
      return '4';
    case XBMCVK_NUMPAD5:
    case XBMCVK_5:
      return '5';
    case XBMCVK_NUMPAD6:
    case XBMCVK_6:
      return '6';
    case XBMCVK_NUMPAD7:
    case XBMCVK_7:
      return '7';
    case XBMCVK_NUMPAD8:
    case XBMCVK_8:
      return '8';
    case XBMCVK_NUMPAD9:
    case XBMCVK_9:
      return '9';

    default:
      break;
  }
  return 0;
}

uint32_t CSystemTranslator::ButtonCodeToModifier(int code)
{
  uint32_t flags = 0;
  if (code & MODIFIER_CTRL)
    flags |= EVENTFLAG_CONTROL_DOWN;
  if (code & MODIFIER_SHIFT)
    flags |= EVENTFLAG_SHIFT_DOWN;
  if (code & MODIFIER_ALT)
    flags |= EVENTFLAG_ALT_DOWN;
  return flags;
}
