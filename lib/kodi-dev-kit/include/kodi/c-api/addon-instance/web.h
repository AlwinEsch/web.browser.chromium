/*
 *  Copyright (C) 2005-2020 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#ifndef C_API_ADDONINSTANCE_WEB_H
#define C_API_ADDONINSTANCE_WEB_H

#include "../addon_base.h"

#define WEB_KODI_CONTROL void*

#define WEB_TYPE_ID_BROWSER 0

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  //============================================================================
  /// @brief Web add-on error codes
  ///
  typedef enum
  {
    /// @brief no error occurred
    WEB_ADDON_ERROR_NO_ERROR = 0,

    /// @brief no error occurred, but existing part reopened
    WEB_ADDON_ERROR_NO_ERROR_REOPEN = -1,

    /// @brief an unknown error occurred
    WEB_ADDON_ERROR_UNKNOWN = -2,

    /// @brief the method that KODI called is not implemented by the add-on
    WEB_ADDON_ERROR_NOT_IMPLEMENTED = -3,

    /// @brief the command was rejected by the addon
    WEB_ADDON_ERROR_REJECTED = -4,

    /// @brief the parameters of the method that was called are invalid for this operation
    WEB_ADDON_ERROR_INVALID_PARAMETERS = -5,

    /// @brief the command failed
    WEB_ADDON_ERROR_FAILED = -6
  } WEB_ADDON_ERROR;
  //----------------------------------------------------------------------------

  //============================================================================
  /// @brief
  typedef enum WEB_KEY_MOD
  {
    /// @brief
    WEB_KEY_MOD_NONE = 0x0000,

    /// @brief
    WEB_KEY_MOD_SHIFT = 0x0001,

    /// @brief
    WEB_KEY_MOD_CTRL = 0x0002,

    /// @brief
    WEB_KEY_MOD_ALT = 0x0004,

    /// @brief
    WEB_KEY_MOD_META = 0x0008,

    /// @brief
    WEB_KEY_MOD_SUPER = 0x0010,

    /// @brief
    WEB_KEY_MOD_NUMLOCK = 0x0100,

    /// @brief
    WEB_KEY_MOD_CAPSLOCK = 0x0200,

    /// @brief
    WEB_KEY_MOD_SCROLLOCK = 0x0400,
  } WEB_KEY_MOD;
  //----------------------------------------------------------------------------

  typedef struct WEB_KEYBOARD_INPUT_EVENT
  {
    const char* controller_id;
    const char* feature_name;
    bool pressed;
    uint32_t unicode;
    WEB_KEY_MOD modifiers;
  } WEB_KEYBOARD_INPUT_EVENT;

  /*!
   * @brief Type defination structure
   */
  typedef struct WEB_ADDON_VARIOUS_TYPE
  {
    const char* strName;
    int iAddonInternalId;
    int iType;
  } WEB_ADDON_VARIOUS_TYPE;

  /*!
   * @brief Web addon gui control handle data
   */
  typedef struct WEB_ADDON_GUI_PROPS
  {
    /*!
     * @brief identify name of related control to know on next open
     */
    char strName[80];

    /*!
     * @brief Used render device, NULL for OpenGL and only be used for DirectX
     */
    void* pDevice;

    /*!
     * @brief For GUI control used render positions and sizes
     */
    float fXPos;
    float fYPos;
    float fWidth;
    float fHeight;
    float fPixelRatio;
    float fFPS;

    /*!
     * @brief For GUI control used positions and sizes on skin
     */
    float fSkinXPos;
    float fSkinYPos;
    float fSkinWidth;
    float fSkinHeight;

    /*!
     * @brief If set the opened control becomes handled transparent with the
     * color value given on iBackgroundColorARGB
     */
    bool bUseTransparentBackground;

    /*!
     * @brief The wanted background color on opened control.
     *
     * If bUseTransparentBackground is false it is the background during empty
     * control (Webside not loaded)
     * If bUseTransparentBackground is true then it set the transparency color
     * of the handled control
     */
    uint32_t iBackgroundColorARGB;

    /*!
     * @brief The id's from control outside of the web GUI render control.
     * Used with OnAction to inform about next GUI item to select if inside
     * control a point comes to the end.
     */
    int iGUIItemLeft;
    int iGUIItemRight;
    int iGUIItemTop;
    int iGUIItemBottom;
    int iGUIItemBack;

    /*!
     * @brief Identifier of the control on Kodi. Required to have on callbacks,
     * must set by callback functions on ADDON_HANDLE::dataAddress with this!
     */
    WEB_KODI_CONTROL pControlIdent;
  } WEB_ADDON_GUI_PROPS;

  struct AddonInstance_WebAddon;
  struct addon_action_data;

  typedef struct WEB_CONTROL_HANDLE
  {
    struct ADDON_HANDLE_STRUCT ownHandle;
    const struct WEB_ADDON_GUI_PROPS* props;
    const struct AddonInstance_WebAddon* instanceData;
  } WEB_CONTROL_HANDLE;

  typedef struct AddonProps_WebAddon
  {
    const char* strUserPath; /*!< path to the user profile */
    const char* strAddonLibPath; /*!< path to this add-on */
    const char* strAddonSharePath; /*!< path to this add-on */
  } AddonProps_WebAddon;

  typedef struct AddonToKodiFuncTable_WebAddon
  {
    KODI_HANDLE kodiInstance;

    void(*inhibit_shutdown)(KODI_HANDLE kodiInstance,
                            bool inhibit);
    void(*inhibit_screensaver)(KODI_HANDLE kodiInstance,
                               bool inhibit);
    bool (*is_muted)(KODI_HANDLE kodiInstance);
    void (*control_set_control_ready)(KODI_HANDLE kodiInstance,
                                      WEB_KODI_CONTROL handle,
                                      bool ready);
    void (*control_set_opened_address)(KODI_HANDLE kodiInstance,
                                       WEB_KODI_CONTROL handle,
                                       const char* title);
    void (*control_set_opened_title)(KODI_HANDLE kodiInstance,
                                     WEB_KODI_CONTROL handle,
                                     const char* title);
    void (*control_set_icon_url)(KODI_HANDLE kodiInstance,
                                 WEB_KODI_CONTROL handle,
                                 const char* icon);
    void (*control_set_fullscreen)(KODI_HANDLE kodiInstance,
                                   WEB_KODI_CONTROL handle,
                                   bool fullscreen);
    void (*control_set_loading_state)(KODI_HANDLE kodiInstance,
                                      WEB_KODI_CONTROL handle,
                                      bool isLoading,
                                      bool canGoBack,
                                      bool canGoForward);
    void (*control_set_tooltip)(KODI_HANDLE kodiInstance,
                                WEB_KODI_CONTROL handle,
                                const char* tooltip);
    void (*control_set_status_message)(KODI_HANDLE kodiInstance,
                                       WEB_KODI_CONTROL handle,
                                       const char* status);
    void (*control_request_open_site_in_new_tab)(KODI_HANDLE kodiInstance,
                                                 WEB_KODI_CONTROL handle,
                                                 const char* url);
  } AddonToKodiFuncTable_WebAddon;

  typedef struct KodiToAddonFuncTable_WebAddon
  {
    KODI_HANDLE addonInstance;

    WEB_ADDON_ERROR(__cdecl* start_instance)(const struct AddonInstance_WebAddon* instance);
    void(__cdecl* stop_instance)(const struct AddonInstance_WebAddon* instance);
    bool(__cdecl* main_initialize)(const struct AddonInstance_WebAddon* instance);
    void(__cdecl* main_loop)(const struct AddonInstance_WebAddon* instance);
    void(__cdecl* main_shutdown)(const struct AddonInstance_WebAddon* instance);
    void(__cdecl* set_mute)(const struct AddonInstance_WebAddon* instance, bool mute);
    bool(__cdecl* set_language)(const struct AddonInstance_WebAddon* instance,
                                const char* language);

    WEB_ADDON_ERROR(__cdecl* create_control)
    (const struct AddonInstance_WebAddon* instance,
     const WEB_ADDON_GUI_PROPS* props,
     const char* start_url,
     ADDON_HANDLE handle);
    bool(__cdecl* destroy_control)(const struct AddonInstance_WebAddon* instance,
                                   const ADDON_HANDLE handle,
                                   bool complete);
    void(__cdecl* control_render)(const struct AddonInstance_WebAddon* instance,
                                  const ADDON_HANDLE handle);
    bool(__cdecl* control_dirty)(const struct AddonInstance_WebAddon* instance,
                                 const ADDON_HANDLE handle);
    bool(__cdecl* control_on_init)(const struct AddonInstance_WebAddon* instance,
                                   const ADDON_HANDLE handle);
    bool(__cdecl* control_on_action)(const struct AddonInstance_WebAddon* instance,
                                     const ADDON_HANDLE handle,
                                     const struct addon_action_data* action,
                                     int* nextItem);
    bool(__cdecl* control_on_mouse_event)(const struct AddonInstance_WebAddon* instance,
                                          const ADDON_HANDLE handle,
                                          int id,
                                          double x,
                                          double y,
                                          double offsetX,
                                          double offsetY,
                                          int state);

    bool(__cdecl* control_open_website)(const struct AddonInstance_WebAddon* instance,
                                        const ADDON_HANDLE handle,
                                        const char* url);
    bool(__cdecl* control_get_history)(const struct AddonInstance_WebAddon* instance,
                                       const ADDON_HANDLE handle,
                                       char*** list,
                                       unsigned int* entries,
                                       bool behind_current);
    void(__cdecl* control_get_history_clear)(const struct AddonInstance_WebAddon* instance,
                                             const ADDON_HANDLE handle,
                                             char** list,
                                             unsigned int entries);
    void(__cdecl* control_search_text)(const struct AddonInstance_WebAddon* instance,
                                       const ADDON_HANDLE handle,
                                       const char* text,
                                       bool forward,
                                       bool matchCase,
                                       bool findNext);
    void(__cdecl* control_stop_search)(const struct AddonInstance_WebAddon* instance,
                                       const ADDON_HANDLE handle,
                                       bool clearSelection);
    void(__cdecl* control_web_cmd_reload)(const struct AddonInstance_WebAddon* instance,
                                          const ADDON_HANDLE handle);
    void(__cdecl* control_web_cmd_stop_load)(const struct AddonInstance_WebAddon* instance,
                                             const ADDON_HANDLE handle);
    void(__cdecl* control_web_cmd_nav_back)(const struct AddonInstance_WebAddon* instance,
                                            const ADDON_HANDLE handle);
    void(__cdecl* control_web_cmd_nav_forward)(const struct AddonInstance_WebAddon* instance,
                                               const ADDON_HANDLE handle);
    void(__cdecl* control_web_open_own_context_menu)(const struct AddonInstance_WebAddon* instance,
                                                     const ADDON_HANDLE handle);
    void(__cdecl* control_screen_size_change)(const struct AddonInstance_WebAddon* instance,
                                              const ADDON_HANDLE handle,
                                              float x,
                                              float y,
                                              float width,
                                              float height,
                                              bool fullscreen);
  } KodiToAddonFuncTable_WebAddon;

  typedef struct AddonInstance_WebAddon
  {
    struct AddonProps_WebAddon* props;
    struct AddonToKodiFuncTable_WebAddon* toKodi;
    struct KodiToAddonFuncTable_WebAddon* toAddon;
  } AddonInstance_WebAddon;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !C_API_ADDONINSTANCE_WEB_H */
