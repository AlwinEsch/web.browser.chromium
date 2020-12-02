/*
 *  Copyright (C) 2005-2020 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#ifndef C_API_GUI_CONTROLS_SPIN_H
#define C_API_GUI_CONTROLS_SPIN_H

#include "../definitions.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  //============================================================================
  /// @ingroup cpp_kodi_gui_windows_controls_CSpin
  /// @anchor AddonGUISpinControlType
  /// @brief The values here defines the used value format for steps on
  /// spin control.
  ///
  typedef enum AddonGUISpinControlType
  {
    /// One spin step interpreted as integer
    ADDON_SPIN_CONTROL_TYPE_INT = 1,
    /// One spin step interpreted as floating point value
    ADDON_SPIN_CONTROL_TYPE_FLOAT = 2,
    /// One spin step interpreted as text string
    ADDON_SPIN_CONTROL_TYPE_TEXT = 3,
    /// One spin step interpreted as a page change value
    ADDON_SPIN_CONTROL_TYPE_PAGE = 4
  } AddonGUISpinControlType;
  //----------------------------------------------------------------------------

  typedef struct AddonToKodiFuncTable_kodi_gui_control_spin
  {
    void (*set_visible)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle, bool visible);
    void (*set_enabled)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle, bool enabled);
    void (*set_text)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle, const char* text);
    void (*reset)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle);
    void (*set_type)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle, int type);
    void (*add_string_label)(KODI_HANDLE kodiBase,
                             KODI_GUI_CONTROL_HANDLE handle,
                             const char* label,
                             const char* value);
    void (*set_string_value)(KODI_HANDLE kodiBase,
                             KODI_GUI_CONTROL_HANDLE handle,
                             const char* value);
    char* (*get_string_value)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle);
    void (*add_int_label)(KODI_HANDLE kodiBase,
                          KODI_GUI_CONTROL_HANDLE handle,
                          const char* label,
                          int value);
    void (*set_int_range)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle, int start, int end);
    void (*set_int_value)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle, int value);
    int (*get_int_value)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle);
    void (*set_float_range)(KODI_HANDLE kodiBase,
                            KODI_GUI_CONTROL_HANDLE handle,
                            float start,
                            float end);
    void (*set_float_value)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle, float value);
    float (*get_float_value)(KODI_HANDLE kodiBase, KODI_GUI_CONTROL_HANDLE handle);
    void (*set_float_interval)(KODI_HANDLE kodiBase,
                               KODI_GUI_CONTROL_HANDLE handle,
                               float interval);
  } AddonToKodiFuncTable_kodi_gui_control_spin;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !C_API_GUI_CONTROLS_SPIN_H */
