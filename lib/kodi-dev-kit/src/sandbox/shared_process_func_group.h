/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

namespace kodi
{
namespace sandbox
{

typedef int DummyValue;

typedef enum funcGroup : int
{
  funcGroup_Main = 0,
  funcGroup_AddonBase_h,
  funcGroup_AudioEngine_h,
  funcGroup_Filesystem_h,
  funcGroup_General_h,
  funcGroup_Network_h,
  funcGroup_addoninstance_AudioDecoder_h,
  funcGroup_addoninstance_AudioEncoder_h,
  funcGroup_addoninstance_Web_h,
  funcGroup_gui_General_h,
  funcGroup_gui_ListItem_h,
  funcGroup_gui_Window_h,
  funcGroup_gui_controls_Button_h,
  funcGroup_gui_controls_Edit_h,
  funcGroup_gui_controls_FadeLabel_h,
  funcGroup_gui_controls_Image_h,
  funcGroup_gui_controls_Label_h,
  funcGroup_gui_controls_Progress_h,
  funcGroup_gui_controls_RadioButton_h,
  funcGroup_gui_controls_Rendering_h,
  funcGroup_gui_controls_SettingsSlider_h,
  funcGroup_gui_controls_Slider_h,
  funcGroup_gui_controls_Spin_h,
  funcGroup_gui_controls_TextBox_h,
  funcGroup_gui_dialogs_ContextMenu_h,
  funcGroup_gui_dialogs_ExtendedProgress_h,
  funcGroup_gui_dialogs_FileBrowser_h,
  funcGroup_gui_dialogs_Keyboard_h,
  funcGroup_gui_dialogs_Numeric_h,
  funcGroup_gui_dialogs_OK_h,
  funcGroup_gui_dialogs_Progress_h,
  funcGroup_gui_dialogs_Select_h,
  funcGroup_gui_dialogs_TextViewer_h,
  funcGroup_gui_dialogs_YesNo_h,
  funcGroup_gui_input_Action_h,
  funcGroup_Max
} funcGroup;

} /* namespace sandbox */
} /* namespace kodi */
