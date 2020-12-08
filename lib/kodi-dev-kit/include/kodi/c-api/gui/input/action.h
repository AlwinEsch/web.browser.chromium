/*
 *  Copyright (C) 2005-2020 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#ifndef C_API_GUI_ACTION_H
#define C_API_GUI_ACTION_H

#include "action_ids.h"

#include <wchar.h>

#define ADDON_ACTION_AMOUNT_MAX 6 // Must be at least 6

struct addon_action_data
{
  ADDON_ACTION id;
  const char* name;
  float amount[ADDON_ACTION_AMOUNT_MAX];
  float repeat;
  unsigned int hold_time;
  unsigned int button_code;
  wchar_t unicode;
  const char* text;
};

#endif /* !C_API_GUI_ACTION_H */
