// /*
//  *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
//  *
//  *  SPDX-License-Identifier: GPL-2.0-or-later
//  *  See LICENSE.md for more information.
//  */
//
// #pragma once
//
// #include <stdint.h>
// #include <string>
// #include <tuple>
//
// typedef enum funcIdKodi : int
// {
//   interface_CreateForNewThread,
//   kodi_Log,
//   kodi_GetAddonPath,
//   kodi_GetBaseUserPath,
//   kodi_GetTempAddonPath,
//   kodi_GetLocalizedString,
//   kodi_GetSettingBoolean,
//   kodi_SetSettingBoolean,
//   kodi_GetSettingInt,
//   kodi_SetSettingInt,
//   kodi_GetSettingString,
//   kodi_SetSettingString,
//   kodi_GetLanguage,
//   kodi_QueueNotification,
//   kodi_GetAddonInfo,
//
//   kodi_gui_Lock,
//   kodi_gui_Unlock,
//   kodi_gui_GetScreenHeight,
//   kodi_gui_GetScreenWidth,
//   kodi_gui_GetVideoResolution,
//   kodi_gui_GetCurrentWindowDialogId,
//   kodi_gui_GetCurrentWindowId,
//   kodi_gui_GetHWContext,
//
//   kodi_gui_CListItem_create,
//   kodi_gui_CListItem_destroy,
//   kodi_gui_CListItem_GetLabel,
//   kodi_gui_CListItem_SetLabel,
//   kodi_gui_CListItem_GetLabel2,
//   kodi_gui_CListItem_SetLabel2,
//   kodi_gui_CListItem_GetArt,
//   kodi_gui_CListItem_SetArt,
//   kodi_gui_CListItem_GetPath,
//   kodi_gui_CListItem_SetPath,
//   kodi_gui_CListItem_GetProperty,
//   kodi_gui_CListItem_SetProperty,
//   kodi_gui_CListItem_Select,
//   kodi_gui_CListItem_IsSelected,
//
//
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::controls::CButton
//   //@{
//   kodi_gui_controls_CButton_create,
//   kodi_gui_controls_CButton_destroy,
//   kodi_gui_controls_CButton_SetVisible,
//   kodi_gui_controls_CButton_SetEnabled,
//   kodi_gui_controls_CButton_SetLabel,
//   kodi_gui_controls_CButton_GetLabel,
//   kodi_gui_controls_CButton_SetLabel2,
//   kodi_gui_controls_CButton_GetLabel2,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::ContextMenu
//   //@{
//   kodi_gui_dialogs_ContextMenu_Show,
//   kodi_gui_dialogs_ContextMenu_Show2,
//   kodi_gui_dialogs_ContextMenu_Show3,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::ExtendedProgress
//   //@{
//   kodi_gui_dialogs_ExtendedProgress_create,
//   kodi_gui_dialogs_ExtendedProgress_destroy,
//   kodi_gui_dialogs_ExtendedProgress_Title,
//   kodi_gui_dialogs_ExtendedProgress_SetTitle,
//   kodi_gui_dialogs_ExtendedProgress_Text,
//   kodi_gui_dialogs_ExtendedProgress_SetText,
//   kodi_gui_dialogs_ExtendedProgress_IsFinished,
//   kodi_gui_dialogs_ExtendedProgress_MarkFinished,
//   kodi_gui_dialogs_ExtendedProgress_Percentage,
//   kodi_gui_dialogs_ExtendedProgress_SetPercentage,
//   kodi_gui_dialogs_ExtendedProgress_SetProgress,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::FileBrowser
//   //@{
//   kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory,
//   kodi_gui_dialogs_FileBrowser_ShowAndGetFile,
//   kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir,
//   kodi_gui_dialogs_FileBrowser_ShowAndGetFileList,
//   kodi_gui_dialogs_FileBrowser_ShowAndGetSource,
//   kodi_gui_dialogs_FileBrowser_ShowAndGetImage,
//   kodi_gui_dialogs_FileBrowser_ShowAndGetImageList,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::Keyboard
//   //@{
//   kodi_gui_dialogs_Keyboard_ShowAndGetInput,
//   kodi_gui_dialogs_Keyboard_ShowAndGetInput2,
//   kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword,
//   kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2,
//   kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword,
//   kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2,
//   kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword,
//   kodi_gui_dialogs_Keyboard_ShowAndGetFilter,
//   kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard,
//   kodi_gui_dialogs_Keyboard_IsKeyboardActivated,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::Numeric
//   //@{
//   kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword,
//   kodi_gui_dialogs_Numeric_ShowAndVerifyPassword,
//   kodi_gui_dialogs_Numeric_ShowAndVerifyInput,
//   kodi_gui_dialogs_Numeric_ShowAndGetTime,
//   kodi_gui_dialogs_Numeric_ShowAndGetDate,
//   kodi_gui_dialogs_Numeric_ShowAndGetIPAddress,
//   kodi_gui_dialogs_Numeric_ShowAndGetNumber,
//   kodi_gui_dialogs_Numeric_ShowAndGetSeconds,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::OK
//   //@{
//   kodi_gui_dialogs_OK_ShowAndGetInput,
//   kodi_gui_dialogs_OK_ShowAndGetInput2,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::CProgress
//   //@{
//   kodi_gui_dialogs_CProgress_create,
//   kodi_gui_dialogs_CProgress_destroy,
//   kodi_gui_dialogs_CProgress_Open,
//   kodi_gui_dialogs_CProgress_SetHeading,
//   kodi_gui_dialogs_CProgress_SetLine,
//   kodi_gui_dialogs_CProgress_SetCanCancel,
//   kodi_gui_dialogs_CProgress_IsCanceled,
//   kodi_gui_dialogs_CProgress_SetPercentage,
//   kodi_gui_dialogs_CProgress_GetPercentage,
//   kodi_gui_dialogs_CProgress_ShowProgressBar,
//   kodi_gui_dialogs_CProgress_SetProgressMax,
//   kodi_gui_dialogs_CProgress_SetProgressAdvance,
//   kodi_gui_dialogs_CProgress_Abort,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::Select
//   //@{
//   kodi_gui_dialogs_Select_Show,
//   kodi_gui_dialogs_Select_Show2,
//   kodi_gui_dialogs_Select_ShowMultiSelect,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::TextViewer
//   //@{
//   kodi_gui_dialogs_TextViewer_Show,
//   //@}
//   //----------------------------------------------------------------------------
//   // kodi::gui::dialogs::YesNo
//   //@{
//   kodi_gui_dialogs_YesNo_ShowAndGetInput,
//   kodi_gui_dialogs_YesNo_ShowAndGetInput2,
//   kodi_gui_dialogs_YesNo_ShowAndGetInput3,
//   //@}
//
//
//   kodi_vfs_StatFile,
//   kodi_vfs_CopyFile,
//   kodi_vfs_FileExists,
//   kodi_vfs_TranslateSpecialProtocol,
//   kodi_vfs_CreateDirectory,
//   kodi_vfs_GetDirectory,
//   kodi_vfs_CFile_construct,
//   kodi_vfs_CFile_destruct,
//   kodi_vfs_CFile_OpenFile,
//   kodi_vfs_CFile_OpenFileForWrite,
//   kodi_vfs_CFile_Close,
//   kodi_vfs_CFile_Read,
//   kodi_vfs_CFile_ReadLine,
//   kodi_vfs_CFile_Write,
//   instance_web_start_done,
//   instance_web_inhibit_shutdown,
//   instance_web_inhibit_screensaver,
//   instance_web_user_path,
//   instance_web_addon_lib_path,
//   instance_web_addon_share_path,
//   instance_web_is_muted,
//   instance_control_SetControlReady,
//   instance_control_SetOpenedAddress,
//   instance_control_SetOpenedTitle,
//   instance_control_SetIconURL,
//   instance_control_SetFullscreen,
//   instance_control_SetLoadingState,
//   instance_control_SetTooltip,
//   instance_control_SetStatusMessage,
//   instance_control_RequestOpenSiteInNewTab,
//   instance_control_GetDataIdentifier,
//   instance_control_GetName,
//   instance_control_GetDevice,
//   instance_control_GetXPos,
//   instance_control_GetYPos,
//   instance_control_GetWidth,
//   instance_control_GetHeight,
//   instance_control_GetPixelRatio,
//   instance_control_GetFPS,
//   instance_control_GetSkinXPos,
//   instance_control_GetSkinYPos,
//   instance_control_GetSkinWidth,
//   instance_control_GetSkinHeight,
//   instance_control_UseTransparentBackground,
//   instance_control_GetBackgroundColorARGB,
//   instance_control_GetGUIItemLeft,
//   instance_control_GetGUIItemRight,
//   instance_control_GetGUIItemTop,
//   instance_control_GetGUIItemBottom,
//   instance_control_GetGUIItemBack,
// } funcIdKodi;
//
// typedef std::tuple<std::string> msg_parent_interface_CreateForNewThread;
// typedef std::tuple<int, std::string> msg_parent_kodi_Log;
// typedef std::tuple<std::string> stream_msg_kodi_GetAddonPath;
// typedef std::tuple<std::string> stream_msg_kodi_GetBaseUserPath;
// typedef std::tuple<std::string> stream_msg_kodi_GetTempAddonPath;
// typedef std::tuple<uint32_t, std::string> stream_msg_kodi_GetLocalizedString;
// typedef std::tuple<std::string, bool> stream_msg_kodi_GetSettingBoolean;
// typedef std::tuple<std::string, bool> stream_msg_kodi_SetSettingBoolean;
// typedef std::tuple<std::string, int> stream_msg_kodi_GetSettingInt;
// typedef std::tuple<std::string, int> stream_msg_kodi_SetSettingInt;
// typedef std::tuple<std::string, std::string> stream_msg_kodi_GetSettingString;
// typedef std::tuple<std::string, std::string> stream_msg_kodi_SetSettingString;
// typedef std::tuple<uint32_t, bool> stream_msg_kodi_GetLanguage;
// typedef std::tuple<int32_t, std::string, std::string, std::string, unsigned int, bool, unsigned int>
//     stream_msg_kodi_QueueNotification;
// typedef std::tuple<std::string> stream_msg_kodi_GetAddonInfo;
//
// //==============================================================================
// // kodi::gui
// //@{
//
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_Lock;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_Unlock;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_GetScreenHeight;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_GetScreenWidth;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_GetVideoResolution;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_GetCurrentWindowDialogId;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_GetCurrentWindowId;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_GetHWContext;
//
// //------------------------------------------------------------------------------
// // kodi::gui::CListItem
// //@{
// typedef std::tuple<uint64_t, std::string, std::string, std::string> msg_parent_kodi_gui_CListItem_create;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_CListItem_destroy;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_CListItem_GetLabel;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_CListItem_SetLabel;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_CListItem_GetLabel2;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_CListItem_SetLabel2;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_CListItem_GetArt;
// typedef std::tuple<uint64_t, std::string, std::string> msg_parent_kodi_gui_CListItem_SetArt;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_CListItem_GetPath;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_CListItem_SetPath;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_CListItem_GetProperty;
// typedef std::tuple<uint64_t, std::string, std::string> msg_parent_kodi_gui_CListItem_SetProperty;
// typedef std::tuple<uint64_t, bool> msg_parent_kodi_gui_CListItem_Select;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_CListItem_IsSelected;
// //@}
//
// //------------------------------------------------------------------------------
// // kodi::gui::CWindow
// //@{
//
// //@}
// //______________________________________________________________________________
// //------------------------------------------------------------------------------
// // kodi::gui::controls
// //@{
// //------------------------------------------------------------------------------
// // kodi::gui::controls::CButton
// //@{
// typedef std::tuple<uint64_t, uint64_t> msg_parent_kodi_gui_controls_CButton_create;
// typedef std::tuple<uint64_t, uint64_t> msg_parent_kodi_gui_controls_CButton_destroy;
// typedef std::tuple<uint64_t, uint64_t, bool> msg_parent_kodi_gui_controls_CButton_SetVisible;
// typedef std::tuple<uint64_t, uint64_t, bool> msg_parent_kodi_gui_controls_CButton_SetEnabled;
// typedef std::tuple<uint64_t, uint64_t, std::string> msg_parent_kodi_gui_controls_CButton_SetLabel;
// typedef std::tuple<uint64_t, uint64_t> msg_parent_kodi_gui_controls_CButton_GetLabel;
// typedef std::tuple<uint64_t, uint64_t, std::string> msg_parent_kodi_gui_controls_CButton_SetLabel2;
// typedef std::tuple<uint64_t, uint64_t> msg_parent_kodi_gui_controls_CButton_GetLabel2;
// //@}
// //@}
// //______________________________________________________________________________
// //------------------------------------------------------------------------------
// // kodi::gui::dialogs
// //@{
// //------------------------------------------------------------------------------
// // kodi::gui::dialogs::ContextMenu
// //@{
// typedef std::tuple<std::string, std::vector<std::string>>
//     stream_msg_kodi_gui_dialogs_ContextMenu_Show;
// typedef std::tuple<std::string, std::vector<std::pair<std::string, std::string>>>
//     stream_msg_kodi_gui_dialogs_ContextMenu_Show2;
// typedef std::tuple<std::string, std::vector<std::pair<int, std::string>>>
//     stream_msg_kodi_gui_dialogs_ContextMenu_Show3;
// //@}
// //------------------------------------------------------------------------------
// // kodi::gui::dialogs::ExtendedProgress
// //@{
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_ExtendedProgress_create;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_ExtendedProgress_destroy;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_ExtendedProgress_Title;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_dialogs_ExtendedProgress_SetTitle;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_ExtendedProgress_Text;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_dialogs_ExtendedProgress_SetText;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_ExtendedProgress_IsFinished;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_ExtendedProgress_MarkFinished;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_ExtendedProgress_Percentage;
// typedef std::tuple<uint64_t, float> msg_parent_kodi_gui_dialogs_ExtendedProgress_SetPercentage;
// typedef std::tuple<uint64_t, int, int> msg_parent_kodi_gui_dialogs_ExtendedProgress_SetProgress;
// //@}
// //------------------------------------------------------------------------------
// // kodi::gui::dialogs::FileBrowser
// //@{
// typedef std::tuple<std::string, std::string, std::string, bool>
//     stream_msg_kodi_gui_dialogs_FileBrowser_ShowAndGetDirectory;
// typedef std::tuple<std::string, std::string, std::string, std::string, bool, bool>
//     stream_msg_kodi_gui_dialogs_FileBrowser_ShowAndGetFile;
// typedef std::tuple<std::string, std::string, std::string, std::string, bool, bool, bool>
//     stream_msg_kodi_gui_dialogs_FileBrowser_ShowAndGetFileFromDir;
// typedef std::tuple<std::string, std::string, std::string, std::vector<std::string>, bool, bool>
//     stream_msg_kodi_gui_dialogs_FileBrowser_ShowAndGetFileList;
// typedef std::tuple<std::string, bool, std::string, std::string>
//     stream_msg_kodi_gui_dialogs_FileBrowser_ShowAndGetSource;
// typedef std::tuple<std::string, std::string, std::string>
//     stream_msg_kodi_gui_dialogs_FileBrowser_ShowAndGetImage;
// typedef std::tuple<std::string, std::string, std::vector<std::string>>
//     stream_msg_kodi_gui_dialogs_FileBrowser_ShowAndGetImageList;
// typedef std::tuple<std::string, std::string, bool, bool, unsigned int>
// //@}
// //------------------------------------------------------------------------------
// // kodi::gui::dialogs::Keyboard
// //@{
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndGetInput;
// typedef std::tuple<std::string, bool, unsigned int>
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndGetInput2;
// typedef std::tuple<std::string, std::string, bool, unsigned int>
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword;
// typedef std::tuple<std::string, unsigned int>
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndGetNewPassword2;
// typedef std::tuple<std::string, std::string, bool, unsigned int>
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword;
// typedef std::tuple<std::string, unsigned int>
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndVerifyNewPassword2;
// typedef std::tuple<std::string, std::string, int, unsigned int>
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndVerifyPassword;
// typedef std::tuple<std::string, bool, unsigned int>
//     stream_msg_kodi_gui_dialogs_Keyboard_ShowAndGetFilter;
// typedef std::tuple<std::string, bool> stream_msg_kodi_gui_dialogs_Keyboard_SendTextToActiveKeyboard;
// typedef std::tuple<std::string> stream_msg_kodi_gui_dialogs_Keyboard_IsKeyboardActivated;
// //@}
// //------------------------------------------------------------------------------
// // kodi::gui::dialogs::Numeric
// //@{
// typedef std::tuple<std::string> msg_parent_kodi_gui_dialogs_Numeric_ShowAndVerifyNewPassword;
// typedef std::tuple<std::string, std::string, int> msg_parent_kodi_gui_dialogs_Numeric_ShowAndVerifyPassword;
// typedef std::tuple<std::string, std::string, bool> msg_parent_kodi_gui_dialogs_Numeric_ShowAndVerifyInput;
// typedef std::tuple<int, int, int, int, int, int, int, int, int, long int, std::string, std::string> msg_parent_kodi_gui_dialogs_Numeric_ShowAndGetTime;
// typedef std::tuple<int, int, int, int, int, int, int, int, int, long int, std::string, std::string> msg_parent_kodi_gui_dialogs_Numeric_ShowAndGetDate;
// typedef std::tuple<std::string, std::string> msg_parent_kodi_gui_dialogs_Numeric_ShowAndGetIPAddress;
// typedef std::tuple<std::string, std::string, unsigned int> msg_parent_kodi_gui_dialogs_Numeric_ShowAndGetNumber;
// typedef std::tuple<std::string, std::string> msg_parent_kodi_gui_dialogs_Numeric_ShowAndGetSeconds;
// //@}
// //----------------------------------------------------------------------------
// // kodi::gui::dialogs::OK
// //@{
// typedef std::tuple<std::string, std::string> stream_msg_kodi_gui_dialogs_OK_ShowAndGetInput;
// typedef std::tuple<std::string, std::string, std::string, std::string>
//     stream_msg_kodi_gui_dialogs_OK_ShowAndGetInput2;
// //@}
// //----------------------------------------------------------------------------
// // kodi::gui::dialogs::CProgress
// //@{
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_CProgress_create;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_CProgress_destroy;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_CProgress_Open;
// typedef std::tuple<uint64_t, std::string> msg_parent_kodi_gui_dialogs_CProgress_SetHeading;
// typedef std::tuple<uint64_t, unsigned int, std::string> msg_parent_kodi_gui_dialogs_CProgress_SetLine;
// typedef std::tuple<uint64_t, bool> msg_parent_kodi_gui_dialogs_CProgress_SetCanCancel;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_CProgress_IsCanceled;
// typedef std::tuple<uint64_t, int> msg_parent_kodi_gui_dialogs_CProgress_SetPercentage;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_CProgress_GetPercentage;
// typedef std::tuple<uint64_t, bool> msg_parent_kodi_gui_dialogs_CProgress_ShowProgressBar;
// typedef std::tuple<uint64_t, int> msg_parent_kodi_gui_dialogs_CProgress_SetProgressMax;
// typedef std::tuple<uint64_t, int> msg_parent_kodi_gui_dialogs_CProgress_SetProgressAdvance;
// typedef std::tuple<uint64_t> msg_parent_kodi_gui_dialogs_CProgress_Abort;
// //@}
// //----------------------------------------------------------------------------
// // kodi::gui::dialogs::Select
// //@{
// typedef std::tuple<std::string, std::vector<std::string>, int, unsigned int> msg_parent_kodi_gui_dialogs_Select_Show;
// typedef std::tuple<std::string, std::vector<std::tuple<std::string, std::string, bool>>, int, unsigned int> msg_parent_kodi_gui_dialogs_Select_Show2;
// typedef std::tuple<std::string, std::vector<std::tuple<std::string, std::string, bool>>, unsigned int> msg_parent_kodi_gui_dialogs_Select_ShowMultiSelect;
// //@}
// //----------------------------------------------------------------------------
// // kodi::gui::dialogs::TextViewer
// //@{
// typedef std::tuple<std::string, std::string> stream_msg_kodi_gui_dialogs_TextViewer_Show;
// //@}
// //----------------------------------------------------------------------------
// // kodi::gui::dialogs::YesNo
// //@{
// typedef std::tuple<std::string, std::string, bool, std::string, std::string>
//     stream_msg_kodi_gui_dialogs_YesNo_ShowAndGetInput;
// typedef std::tuple<std::string, std::string, std::string, std::string, std::string, std::string>
//     stream_msg_kodi_gui_dialogs_YesNo_ShowAndGetInput2;
// typedef std::
//     tuple<std::string, std::string, std::string, std::string, bool, std::string, std::string>
//         stream_msg_kodi_gui_dialogs_YesNo_ShowAndGetInput3;
// //@}
// //@}
//
//
// typedef std::tuple<std::string> stream_msg_kodi_vfs_StatFile;
// typedef std::tuple<std::string, std::string> stream_msg_kodi_vfs_CopyFile;
// typedef std::tuple<std::string, bool> stream_msg_kodi_vfs_FileExists;
// typedef std::tuple<std::string> stream_msg_kodi_vfs_TranslateSpecialProtocol;
// typedef std::tuple<std::string> stream_msg_kodi_vfs_CreateDirectory;
// typedef std::tuple<std::string, std::string> stream_msg_kodi_vfs_GetDirectory;
// typedef std::tuple<uint64_t> stream_msg_kodi_vfs_CFile_construct;
// typedef std::tuple<uint64_t> stream_msg_kodi_vfs_CFile_destruct;
// typedef std::tuple<uint64_t, std::string, unsigned int> stream_msg_kodi_vfs_CFile_OpenFile;
// typedef std::tuple<uint64_t, std::string, bool> stream_msg_kodi_vfs_CFile_OpenFileForWrite;
// typedef std::tuple<uint64_t> stream_msg_kodi_vfs_CFile_Close;
// typedef std::tuple<uint64_t, size_t> stream_msg_kodi_vfs_CFile_Read;
// typedef std::tuple<uint64_t, std::string> stream_msg_kodi_vfs_CFile_ReadLine;
// typedef std::tuple<uint64_t, std::vector<int8_t>> stream_msg_kodi_vfs_CFile_Write;
//
// typedef std::tuple<uint64_t, uint64_t> stream_msg_instance_web_start_done;
// typedef std::tuple<uint64_t, bool> stream_msg_instance_web_inhibit_shutdown;
// typedef std::tuple<uint64_t, bool> stream_msg_instance_web_inhibit_screensaver;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_web_user_path;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_web_addon_lib_path;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_web_addon_share_path;
// typedef std::tuple<uint64_t> stream_msg_instance_web_is_muted;
// typedef std::tuple<uint64_t, bool> stream_msg_instance_control_SetControlReady;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_control_SetOpenedAddress;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_control_SetOpenedTitle;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_control_SetIconURL;
// typedef std::tuple<uint64_t, bool> stream_msg_instance_control_SetFullscreen;
// typedef std::tuple<uint64_t, bool, bool, bool> stream_msg_instance_control_SetLoadingState;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_control_SetTooltip;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_control_SetStatusMessage;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_control_RequestOpenSiteInNewTab;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetDataIdentifier;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetName;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetDevice;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetXPos;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetYPos;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetWidth;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetHeight;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetPixelRatio;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetFPS;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetSkinXPos;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetSkinYPos;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetSkinWidth;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetSkinHeight;
// typedef std::tuple<uint64_t> stream_msg_instance_control_UseTransparentBackground;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetBackgroundColorARGB;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetGUIItemLeft;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetGUIItemRight;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetGUIItemTop;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetGUIItemBottom;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GetGUIItemBack;
