/*
 *      Copyright (C) 2015 Team KODI
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

#include <stdio.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#undef Success     // Definition conflicts with cef_message_router.h
#undef RootWindow  // Definition conflicts with root_window.h

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_parser.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include "p8-platform/util/StringUtils.h"

#include "addon.h"
#include "WebGUIDialogContextMenu.h"
#include "WebBrowserClientBase.h"
#include "WebBrowserManager.h"
#include "URICheckHandler.h"
#include "Utils.h"
#include "Dialogs/WebGUIDialogContextMenu.h"
// Reserved 0 - 255
//  XBIRRemote.h
//  XINPUT_IR_REMOTE-*

// Analogue - don't change order
#define KEY_BUTTON_A                        256
#define KEY_BUTTON_B                        257
#define KEY_BUTTON_X                        258
#define KEY_BUTTON_Y                        259
#define KEY_BUTTON_BLACK                    260
#define KEY_BUTTON_WHITE                    261
#define KEY_BUTTON_LEFT_TRIGGER             262
#define KEY_BUTTON_RIGHT_TRIGGER            263

#define KEY_BUTTON_LEFT_THUMB_STICK         264
#define KEY_BUTTON_RIGHT_THUMB_STICK        265

#define KEY_BUTTON_RIGHT_THUMB_STICK_UP     266 // right thumb stick directions
#define KEY_BUTTON_RIGHT_THUMB_STICK_DOWN   267 // for defining different actions per direction
#define KEY_BUTTON_RIGHT_THUMB_STICK_LEFT   268
#define KEY_BUTTON_RIGHT_THUMB_STICK_RIGHT  269

// Digital - don't change order
#define KEY_BUTTON_DPAD_UP                  270
#define KEY_BUTTON_DPAD_DOWN                271
#define KEY_BUTTON_DPAD_LEFT                272
#define KEY_BUTTON_DPAD_RIGHT               273

#define KEY_BUTTON_START                    274
#define KEY_BUTTON_BACK                     275

#define KEY_BUTTON_LEFT_THUMB_BUTTON        276
#define KEY_BUTTON_RIGHT_THUMB_BUTTON       277

#define KEY_BUTTON_LEFT_ANALOG_TRIGGER      278
#define KEY_BUTTON_RIGHT_ANALOG_TRIGGER     279

#define KEY_BUTTON_LEFT_THUMB_STICK_UP      280 // left thumb stick directions
#define KEY_BUTTON_LEFT_THUMB_STICK_DOWN    281 // for defining different actions per direction
#define KEY_BUTTON_LEFT_THUMB_STICK_LEFT    282
#define KEY_BUTTON_LEFT_THUMB_STICK_RIGHT   283

// 0xF000 -> 0xF200 is reserved for the keyboard; a keyboard press is either
#define KEY_VKEY            0xF000 // a virtual key/functional key e.g. cursor left
#define KEY_ASCII           0xF100 // a printable character in the range of TRUE ASCII (from 0 to 127) // FIXME make it clean and pure unicode! remove the need for KEY_ASCII
#define KEY_UNICODE         0xF200 // another printable character whose range is not included in this KEY code

// 0xE000 -> 0xEFFF is reserved for mouse actions
#define KEY_VMOUSE          0xEFFF

#define KEY_MOUSE_START            0xE000
#define KEY_MOUSE_CLICK            0xE000
#define KEY_MOUSE_RIGHTCLICK       0xE001
#define KEY_MOUSE_MIDDLECLICK      0xE002
#define KEY_MOUSE_DOUBLE_CLICK     0xE010
#define KEY_MOUSE_LONG_CLICK       0xE020
#define KEY_MOUSE_WHEEL_UP         0xE101
#define KEY_MOUSE_WHEEL_DOWN       0xE102
#define KEY_MOUSE_MOVE             0xE103
#define KEY_MOUSE_DRAG             0xE104
#define KEY_MOUSE_DRAG_START       0xE105
#define KEY_MOUSE_DRAG_END         0xE106
#define KEY_MOUSE_RDRAG            0xE107
#define KEY_MOUSE_RDRAG_START      0xE108
#define KEY_MOUSE_RDRAG_END        0xE109
#define KEY_MOUSE_NOOP             0xEFFF
#define KEY_MOUSE_END              0xEFFF

// 0xD000 -> 0xD0FF is reserved for WM_APPCOMMAND messages
#define KEY_APPCOMMAND      0xD000

// 0xF000 -> 0xF0FF is reserved for mouse actions
#define KEY_TOUCH           0xF000

#define KEY_INVALID         0xFFFF

#define ADDON_ACTION_NONE                    0
#define ADDON_ACTION_MOVE_LEFT               1
#define ADDON_ACTION_MOVE_RIGHT              2
#define ADDON_ACTION_MOVE_UP                 3
#define ADDON_ACTION_MOVE_DOWN               4
#define ADDON_ACTION_PAGE_UP                 5
#define ADDON_ACTION_PAGE_DOWN               6
#define ADDON_ACTION_SELECT_ITEM             7
#define ADDON_ACTION_HIGHLIGHT_ITEM          8
#define ADDON_ACTION_PARENT_DIR              9
#define ADDON_ACTION_PREVIOUS_MENU          10
#define ADDON_ACTION_SHOW_INFO              11

#define ADDON_ACTION_PAUSE                  12
#define ADDON_ACTION_STOP                   13
#define ADDON_ACTION_NEXT_ITEM              14
#define ADDON_ACTION_PREV_ITEM              15
#define ADDON_ACTION_FORWARD                16 // Can be used to specify specific action in a window, Playback control is handled in ACTION_PLAYER_*
#define ADDON_ACTION_REWIND                 17 // Can be used to specify specific action in a window, Playback control is handled in ACTION_PLAYER_*

#define ADDON_ACTION_SHOW_GUI               18 // toggle between GUI and movie or GUI and visualisation.
#define ADDON_ACTION_ASPECT_RATIO           19 // toggle quick-access zoom modes. Can b used in videoFullScreen.zml window id=2005
#define ADDON_ACTION_STEP_FORWARD           20 // seek +1% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_STEP_BACK              21 // seek -1% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_BIG_STEP_FORWARD       22 // seek +10% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_BIG_STEP_BACK          23 // seek -10% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SHOW_OSD               24 // show/hide OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SHOW_SUBTITLES         25 // turn subtitles on/off. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_NEXT_SUBTITLE          26 // switch to next subtitle of movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SHOW_CODEC             27 // show information about file. Can b used in videoFullScreen.xml window id=2005 and in slideshow.xml window id=2007
#define ADDON_ACTION_NEXT_PICTURE           28 // show next picture of slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_PREV_PICTURE           29 // show previous picture of slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_OUT               30 // zoom in picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_IN                31 // zoom out picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_TOGGLE_SOURCE_DEST     32 // used to toggle between source view and destination view. Can be used in myfiles.xml window id=3
#define ADDON_ACTION_SHOW_PLAYLIST          33 // used to toggle between current view and playlist view. Can b used in all mymusic xml files
#define ADDON_ACTION_QUEUE_ITEM             34 // used to queue a item to the playlist. Can b used in all mymusic xml files
#define ADDON_ACTION_REMOVE_ITEM            35 // not used anymore
#define ADDON_ACTION_SHOW_FULLSCREEN        36 // not used anymore
#define ADDON_ACTION_ZOOM_LEVEL_NORMAL      37 // zoom 1x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_1           38 // zoom 2x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_2           39 // zoom 3x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_3           40 // zoom 4x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_4           41 // zoom 5x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_5           42 // zoom 6x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_6           43 // zoom 7x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_7           44 // zoom 8x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_8           45 // zoom 9x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_9           46 // zoom 10x picture during slideshow. Can b used in slideshow.xml window id=2007

#define ADDON_ACTION_CALIBRATE_SWAP_ARROWS  47 // select next arrow. Can b used in: settingsScreenCalibration.xml windowid=11
#define ADDON_ACTION_CALIBRATE_RESET        48 // reset calibration to defaults. Can b used in: settingsScreenCalibration.xml windowid=11/settingsUICalibration.xml windowid=10
#define ADDON_ACTION_ANALOG_MOVE            49 // analog thumbstick move. Can b used in: slideshow.xml window id=2007/settingsScreenCalibration.xml windowid=11/settingsUICalibration.xml windowid=10
// see also ACTION_ANALOG_MOVE_X, ACTION_ANALOG_MOVE_Y
#define ADDON_ACTION_ROTATE_PICTURE_CW      50 // rotate current picture clockwise during slideshow. Can be used in slideshow.xml window id=2007
#define ADDON_ACTION_ROTATE_PICTURE_CCW     51 // rotate current picture counterclockwise during slideshow. Can be used in slideshow.xml window id=2007

#define ADDON_ACTION_SUBTITLE_DELAY_MIN     52 // Decrease subtitle/movie Delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SUBTITLE_DELAY_PLUS    53 // Increase subtitle/movie Delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_AUDIO_DELAY_MIN        54 // Increase avsync delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_AUDIO_DELAY_PLUS       55 // Decrease avsync delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_AUDIO_NEXT_LANGUAGE    56 // Select next language in movie.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_CHANGE_RESOLUTION      57 // switch 2 next resolution. Can b used during screen calibration settingsScreenCalibration.xml windowid=11

#define REMOTE_0                    58  // remote keys 0-9. are used by multiple windows
#define REMOTE_1                    59  // for example in videoFullScreen.xml window id=2005 you can
#define REMOTE_2                    60  // enter time (mmss) to jump to particular point in the movie
#define REMOTE_3                    61
#define REMOTE_4                    62  // with spincontrols you can enter 3digit number to quickly set
#define REMOTE_5                    63  // spincontrol to desired value
#define REMOTE_6                    64
#define REMOTE_7                    65
#define REMOTE_8                    66
#define REMOTE_9                    67

#define ADDON_ACTION_PLAY                 68  // Unused at the moment
#define ADDON_ACTION_OSD_SHOW_LEFT        69  // Move left in OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_OSD_SHOW_RIGHT       70  // Move right in OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_OSD_SHOW_UP          71  // Move up in OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_OSD_SHOW_DOWN        72  // Move down in OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_OSD_SHOW_SELECT      73  // toggle/select option in OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_OSD_SHOW_VALUE_PLUS  74  // increase value of current option in OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_OSD_SHOW_VALUE_MIN   75  // decrease value of current option in OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SMALL_STEP_BACK      76  // jumps a few seconds back during playback of movie. Can b used in videoFullScreen.xml window id=2005

#define ADDON_ACTION_PLAYER_FORWARD        77  // FF in current file played. global action, can be used anywhere
#define ADDON_ACTION_PLAYER_REWIND         78  // RW in current file played. global action, can be used anywhere
#define ADDON_ACTION_PLAYER_PLAY           79  // Play current song. Unpauses song and sets playspeed to 1x. global action, can be used anywhere

#define ADDON_ACTION_DELETE_ITEM          80  // delete current selected item. Can be used in myfiles.xml window id=3 and in myvideoTitle.xml window id=25
#define ADDON_ACTION_COPY_ITEM            81  // copy current selected item. Can be used in myfiles.xml window id=3
#define ADDON_ACTION_MOVE_ITEM            82  // move current selected item. Can be used in myfiles.xml window id=3
#define ADDON_ACTION_SHOW_MPLAYER_OSD     83  // toggles mplayers OSD. Can be used in videofullscreen.xml window id=2005
#define ADDON_ACTION_OSD_HIDESUBMENU      84  // removes an OSD sub menu. Can be used in videoOSD.xml window id=2901
#define ADDON_ACTION_TAKE_SCREENSHOT      85  // take a screenshot
#define ADDON_ACTION_RENAME_ITEM          87  // rename item

#define ADDON_ACTION_VOLUME_UP            88
#define ADDON_ACTION_VOLUME_DOWN          89
#define ADDON_ACTION_VOLAMP               90
#define ADDON_ACTION_MUTE                 91
#define ADDON_ACTION_NAV_BACK             92
#define ADDON_ACTION_VOLAMP_UP            93
#define ADDON_ACTION_VOLAMP_DOWN          94

#define ADDON_ACTION_CREATE_EPISODE_BOOKMARK 95 //Creates an episode bookmark on the currently playing video file containing more than one episode
#define ADDON_ACTION_CREATE_BOOKMARK         96 //Creates a bookmark of the currently playing video file

#define ADDON_ACTION_CHAPTER_OR_BIG_STEP_FORWARD       97 // Goto the next chapter, if not available perform a big step forward
#define ADDON_ACTION_CHAPTER_OR_BIG_STEP_BACK          98 // Goto the previous chapter, if not available perform a big step back

#define ADDON_ACTION_CYCLE_SUBTITLE         99 // switch to next subtitle of movie, but will not enable/disable the subtitles. Can be used in videoFullScreen.xml window id=2005

#define ADDON_ACTION_MOUSE_START            100
#define ADDON_ACTION_MOUSE_LEFT_CLICK       100
#define ADDON_ACTION_MOUSE_RIGHT_CLICK      101
#define ADDON_ACTION_MOUSE_MIDDLE_CLICK     102
#define ADDON_ACTION_MOUSE_DOUBLE_CLICK     103
#define ADDON_ACTION_MOUSE_WHEEL_UP         104
#define ADDON_ACTION_MOUSE_WHEEL_DOWN       105
#define ADDON_ACTION_MOUSE_DRAG             106
#define ADDON_ACTION_MOUSE_MOVE             107
#define ADDON_ACTION_MOUSE_LONG_CLICK       108
#define ADDON_ACTION_MOUSE_END              109

#define ADDON_ACTION_BACKSPACE          110
#define ADDON_ACTION_SCROLL_UP          111
#define ADDON_ACTION_SCROLL_DOWN        112
#define ADDON_ACTION_ANALOG_FORWARD     113
#define ADDON_ACTION_ANALOG_REWIND      114

#define ADDON_ACTION_MOVE_ITEM_UP       115  // move item up in playlist
#define ADDON_ACTION_MOVE_ITEM_DOWN     116  // move item down in playlist
#define ADDON_ACTION_CONTEXT_MENU       117  // pops up the context menu


// stuff for virtual keyboard shortcuts
#define ADDON_ACTION_SHIFT              118
#define ADDON_ACTION_SYMBOLS            119
#define ADDON_ACTION_CURSOR_LEFT        120
#define ADDON_ACTION_CURSOR_RIGHT       121

#define ADDON_ACTION_BUILT_IN_FUNCTION  122

#define ADDON_ACTION_SHOW_OSD_TIME      123 // displays current time, can be used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_ANALOG_SEEK_FORWARD  124 // seeks forward, and displays the seek bar.
#define ADDON_ACTION_ANALOG_SEEK_BACK     125 // seeks backward, and displays the seek bar.

#define ADDON_ACTION_VIS_PRESET_SHOW        126
#define ADDON_ACTION_VIS_PRESET_NEXT        128
#define ADDON_ACTION_VIS_PRESET_PREV        129
#define ADDON_ACTION_VIS_PRESET_LOCK        130
#define ADDON_ACTION_VIS_PRESET_RANDOM      131
#define ADDON_ACTION_VIS_RATE_PRESET_PLUS   132
#define ADDON_ACTION_VIS_RATE_PRESET_MINUS  133

#define ADDON_ACTION_SHOW_VIDEOMENU         134
#define ADDON_ACTION_ENTER                  135

#define ADDON_ACTION_INCREASE_RATING        136
#define ADDON_ACTION_DECREASE_RATING        137

#define ADDON_ACTION_NEXT_SCENE             138 // switch to next scene/cutpoint in movie
#define ADDON_ACTION_PREV_SCENE             139 // switch to previous scene/cutpoint in movie

#define ADDON_ACTION_NEXT_LETTER            140 // jump through a list or container by letter
#define ADDON_ACTION_PREV_LETTER            141

#define ADDON_ACTION_JUMP_SMS2              142 // jump direct to a particular letter using SMS-style input
#define ADDON_ACTION_JUMP_SMS3              143
#define ADDON_ACTION_JUMP_SMS4              144
#define ADDON_ACTION_JUMP_SMS5              145
#define ADDON_ACTION_JUMP_SMS6              146
#define ADDON_ACTION_JUMP_SMS7              147
#define ADDON_ACTION_JUMP_SMS8              148
#define ADDON_ACTION_JUMP_SMS9              149

#define ADDON_ACTION_FILTER_CLEAR           150
#define ADDON_ACTION_FILTER_SMS2            151
#define ADDON_ACTION_FILTER_SMS3            152
#define ADDON_ACTION_FILTER_SMS4            153
#define ADDON_ACTION_FILTER_SMS5            154
#define ADDON_ACTION_FILTER_SMS6            155
#define ADDON_ACTION_FILTER_SMS7            156
#define ADDON_ACTION_FILTER_SMS8            157
#define ADDON_ACTION_FILTER_SMS9            158

#define ADDON_ACTION_FIRST_PAGE             159
#define ADDON_ACTION_LAST_PAGE              160

#define ADDON_ACTION_AUDIO_DELAY            161
#define ADDON_ACTION_SUBTITLE_DELAY         162
#define ADDON_ACTION_MENU                   163

#define ADDON_ACTION_RECORD                 170

#define ADDON_ACTION_PASTE                  180
#define ADDON_ACTION_NEXT_CONTROL           181
#define ADDON_ACTION_PREV_CONTROL           182
#define ADDON_ACTION_CHANNEL_SWITCH         183
#define ADDON_ACTION_CHANNEL_UP             184
#define ADDON_ACTION_CHANNEL_DOWN           185
#define ADDON_ACTION_NEXT_CHANNELGROUP      186
#define ADDON_ACTION_PREVIOUS_CHANNELGROUP  187
#define ADDON_ACTION_PVR_PLAY               188
#define ADDON_ACTION_PVR_PLAY_TV            189
#define ADDON_ACTION_PVR_PLAY_RADIO         190

#define ADDON_ACTION_TOGGLE_FULLSCREEN      199 // switch 2 desktop resolution
#define ADDON_ACTION_TOGGLE_WATCHED         200 // Toggle watched status (videos)
#define ADDON_ACTION_SCAN_ITEM              201 // scan item
#define ADDON_ACTION_TOGGLE_DIGITAL_ANALOG  202 // switch digital <-> analog
#define ADDON_ACTION_RELOAD_KEYMAPS         203 // reloads CButtonTranslator's keymaps
#define ADDON_ACTION_GUIPROFILE_BEGIN       204 // start the GUIControlProfiler running

#define ADDON_ACTION_TELETEXT_RED           215 // Teletext Color buttons to control TopText
#define ADDON_ACTION_TELETEXT_GREEN         216 //    "       "      "    "     "       "
#define ADDON_ACTION_TELETEXT_YELLOW        217 //    "       "      "    "     "       "
#define ADDON_ACTION_TELETEXT_BLUE          218 //    "       "      "    "     "       "

#define ADDON_ACTION_INCREASE_PAR           219
#define ADDON_ACTION_DECREASE_PAR           220

#define ADDON_ACTION_VSHIFT_UP              227 // shift up video image in DVDPlayer
#define ADDON_ACTION_VSHIFT_DOWN            228 // shift down video image in DVDPlayer

#define ADDON_ACTION_PLAYER_PLAYPAUSE       229 // Play/pause. If playing it pauses, if paused it plays.

#define ADDON_ACTION_SUBTITLE_VSHIFT_UP     230 // shift up subtitles in DVDPlayer
#define ADDON_ACTION_SUBTITLE_VSHIFT_DOWN   231 // shift down subtitles in DVDPlayer
#define ADDON_ACTION_SUBTITLE_ALIGN         232 // toggle vertical alignment of subtitles

#define ADDON_ACTION_FILTER                 233

#define ADDON_ACTION_SWITCH_PLAYER          234

#define ADDON_ACTION_STEREOMODE_NEXT        235
#define ADDON_ACTION_STEREOMODE_PREVIOUS    236
#define ADDON_ACTION_STEREOMODE_TOGGLE      237 // turns 3d mode on/off
#define ADDON_ACTION_STEREOMODE_SELECT      238
#define ADDON_ACTION_STEREOMODE_TOMONO      239
#define ADDON_ACTION_STEREOMODE_SET         240

#define ADDON_ACTION_SETTINGS_RESET         241
#define ADDON_ACTION_SETTINGS_LEVEL_CHANGE  242

#define ADDON_ACTION_TRIGGER_OSD            243 // show autoclosing OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_INPUT_TEXT             244
#define ADDON_ACTION_VOLUME_SET             245

// touch actions
#define ADDON_ACTION_TOUCH_TAP              401
#define ADDON_ACTION_TOUCH_TAP_TEN          410
#define ADDON_ACTION_TOUCH_LONGPRESS        411
#define ADDON_ACTION_TOUCH_LONGPRESS_TEN    420

#define ADDON_ACTION_GESTURE_NOTIFY         500
#define ADDON_ACTION_GESTURE_BEGIN          501
#define ADDON_ACTION_GESTURE_ZOOM           502 //sendaction with point and currentPinchScale (fingers together < 1.0 -> fingers apart > 1.0)
#define ADDON_ACTION_GESTURE_ROTATE         503
#define ADDON_ACTION_GESTURE_PAN            504

#define ADDON_ACTION_GESTURE_SWIPE_LEFT       511
#define ADDON_ACTION_GESTURE_SWIPE_LEFT_TEN   520
#define ADDON_ACTION_GESTURE_SWIPE_RIGHT      521
#define ADDON_ACTION_GESTURE_SWIPE_RIGHT_TEN  530
#define ADDON_ACTION_GESTURE_SWIPE_UP         531
#define ADDON_ACTION_GESTURE_SWIPE_UP_TEN     540
#define ADDON_ACTION_GESTURE_SWIPE_DOWN       541
#define ADDON_ACTION_GESTURE_SWIPE_DOWN_TEN   550
// 5xx is reserved for additional gesture actions
#define ADDON_ACTION_GESTURE_END            599

// other, non-gesture actions
#define ADDON_ACTION_ANALOG_MOVE_X            601 // analog thumbstick move, horizontal axis; see ACTION_ANALOG_MOVE
#define ADDON_ACTION_ANALOG_MOVE_Y            602 // analog thumbstick move, vertical axis; see ACTION_ANALOG_MOVE


// The NOOP action can be specified to disable an input event. This is
// useful in user keyboard.xml etc to disable actions specified in the
// system mappings. ERROR action is used to play an error sound
#define ADDON_ACTION_ERROR                  998
#define ADDON_ACTION_NOOP                   999


using namespace std;
using namespace ADDON;
using namespace P8PLATFORM;

// From ui/events/keycodes/keyboard_codes_posix.h.
enum KeyboardCode
{
        VKEY_BACK = 0x08,
        VKEY_TAB = 0x09,
        VKEY_BACKTAB = 0x0A,
        VKEY_CLEAR = 0x0C,
  VKEY_RETURN = 0x0D,
        VKEY_SHIFT = 0x10,
        VKEY_CONTROL = 0x11,
        VKEY_MENU = 0x12,
        VKEY_PAUSE = 0x13,
        VKEY_CAPITAL = 0x14,
        VKEY_KANA = 0x15,
        VKEY_HANGUL = 0x15,
        VKEY_JUNJA = 0x17,
        VKEY_FINAL = 0x18,
        VKEY_HANJA = 0x19,
        VKEY_KANJI = 0x19,
        VKEY_ESCAPE = 0x1B,
        VKEY_CONVERT = 0x1C,
        VKEY_NONCONVERT = 0x1D,
        VKEY_ACCEPT = 0x1E,
        VKEY_MODECHANGE = 0x1F,
        VKEY_SPACE = 0x20,
  VKEY_PRIOR = 0x21,
  VKEY_NEXT = 0x22,
  VKEY_END = 0x23,
  VKEY_HOME = 0x24,
  VKEY_LEFT = 0x25,
  VKEY_UP = 0x26,
  VKEY_RIGHT = 0x27,
  VKEY_DOWN = 0x28,
        VKEY_SELECT = 0x29,
        VKEY_PRINT = 0x2A,
        VKEY_EXECUTE = 0x2B,
        VKEY_SNAPSHOT = 0x2C,
        VKEY_INSERT = 0x2D,
        VKEY_DELETE = 0x2E,
        VKEY_HELP = 0x2F,
        VKEY_0 = 0x30,
        VKEY_1 = 0x31,
        VKEY_2 = 0x32,
        VKEY_3 = 0x33,
        VKEY_4 = 0x34,
        VKEY_5 = 0x35,
        VKEY_6 = 0x36,
        VKEY_7 = 0x37,
        VKEY_8 = 0x38,
        VKEY_9 = 0x39,
        VKEY_A = 0x41,
        VKEY_B = 0x42,
        VKEY_C = 0x43,
        VKEY_D = 0x44,
        VKEY_E = 0x45,
        VKEY_F = 0x46,
        VKEY_G = 0x47,
        VKEY_H = 0x48,
        VKEY_I = 0x49,
        VKEY_J = 0x4A,
        VKEY_K = 0x4B,
        VKEY_L = 0x4C,
        VKEY_M = 0x4D,
        VKEY_N = 0x4E,
        VKEY_O = 0x4F,
        VKEY_P = 0x50,
        VKEY_Q = 0x51,
        VKEY_R = 0x52,
        VKEY_S = 0x53,
        VKEY_T = 0x54,
        VKEY_U = 0x55,
        VKEY_V = 0x56,
        VKEY_W = 0x57,
        VKEY_X = 0x58,
        VKEY_Y = 0x59,
        VKEY_Z = 0x5A,
        VKEY_LWIN = 0x5B,
        VKEY_COMMAND = VKEY_LWIN,  // Provide the Mac name for convenience.
        VKEY_RWIN = 0x5C,
        VKEY_APPS = 0x5D,
        VKEY_SLEEP = 0x5F,
  VKEY_NUMPAD0 = 0x60,
  VKEY_NUMPAD1 = 0x61,
  VKEY_NUMPAD2 = 0x62,
  VKEY_NUMPAD3 = 0x63,
  VKEY_NUMPAD4 = 0x64,
  VKEY_NUMPAD5 = 0x65,
  VKEY_NUMPAD6 = 0x66,
  VKEY_NUMPAD7 = 0x67,
  VKEY_NUMPAD8 = 0x68,
  VKEY_NUMPAD9 = 0x69,
        VKEY_MULTIPLY = 0x6A,
        VKEY_ADD = 0x6B,
        VKEY_SEPARATOR = 0x6C,
        VKEY_SUBTRACT = 0x6D,
        VKEY_DECIMAL = 0x6E,
        VKEY_DIVIDE = 0x6F,
        VKEY_F1 = 0x70,
        VKEY_F2 = 0x71,
        VKEY_F3 = 0x72,
        VKEY_F4 = 0x73,
        VKEY_F5 = 0x74,
        VKEY_F6 = 0x75,
        VKEY_F7 = 0x76,
        VKEY_F8 = 0x77,
        VKEY_F9 = 0x78,
        VKEY_F10 = 0x79,
        VKEY_F11 = 0x7A,
        VKEY_F12 = 0x7B,
        VKEY_F13 = 0x7C,
        VKEY_F14 = 0x7D,
        VKEY_F15 = 0x7E,
        VKEY_F16 = 0x7F,
        VKEY_F17 = 0x80,
        VKEY_F18 = 0x81,
        VKEY_F19 = 0x82,
        VKEY_F20 = 0x83,
        VKEY_F21 = 0x84,
        VKEY_F22 = 0x85,
        VKEY_F23 = 0x86,
        VKEY_F24 = 0x87,
        VKEY_NUMLOCK = 0x90,
        VKEY_SCROLL = 0x91,
        VKEY_LSHIFT = 0xA0,
        VKEY_RSHIFT = 0xA1,
        VKEY_LCONTROL = 0xA2,
        VKEY_RCONTROL = 0xA3,
        VKEY_LMENU = 0xA4,
        VKEY_RMENU = 0xA5,
        VKEY_BROWSER_BACK = 0xA6,
        VKEY_BROWSER_FORWARD = 0xA7,
        VKEY_BROWSER_REFRESH = 0xA8,
        VKEY_BROWSER_STOP = 0xA9,
        VKEY_BROWSER_SEARCH = 0xAA,
        VKEY_BROWSER_FAVORITES = 0xAB,
        VKEY_BROWSER_HOME = 0xAC,
        VKEY_VOLUME_MUTE = 0xAD,
        VKEY_VOLUME_DOWN = 0xAE,
        VKEY_VOLUME_UP = 0xAF,
        VKEY_MEDIA_NEXT_TRACK = 0xB0,
        VKEY_MEDIA_PREV_TRACK = 0xB1,
        VKEY_MEDIA_STOP = 0xB2,
        VKEY_MEDIA_PLAY_PAUSE = 0xB3,
        VKEY_MEDIA_LAUNCH_MAIL = 0xB4,
        VKEY_MEDIA_LAUNCH_MEDIA_SELECT = 0xB5,
        VKEY_MEDIA_LAUNCH_APP1 = 0xB6,
        VKEY_MEDIA_LAUNCH_APP2 = 0xB7,
        VKEY_OEM_1 = 0xBA,
        VKEY_OEM_PLUS = 0xBB,
        VKEY_OEM_COMMA = 0xBC,
        VKEY_OEM_MINUS = 0xBD,
        VKEY_OEM_PERIOD = 0xBE,
        VKEY_OEM_2 = 0xBF,
        VKEY_OEM_3 = 0xC0,
        VKEY_OEM_4 = 0xDB,
        VKEY_OEM_5 = 0xDC,
        VKEY_OEM_6 = 0xDD,
        VKEY_OEM_7 = 0xDE,
        VKEY_OEM_8 = 0xDF,
        VKEY_OEM_102 = 0xE2,
        VKEY_OEM_103 = 0xE3,  // GTV KEYCODE_MEDIA_REWIND
        VKEY_OEM_104 = 0xE4,  // GTV KEYCODE_MEDIA_FAST_FORWARD
        VKEY_PROCESSKEY = 0xE5,
        VKEY_PACKET = 0xE7,
        VKEY_DBE_SBCSCHAR = 0xF3,
        VKEY_DBE_DBCSCHAR = 0xF4,
        VKEY_ATTN = 0xF6,
        VKEY_CRSEL = 0xF7,
        VKEY_EXSEL = 0xF8,
        VKEY_EREOF = 0xF9,
        VKEY_PLAY = 0xFA,
        VKEY_ZOOM = 0xFB,
        VKEY_NONAME = 0xFC,
        VKEY_PA1 = 0xFD,
        VKEY_OEM_CLEAR = 0xFE,
        VKEY_UNKNOWN = 0,

        // POSIX specific VKEYs. Note that as of Windows SDK 7.1, 0x97-9F, 0xD8-DA,
        // and 0xE8 are unassigned.
        VKEY_WLAN = 0x97,
        VKEY_POWER = 0x98,
        VKEY_BRIGHTNESS_DOWN = 0xD8,
        VKEY_BRIGHTNESS_UP = 0xD9,
        VKEY_KBD_BRIGHTNESS_DOWN = 0xDA,
        VKEY_KBD_BRIGHTNESS_UP = 0xE8,

        // Windows does not have a specific key code for AltGr. We use the unused 0xE1
        // (VK_OEM_AX) code to represent AltGr, matching the behaviour of Firefox on
        // Linux.
        VKEY_ALTGR = 0xE1,
        // Windows does not have a specific key code for Compose. We use the unused
        // 0xE6 (VK_ICO_CLEAR) code to represent Compose.
        VKEY_COMPOSE = 0xE6,
};

// From ui/events/keycodes/keyboard_code_conversion_x.cc.
// Gdk key codes (e.g. GDK_BackSpace) and X keysyms (e.g. XK_BackSpace) share
// the same values.
KeyboardCode ActionIdToKeyboardCode(int actionId)
{
fprintf(stderr, "actionId - %i\n", actionId);
  switch (actionId)
  {
    case ADDON_ACTION_MOVE_LEFT:        return VKEY_LEFT;
    case ADDON_ACTION_MOVE_RIGHT:       return VKEY_RIGHT;
    case ADDON_ACTION_MOVE_UP:          return VKEY_UP;
    case ADDON_ACTION_MOVE_DOWN:        return VKEY_DOWN;
    case ADDON_ACTION_PAGE_UP:          return VKEY_PRIOR;
    case ADDON_ACTION_PAGE_DOWN:        fprintf(stderr, "ADDON_ACTION_PAGE_DOWN\n"); return VKEY_NEXT;
    case ADDON_ACTION_SELECT_ITEM:      fprintf(stderr, "ADDON_ACTION_SELECT_ITEM\n"); return VKEY_RETURN;
    case ADDON_ACTION_HIGHLIGHT_ITEM:
      fprintf(stderr, "ADDON_ACTION_HIGHLIGHT_ITEM\n");
      break;
    case ADDON_ACTION_PARENT_DIR:
      fprintf(stderr, "ADDON_ACTION_PARENT_DIR\n");;
      break;
    case ADDON_ACTION_PREVIOUS_MENU:
      fprintf(stderr, "ADDON_ACTION_PREVIOUS_MENU\n");;
      break;
    case ADDON_ACTION_SHOW_INFO:
      fprintf(stderr, "ADDON_ACTION_SHOW_INFO\n");;
      break;
    case ADDON_ACTION_PAUSE:
      fprintf(stderr, "ADDON_ACTION_PAUSE\n");;
      break;
    case ADDON_ACTION_STOP:
      fprintf(stderr, "ADDON_ACTION_STOP\n");;
      break;
    case ADDON_ACTION_NEXT_ITEM:
      fprintf(stderr, "ADDON_ACTION_NEXT_ITEM\n");;
      break;
    case ADDON_ACTION_PREV_ITEM:
      fprintf(stderr, "ADDON_ACTION_PREV_ITEM\n");;
      break;
    case ADDON_ACTION_FORWARD:
      fprintf(stderr, "ADDON_ACTION_FORWARD\n");;
      break;
    case ADDON_ACTION_REWIND:
      fprintf(stderr, "ADDON_ACTION_REWIND\n");;
      break;
    case ADDON_ACTION_SHOW_GUI:
      fprintf(stderr, "ADDON_ACTION_SHOW_GUI\n");;
      break;
    case ADDON_ACTION_ASPECT_RATIO:
      fprintf(stderr, "ADDON_ACTION_ASPECT_RATIO\n");;
      break;
    case ADDON_ACTION_STEP_FORWARD:
      fprintf(stderr, "ADDON_ACTION_STEP_FORWARD\n");;
      break;
    case ADDON_ACTION_STEP_BACK:
      fprintf(stderr, "ADDON_ACTION_STEP_BACK\n");;
      break;
    case ADDON_ACTION_BIG_STEP_FORWARD:
      fprintf(stderr, "ADDON_ACTION_BIG_STEP_FORWARD\n");;
      break;
    case ADDON_ACTION_BIG_STEP_BACK:
      fprintf(stderr, "ADDON_ACTION_BIG_STEP_BACK\n");;
      break;
    case ADDON_ACTION_SHOW_OSD:
      fprintf(stderr, "ADDON_ACTION_SHOW_OSD\n");;
      break;
    case ADDON_ACTION_SHOW_SUBTITLES:
      fprintf(stderr, "ADDON_ACTION_SHOW_SUBTITLES\n");;
      break;
    case ADDON_ACTION_NEXT_SUBTITLE:
      fprintf(stderr, "ADDON_ACTION_NEXT_SUBTITLE\n");;
      break;
    case ADDON_ACTION_SHOW_CODEC:
      fprintf(stderr, "ADDON_ACTION_SHOW_CODEC\n");;
      break;
    case ADDON_ACTION_NEXT_PICTURE:
      fprintf(stderr, "ADDON_ACTION_NEXT_PICTURE\n");;
      break;
    case ADDON_ACTION_PREV_PICTURE:
      fprintf(stderr, "ADDON_ACTION_PREV_PICTURE\n");;
      break;
    case ADDON_ACTION_ZOOM_OUT:
      fprintf(stderr, "ADDON_ACTION_ZOOM_OUT\n");;
      break;
    case ADDON_ACTION_ZOOM_IN:
      fprintf(stderr, "ADDON_ACTION_ZOOM_IN\n");;
      break;
    case ADDON_ACTION_TOGGLE_SOURCE_DEST:
      fprintf(stderr, "ADDON_ACTION_TOGGLE_SOURCE_DEST\n");;
      break;
    case ADDON_ACTION_SHOW_PLAYLIST:
      fprintf(stderr, "ADDON_ACTION_SHOW_PLAYLIST\n");;
      break;
    case ADDON_ACTION_QUEUE_ITEM:
      fprintf(stderr, "ADDON_ACTION_MOVE_LEFT\n");;
      break;
    case ADDON_ACTION_REMOVE_ITEM:
      fprintf(stderr, "ADDON_ACTION_REMOVE_ITEM\n");;
      break;
    case ADDON_ACTION_SHOW_FULLSCREEN:
      fprintf(stderr, "ADDON_ACTION_SHOW_FULLSCREEN\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_NORMAL:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_NORMAL\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_1:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_1\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_2:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_2\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_3:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_3\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_4:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_4\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_5:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_5\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_6:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_6\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_7:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_7\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_8:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_8\n");;
      break;
    case ADDON_ACTION_ZOOM_LEVEL_9:
      fprintf(stderr, "ADDON_ACTION_ZOOM_LEVEL_9\n");;
      break;
    case ADDON_ACTION_CALIBRATE_SWAP_ARROWS:
      fprintf(stderr, "ADDON_ACTION_CALIBRATE_SWAP_ARROWS\n");;
      break;
    case ADDON_ACTION_CALIBRATE_RESET:
      fprintf(stderr, "ADDON_ACTION_CALIBRATE_RESET\n");
      break;
    case ADDON_ACTION_ANALOG_MOVE:
      fprintf(stderr, "ADDON_ACTION_ANALOG_MOVE\n");
      break;
    case ADDON_ACTION_ROTATE_PICTURE_CW:
      fprintf(stderr, "ADDON_ACTION_ROTATE_PICTURE_CW\n");
      break;
    case ADDON_ACTION_ROTATE_PICTURE_CCW:
      fprintf(stderr, "ADDON_ACTION_ROTATE_PICTURE_CCW\n");
      break;
    case ADDON_ACTION_SUBTITLE_DELAY_MIN:
      fprintf(stderr, "ADDON_ACTION_SUBTITLE_DELAY_MIN\n");
      break;
    case ADDON_ACTION_SUBTITLE_DELAY_PLUS:
      fprintf(stderr, "ADDON_ACTION_SUBTITLE_DELAY_PLUS\n");
      break;
    case ADDON_ACTION_AUDIO_DELAY_MIN:
      fprintf(stderr, "ADDON_ACTION_AUDIO_DELAY_MIN\n");
      break;
    case ADDON_ACTION_AUDIO_DELAY_PLUS:
      fprintf(stderr, "ADDON_ACTION_AUDIO_DELAY_PLUS\n");
      break;
    case ADDON_ACTION_AUDIO_NEXT_LANGUAGE:
      fprintf(stderr, "ADDON_ACTION_AUDIO_NEXT_LANGUAGE\n");
      break;
    case ADDON_ACTION_CHANGE_RESOLUTION:
      fprintf(stderr, "ADDON_ACTION_CHANGE_RESOLUTION\n");
      break;
    case REMOTE_0:                      return VKEY_NUMPAD0;
    case REMOTE_1:                      return VKEY_NUMPAD1;
    case REMOTE_2:                      return VKEY_NUMPAD2;
    case REMOTE_3:                      return VKEY_NUMPAD3;
    case REMOTE_4:                      return VKEY_NUMPAD4;
    case REMOTE_5:                      return VKEY_NUMPAD5;
    case REMOTE_6:                      return VKEY_NUMPAD6;
    case REMOTE_7:                      return VKEY_NUMPAD7;
    case REMOTE_8:                      return VKEY_NUMPAD8;
    case REMOTE_9:                      return VKEY_NUMPAD9;
    case ADDON_ACTION_PLAY:
      fprintf(stderr, "ADDON_ACTION_PLAY\n");
      break;
    case ADDON_ACTION_OSD_SHOW_LEFT:
      fprintf(stderr, "ADDON_ACTION_OSD_SHOW_LEFT\n");
      break;
    case ADDON_ACTION_OSD_SHOW_RIGHT:
      fprintf(stderr, "ADDON_ACTION_OSD_SHOW_RIGHT\n");
      break;
    case ADDON_ACTION_OSD_SHOW_UP:
      fprintf(stderr, "ADDON_ACTION_OSD_SHOW_UP\n");
      break;
    case ADDON_ACTION_OSD_SHOW_DOWN:
      fprintf(stderr, "ADDON_ACTION_OSD_SHOW_DOWN\n");
      break;
    case ADDON_ACTION_OSD_SHOW_SELECT:
      fprintf(stderr, "ADDON_ACTION_OSD_SHOW_SELECT\n");
      break;
    case ADDON_ACTION_OSD_SHOW_VALUE_PLUS:
      fprintf(stderr, "ADDON_ACTION_OSD_SHOW_VALUE_PLUS\n");
      break;
    case ADDON_ACTION_OSD_SHOW_VALUE_MIN:
      fprintf(stderr, "ADDON_ACTION_OSD_SHOW_VALUE_MIN\n");
      break;
    case ADDON_ACTION_SMALL_STEP_BACK:
      fprintf(stderr, "ADDON_ACTION_SMALL_STEP_BACK\n");
      break;
    case ADDON_ACTION_PLAYER_FORWARD:
      fprintf(stderr, "ADDON_ACTION_PLAYER_FORWARD\n");
      break;
    case ADDON_ACTION_PLAYER_REWIND:
      fprintf(stderr, "ADDON_ACTION_PLAYER_REWIND\n");
      break;
    case ADDON_ACTION_PLAYER_PLAY:
      fprintf(stderr, "ADDON_ACTION_PLAYER_PLAY\n");
      break;
    case ADDON_ACTION_DELETE_ITEM:
      fprintf(stderr, "ADDON_ACTION_DELETE_ITEM\n");
      break;
    case ADDON_ACTION_COPY_ITEM:
      fprintf(stderr, "ADDON_ACTION_COPY_ITEM\n");
      break;
    case ADDON_ACTION_MOVE_ITEM:
      fprintf(stderr, "ADDON_ACTION_MOVE_ITEM\n");
      break;
    case ADDON_ACTION_SHOW_MPLAYER_OSD:
      fprintf(stderr, "ADDON_ACTION_SHOW_MPLAYER_OSD\n");
      break;
    case ADDON_ACTION_OSD_HIDESUBMENU:
      fprintf(stderr, "ADDON_ACTION_OSD_HIDESUBMENU\n");
      break;
    case ADDON_ACTION_TAKE_SCREENSHOT:
      fprintf(stderr, "ADDON_ACTION_TAKE_SCREENSHOT\n");
      break;
    case ADDON_ACTION_RENAME_ITEM:
      fprintf(stderr, "ADDON_ACTION_RENAME_ITEM\n");
      break;
    case ADDON_ACTION_VOLUME_UP:
      fprintf(stderr, "ADDON_ACTION_VOLUME_UP\n");
      break;
    case ADDON_ACTION_VOLUME_DOWN:
      fprintf(stderr, "ADDON_ACTION_VOLUME_DOWN\n");
      break;
    case ADDON_ACTION_VOLAMP:
      fprintf(stderr, "ADDON_ACTION_VOLAMP\n");
      break;
    case ADDON_ACTION_MUTE:
      fprintf(stderr, "ADDON_ACTION_MUTE\n");
      break;
    case ADDON_ACTION_NAV_BACK:
      fprintf(stderr, "ADDON_ACTION_NAV_BACK\n");
      break;
    case ADDON_ACTION_VOLAMP_UP:
      fprintf(stderr, "ADDON_ACTION_VOLAMP_UP\n");
      break;
    case ADDON_ACTION_VOLAMP_DOWN:
      fprintf(stderr, "ADDON_ACTION_VOLAMP_DOWN\n");
      break;
    case ADDON_ACTION_CREATE_EPISODE_BOOKMARK:
      fprintf(stderr, "ADDON_ACTION_CREATE_EPISODE_BOOKMARK\n");
      break;
    case ADDON_ACTION_CREATE_BOOKMARK:
      fprintf(stderr, "ADDON_ACTION_CREATE_BOOKMARK\n");
      break;
    case ADDON_ACTION_CHAPTER_OR_BIG_STEP_FORWARD:
      fprintf(stderr, "ADDON_ACTION_CHAPTER_OR_BIG_STEP_FORWARD\n");
      break;
    case ADDON_ACTION_CHAPTER_OR_BIG_STEP_BACK:
      fprintf(stderr, "ADDON_ACTION_CHAPTER_OR_BIG_STEP_BACK\n");
      break;
    case ADDON_ACTION_CYCLE_SUBTITLE:
      fprintf(stderr, "ADDON_ACTION_CYCLE_SUBTITLE\n");
      break;
//    case ADDON_ACTION_MOUSE_START:
//      fprintf(stderr, "ADDON_ACTION_MOUSE_START\n");
//      break;
    case ADDON_ACTION_MOUSE_LEFT_CLICK:
      fprintf(stderr, "ADDON_ACTION_MOUSE_LEFT_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_RIGHT_CLICK:
      fprintf(stderr, "ADDON_ACTION_MOUSE_RIGHT_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_MIDDLE_CLICK:
      fprintf(stderr, "ADDON_ACTION_MOUSE_MIDDLE_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_DOUBLE_CLICK:
      fprintf(stderr, "ADDON_ACTION_MOUSE_DOUBLE_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_WHEEL_UP:
      fprintf(stderr, "ADDON_ACTION_MOUSE_WHEEL_UP\n");
      break;
    case ADDON_ACTION_MOUSE_WHEEL_DOWN:
      fprintf(stderr, "ADDON_ACTION_MOUSE_WHEEL_DOWN\n");
      break;
    case ADDON_ACTION_MOUSE_DRAG:
      fprintf(stderr, "ADDON_ACTION_MOUSE_DRAG\n");
      break;
    case ADDON_ACTION_MOUSE_MOVE:
      fprintf(stderr, "ADDON_ACTION_MOUSE_MOVE\n");
      break;
    case ADDON_ACTION_MOUSE_LONG_CLICK:
      fprintf(stderr, "ADDON_ACTION_MOUSE_LONG_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_END:
      fprintf(stderr, "ADDON_ACTION_MOUSE_END\n");
      break;
    case ADDON_ACTION_BACKSPACE:
      fprintf(stderr, "ADDON_ACTION_BACKSPACE\n");
      break;
    case ADDON_ACTION_SCROLL_UP:
      fprintf(stderr, "ADDON_ACTION_SCROLL_UP\n");
      break;
    case ADDON_ACTION_SCROLL_DOWN:
      fprintf(stderr, "ADDON_ACTION_SCROLL_DOWN\n");
      break;
    case ADDON_ACTION_ANALOG_FORWARD:
      fprintf(stderr, "ADDON_ACTION_ANALOG_FORWARD\n");
      break;
    case ADDON_ACTION_ANALOG_REWIND:
      fprintf(stderr, "ADDON_ACTION_ANALOG_REWIND\n");
      break;
    case ADDON_ACTION_MOVE_ITEM_UP:
      fprintf(stderr, "ADDON_ACTION_MOVE_ITEM_UP\n");
      break;
    case ADDON_ACTION_MOVE_ITEM_DOWN:
      fprintf(stderr, "ADDON_ACTION_MOVE_ITEM_DOWN\n");
      break;
    case ADDON_ACTION_CONTEXT_MENU:
      fprintf(stderr, "ADDON_ACTION_CONTEXT_MENU\n");
      break;
    case ADDON_ACTION_SHIFT:
      fprintf(stderr, "ADDON_ACTION_SHIFT\n");
      break;
    case ADDON_ACTION_SYMBOLS:
      fprintf(stderr, "ADDON_ACTION_SYMBOLS\n");
      break;
    case ADDON_ACTION_CURSOR_LEFT:
      fprintf(stderr, "ADDON_ACTION_CURSOR_LEFT\n");
      break;
    case ADDON_ACTION_CURSOR_RIGHT:
      fprintf(stderr, "ADDON_ACTION_CURSOR_RIGHT\n");
      break;
    case ADDON_ACTION_BUILT_IN_FUNCTION:
      fprintf(stderr, "ADDON_ACTION_BUILT_IN_FUNCTION\n");
      break;
    case ADDON_ACTION_SHOW_OSD_TIME:
      fprintf(stderr, "ADDON_ACTION_SHOW_OSD_TIME\n");
      break;
    case ADDON_ACTION_ANALOG_SEEK_FORWARD:
      fprintf(stderr, "ADDON_ACTION_ANALOG_SEEK_FORWARD\n");
      break;
    case ADDON_ACTION_ANALOG_SEEK_BACK:
      fprintf(stderr, "ADDON_ACTION_ANALOG_SEEK_BACK\n");
      break;
    case ADDON_ACTION_VIS_PRESET_SHOW:
      fprintf(stderr, "ADDON_ACTION_VIS_PRESET_SHOW\n");
      break;
    case ADDON_ACTION_VIS_PRESET_NEXT:
      fprintf(stderr, "ADDON_ACTION_VIS_PRESET_NEXT\n");
      break;
    case ADDON_ACTION_VIS_PRESET_PREV:
      fprintf(stderr, "ADDON_ACTION_VIS_PRESET_PREV\n");
      break;
    case ADDON_ACTION_VIS_PRESET_LOCK:
      fprintf(stderr, "ADDON_ACTION_VIS_PRESET_LOCK\n");
      break;
    case ADDON_ACTION_VIS_PRESET_RANDOM:
      fprintf(stderr, "ADDON_ACTION_VIS_PRESET_RANDOM\n");
      break;
    case ADDON_ACTION_VIS_RATE_PRESET_PLUS:
      fprintf(stderr, "ADDON_ACTION_VIS_RATE_PRESET_PLUS\n");
      break;
    case ADDON_ACTION_VIS_RATE_PRESET_MINUS:
      fprintf(stderr, "ADDON_ACTION_VIS_RATE_PRESET_MINUS\n");
      break;
    case ADDON_ACTION_SHOW_VIDEOMENU:
      fprintf(stderr, "ADDON_ACTION_SHOW_VIDEOMENU\n");
      break;
    case ADDON_ACTION_ENTER:
      fprintf(stderr, "ADDON_ACTION_ENTER\n");
      break;
    case ADDON_ACTION_INCREASE_RATING:
      fprintf(stderr, "ADDON_ACTION_INCREASE_RATING\n");
      break;
    case ADDON_ACTION_DECREASE_RATING:
      fprintf(stderr, "ADDON_ACTION_DECREASE_RATING\n");
      break;
    case ADDON_ACTION_NEXT_SCENE:
      fprintf(stderr, "ADDON_ACTION_NEXT_SCENE\n");
      break;
    case ADDON_ACTION_PREV_SCENE:
      fprintf(stderr, "ADDON_ACTION_PREV_SCENE\n");
      break;
    case ADDON_ACTION_NEXT_LETTER:
      fprintf(stderr, "ADDON_ACTION_NEXT_LETTER\n");
      break;
    case ADDON_ACTION_PREV_LETTER:
      fprintf(stderr, "ADDON_ACTION_PREV_LETTER\n");
      break;
    case ADDON_ACTION_JUMP_SMS2:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS2\n");
      break;
    case ADDON_ACTION_JUMP_SMS3:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS3\n");
      break;
    case ADDON_ACTION_JUMP_SMS4:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS4\n");
      break;
    case ADDON_ACTION_JUMP_SMS5:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS5\n");
      break;
    case ADDON_ACTION_JUMP_SMS6:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS6\n");
      break;
    case ADDON_ACTION_JUMP_SMS7:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS7\n");
      break;
    case ADDON_ACTION_JUMP_SMS8:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS8\n");
      break;
    case ADDON_ACTION_JUMP_SMS9:
      fprintf(stderr, "ADDON_ACTION_JUMP_SMS9\n");
      break;
    case ADDON_ACTION_FILTER_CLEAR:
      fprintf(stderr, "ADDON_ACTION_FILTER_CLEAR\n");
      break;
    case ADDON_ACTION_FILTER_SMS2:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS2\n");
      break;
    case ADDON_ACTION_FILTER_SMS3:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS3\n");
      break;
    case ADDON_ACTION_FILTER_SMS4:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS4\n");
      break;
    case ADDON_ACTION_FILTER_SMS5:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS5\n");
      break;
    case ADDON_ACTION_FILTER_SMS6:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS6\n");
      break;
    case ADDON_ACTION_FILTER_SMS7:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS7\n");
      break;
    case ADDON_ACTION_FILTER_SMS8:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS8\n");
      break;
    case ADDON_ACTION_FILTER_SMS9:
      fprintf(stderr, "ADDON_ACTION_FILTER_SMS9\n");
      break;
    case ADDON_ACTION_FIRST_PAGE:        fprintf(stderr, "ADDON_ACTION_SELECT_ITEM\n"); return VKEY_HOME;
    case ADDON_ACTION_LAST_PAGE:         fprintf(stderr, "ADDON_ACTION_SELECT_ITEM\n"); return VKEY_END;
    case ADDON_ACTION_AUDIO_DELAY:
      fprintf(stderr, "ADDON_ACTION_AUDIO_DELAY\n");
      break;
    case ADDON_ACTION_SUBTITLE_DELAY:
      fprintf(stderr, "ADDON_ACTION_SUBTITLE_DELAY\n");
      break;
    case ADDON_ACTION_MENU:
      fprintf(stderr, "ADDON_ACTION_MENU\n");
      break;
    case ADDON_ACTION_RECORD:
      fprintf(stderr, "ADDON_ACTION_RECORD\n");
      break;
    case ADDON_ACTION_PASTE:
      fprintf(stderr, "ADDON_ACTION_PASTE\n");
      break;
    case ADDON_ACTION_NEXT_CONTROL:
      fprintf(stderr, "ADDON_ACTION_NEXT_CONTROL\n");
      break;
    case ADDON_ACTION_PREV_CONTROL:
      fprintf(stderr, "ADDON_ACTION_PREV_CONTROL\n");
      break;
    case ADDON_ACTION_CHANNEL_SWITCH:
      fprintf(stderr, "ADDON_ACTION_CHANNEL_SWITCH\n");
      break;
    case ADDON_ACTION_CHANNEL_UP:
      fprintf(stderr, "ADDON_ACTION_CHANNEL_UP\n");
      break;
    case ADDON_ACTION_CHANNEL_DOWN:
      fprintf(stderr, "ADDON_ACTION_CHANNEL_DOWN\n");
      break;
    case ADDON_ACTION_NEXT_CHANNELGROUP:
      fprintf(stderr, "ADDON_ACTION_NEXT_CHANNELGROUP\n");
      break;
    case ADDON_ACTION_PREVIOUS_CHANNELGROUP:
      fprintf(stderr, "ADDON_ACTION_PREVIOUS_CHANNELGROUP\n");
      break;
    case ADDON_ACTION_PVR_PLAY:
      fprintf(stderr, "ADDON_ACTION_PVR_PLAY\n");
      break;
    case ADDON_ACTION_PVR_PLAY_TV:
      fprintf(stderr, "ADDON_ACTION_PVR_PLAY_TV\n");
      break;
    case ADDON_ACTION_PVR_PLAY_RADIO:
      fprintf(stderr, "ADDON_ACTION_PVR_PLAY_RADIO\n");
      break;
    case ADDON_ACTION_TOGGLE_FULLSCREEN:
      fprintf(stderr, "ADDON_ACTION_TOGGLE_FULLSCREEN\n");
      break;
    case ADDON_ACTION_TOGGLE_WATCHED:
      fprintf(stderr, "ADDON_ACTION_TOGGLE_WATCHED\n");
      break;
    case ADDON_ACTION_SCAN_ITEM:
      fprintf(stderr, "ADDON_ACTION_SCAN_ITEM\n");
      break;
    case ADDON_ACTION_TOGGLE_DIGITAL_ANALOG:
      fprintf(stderr, "ADDON_ACTION_TOGGLE_DIGITAL_ANALOG\n");
      break;
    case ADDON_ACTION_RELOAD_KEYMAPS:
      fprintf(stderr, "ADDON_ACTION_RELOAD_KEYMAPS\n");
      break;
    case ADDON_ACTION_GUIPROFILE_BEGIN:
      fprintf(stderr, "ADDON_ACTION_GUIPROFILE_BEGIN\n");
      break;
    case ADDON_ACTION_TELETEXT_RED:
      fprintf(stderr, "ADDON_ACTION_TELETEXT_RED\n");
      break;
    case ADDON_ACTION_TELETEXT_GREEN:
      fprintf(stderr, "ADDON_ACTION_TELETEXT_GREEN\n");
      break;
    case ADDON_ACTION_TELETEXT_YELLOW:
      fprintf(stderr, "ADDON_ACTION_TELETEXT_YELLOW\n");
      break;
    case ADDON_ACTION_TELETEXT_BLUE:
      fprintf(stderr, "ADDON_ACTION_TELETEXT_BLUE\n");
      break;
    case ADDON_ACTION_INCREASE_PAR:
      fprintf(stderr, "ADDON_ACTION_INCREASE_PAR\n");
      break;
    case ADDON_ACTION_DECREASE_PAR:
      fprintf(stderr, "ADDON_ACTION_DECREASE_PAR\n");
      break;
    case ADDON_ACTION_VSHIFT_UP:
      fprintf(stderr, "ADDON_ACTION_VSHIFT_UP\n");
      break;
    case ADDON_ACTION_VSHIFT_DOWN:
      fprintf(stderr, "ADDON_ACTION_VSHIFT_DOWN\n");
      break;
    case ADDON_ACTION_PLAYER_PLAYPAUSE:
      fprintf(stderr, "ADDON_ACTION_PLAYER_PLAYPAUSE\n");
      break;
    case ADDON_ACTION_SUBTITLE_VSHIFT_UP:
      fprintf(stderr, "ADDON_ACTION_SUBTITLE_VSHIFT_UP\n");
      break;
    case ADDON_ACTION_SUBTITLE_VSHIFT_DOWN:
      fprintf(stderr, "ADDON_ACTION_SUBTITLE_VSHIFT_DOWN\n");
      break;
    case ADDON_ACTION_SUBTITLE_ALIGN:
      fprintf(stderr, "ADDON_ACTION_SUBTITLE_ALIGN\n");
      break;
    case ADDON_ACTION_FILTER:
      fprintf(stderr, "ADDON_ACTION_FILTER\n");
      break;
    case ADDON_ACTION_SWITCH_PLAYER:
      fprintf(stderr, "ADDON_ACTION_SWITCH_PLAYER\n");
      break;
    case ADDON_ACTION_STEREOMODE_NEXT:
      fprintf(stderr, "ADDON_ACTION_STEREOMODE_NEXT\n");
      break;
    case ADDON_ACTION_STEREOMODE_PREVIOUS:
      fprintf(stderr, "ADDON_ACTION_STEREOMODE_PREVIOUS\n");
      break;
    case ADDON_ACTION_STEREOMODE_TOGGLE:
      fprintf(stderr, "ADDON_ACTION_STEREOMODE_TOGGLE\n");
      break;
    case ADDON_ACTION_STEREOMODE_SELECT:
      fprintf(stderr, "ADDON_ACTION_STEREOMODE_SELECT\n");
      break;
    case ADDON_ACTION_STEREOMODE_TOMONO:
      fprintf(stderr, "ADDON_ACTION_STEREOMODE_TOMONO\n");
      break;
    case ADDON_ACTION_STEREOMODE_SET:
      fprintf(stderr, "ADDON_ACTION_STEREOMODE_SET\n");
      break;
    case ADDON_ACTION_SETTINGS_RESET:
      fprintf(stderr, "ADDON_ACTION_SETTINGS_RESET\n");
      break;
    case ADDON_ACTION_SETTINGS_LEVEL_CHANGE:
      fprintf(stderr, "ADDON_ACTION_SETTINGS_LEVEL_CHANGE\n");
      break;
    case ADDON_ACTION_TRIGGER_OSD:
      fprintf(stderr, "ADDON_ACTION_TRIGGER_OSD\n");
      break;
    case ADDON_ACTION_INPUT_TEXT:
      fprintf(stderr, "ADDON_ACTION_INPUT_TEXT\n");
      break;
    case ADDON_ACTION_VOLUME_SET:
      fprintf(stderr, "ADDON_ACTION_VOLUME_SET\n");
      break;
    case ADDON_ACTION_TOUCH_TAP:
      fprintf(stderr, "ADDON_ACTION_TOUCH_TAP\n");
      break;
    case ADDON_ACTION_TOUCH_TAP_TEN:
      fprintf(stderr, "ADDON_ACTION_TOUCH_TAP_TEN\n");
      break;
    case ADDON_ACTION_TOUCH_LONGPRESS:
      fprintf(stderr, "ADDON_ACTION_TOUCH_LONGPRESS\n");
      break;
    case ADDON_ACTION_TOUCH_LONGPRESS_TEN:
      fprintf(stderr, "ADDON_ACTION_TOUCH_LONGPRESS_TEN\n");
      break;
    case ADDON_ACTION_GESTURE_NOTIFY:
      fprintf(stderr, "ADDON_ACTION_GESTURE_NOTIFY\n");
      break;
    case ADDON_ACTION_GESTURE_BEGIN:
      fprintf(stderr, "ADDON_ACTION_GESTURE_BEGIN\n");
      break;
    case ADDON_ACTION_GESTURE_ZOOM:
      fprintf(stderr, "ADDON_ACTION_GESTURE_ZOOM\n");
      break;
    case ADDON_ACTION_GESTURE_ROTATE:
      fprintf(stderr, "ADDON_ACTION_GESTURE_ROTATE\n");
      break;
    case ADDON_ACTION_GESTURE_PAN:
      fprintf(stderr, "ADDON_ACTION_GESTURE_PAN\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_LEFT:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_LEFT\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_LEFT_TEN:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_LEFT_TEN\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_RIGHT:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_RIGHT\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_RIGHT_TEN:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_RIGHT_TEN\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_UP:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_UP\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_UP_TEN:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_UP_TEN\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_DOWN:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_DOWN\n");
      break;
    case ADDON_ACTION_GESTURE_SWIPE_DOWN_TEN:
      fprintf(stderr, "ADDON_ACTION_GESTURE_SWIPE_DOWN_TEN\n");
      break;
    case ADDON_ACTION_GESTURE_END:
      fprintf(stderr, "ADDON_ACTION_GESTURE_END\n");
      break;
    case ADDON_ACTION_ANALOG_MOVE_X:
      fprintf(stderr, "ADDON_ACTION_ANALOG_MOVE_X\n");
      break;
    case ADDON_ACTION_ANALOG_MOVE_Y:
      fprintf(stderr, "ADDON_ACTION_ANALOG_MOVE_Y\n");
      break;
    case ADDON_ACTION_ERROR:
      fprintf(stderr, "ADDON_ACTION_ERROR\n");
      break;
    case ADDON_ACTION_NOOP:
      fprintf(stderr, "ADDON_ACTION_NOOP\n");
      break;
    case ADDON_ACTION_NONE:
    default:
      break;
  }
  return VKEY_UNKNOWN;
}

//#define ADDON_ACTION_MOUSE_START            100
//#define ADDON_ACTION_MOUSE_LEFT_CLICK       100
//#define ADDON_ACTION_MOUSE_RIGHT_CLICK      101
//#define ADDON_ACTION_MOUSE_MIDDLE_CLICK     102
//#define ADDON_ACTION_MOUSE_DOUBLE_CLICK     103
//#define ADDON_ACTION_MOUSE_WHEEL_UP         104
//#define ADDON_ACTION_MOUSE_WHEEL_DOWN       105
//#define ADDON_ACTION_MOUSE_DRAG             106
//#define ADDON_ACTION_MOUSE_MOVE             107
//#define ADDON_ACTION_MOUSE_LONG_CLICK       108
//#define ADDON_ACTION_MOUSE_END              109

int GetCefStateModifiers(int actionId)
{
  int modifiers = 0;
  switch (actionId)
  {
    case ADDON_ACTION_MOUSE_LEFT_CLICK:
      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
      fprintf(stderr, "ADDON_ACTION_MOUSE_LEFT_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_RIGHT_CLICK:
      modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
      fprintf(stderr, "ADDON_ACTION_MOUSE_RIGHT_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_MIDDLE_CLICK:
      modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
      fprintf(stderr, "EVENTFLAG_MIDDLE_MOUSE_BUTTON\n");
      break;
    case ADDON_ACTION_MOUSE_DOUBLE_CLICK:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ADDON_ACTION_MOUSE_DOUBLE_CLICK\n");
      break;
    case ADDON_ACTION_MOUSE_WHEEL_UP:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ADDON_ACTION_MOUSE_WHEEL_UP\n");
      break;
    case ADDON_ACTION_MOUSE_WHEEL_DOWN:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ADDON_ACTION_MOUSE_WHEEL_DOWN\n");
      break;
    case ADDON_ACTION_MOUSE_DRAG:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ADDON_ACTION_MOUSE_DRAG\n");
      break;
    case ADDON_ACTION_MOUSE_MOVE:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ADDON_ACTION_MOUSE_MOVE\n");
      break;
    case ADDON_ACTION_MOUSE_LONG_CLICK:
//      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
fprintf(stderr, "ADDON_ACTION_MOUSE_LONG_CLICK\n");
      break;
    default:
      break;
  }



//  if (state & GDK_SHIFT_MASK)
////    modifiers |= EVENTFLAG_SHIFT_DOWN;
////  if (state & GDK_LOCK_MASK)
////    modifiers |= EVENTFLAG_CAPS_LOCK_ON;
////  if (state & GDK_CONTROL_MASK)
////    modifiers |= EVENTFLAG_CONTROL_DOWN;
////  if (state & GDK_MOD1_MASK)
////    modifiers |= EVENTFLAG_ALT_DOWN;
////  if (state & GDK_BUTTON1_MASK)
////    modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
////  if (state & GDK_BUTTON2_MASK)
////    modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
////  if (state & GDK_BUTTON3_MASK)
////    modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
  return modifiers;
}

CWebBrowserClientBase::CWebBrowserClientBase(int iUniqueClientId, const WEB_ADDON_GUI_PROPS *props) :
  m_iUniqueClientId(iUniqueClientId),
  m_iBrowserCount(0),
  m_bIsDirty(false),
  m_bFocusOnEditableField(0),
  m_iMousePreviousFlags(0),
  m_iMousePreviousControl(MBT_LEFT),
  m_BrowserId(-1),
  m_Browser(NULL),
  m_pDevice(props->pDevice),
  m_iXPos(props->iXPos),
  m_iYPos(props->iYPos),
  m_iWidth(props->iWidth),
  m_iHeight(props->iHeight),
  m_iSkinXPos(props->iSkinXPos),
  m_iSkinYPos(props->iSkinYPos),
  m_iSkinWidth(props->iSkinWidth),
  m_iSkinHeight(props->iSkinHeight),
  m_fPixelRatio(props->fPixelRatio),
  m_strIdName(props->strName),
  m_iLeaveOpenTime(props->iLeaveOpenTime),
  m_iGUIItemLeft(props->iGUIItemLeft),
  m_iGUIItemRight(props->iGUIItemRight),
  m_iGUIItemTop(props->iGUIItemTop),
  m_iGUIItemBottom(props->iGUIItemBottom),
  m_iGUIItemBack(props->iGUIItemBack),
  m_bTransparentBackground(props->bUseTransparentBackground),
  m_pControlIdent(props->pControlIdent),
  m_isClosing(false)
{
  m_BackgroundColor[3]  = float(CefColorGetA(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[2]  = float(CefColorGetR(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[1]  = float(CefColorGetG(props->iBackgroundColorARGB)) / 255.0f;
  m_BackgroundColor[0]  = float(CefColorGetB(props->iBackgroundColorARGB)) / 255.0f;
  m_fMouseXScaleFactor  = float(m_iXPos + m_iWidth) / float(m_iSkinXPos + m_iSkinWidth);
  m_fMouseYScaleFactor  = float(m_iYPos + m_iHeight) / float(m_iSkinYPos + m_iSkinHeight);
  m_strTempStoreA       = (char*)malloc(TEMP_STORE_SIZE);
  m_strTempStoreB       = (char*)malloc(TEMP_STORE_SIZE);

  /*!> CEF related sub classes to manage web parts */
  m_resourceManager     = new CefResourceManager();
}

CWebBrowserClientBase::~CWebBrowserClientBase()
{
  free(m_strTempStoreA);
  free(m_strTempStoreB);
}

void CWebBrowserClientBase::SetBrowser(CefRefPtr<CefBrowser> browser)
{
  m_Browser = browser;
}

bool CWebBrowserClientBase::SetInactive()
{
  m_inactivateTime = time(NULL);
  return m_iLeaveOpenTime < 0;
}

bool CWebBrowserClientBase::SetActive()
{
  return true;
}

int CWebBrowserClientBase::CurrentInactiveCountdown()
{
  return time(NULL) - m_inactivateTime + m_iLeaveOpenTime;
}

void CWebBrowserClientBase::SendMessage(Message& message, bool wait)
{
  std::shared_ptr<CEvent> waitEvent;
  if (wait)
  {
    message.waitEvent.reset(new CEvent(true));
    waitEvent = message.waitEvent;
  }

  CLockObject lock(m_Mutex);

  Message* msg    = new Message();
  msg->dwMessage  = message.dwMessage;
  msg->param1     = message.param1;
  msg->param2     = message.param2;
  msg->param3     = message.param3;
  msg->lpVoid     = message.lpVoid;
  msg->strParam   = message.strParam;
  msg->params     = message.params;
  msg->waitEvent  = message.waitEvent;

  m_processQueue.push(msg);
  lock.Unlock();

  if (waitEvent)
    waitEvent->Wait(1000);
}

void CWebBrowserClientBase::HandleMessages()
{
  // process threadmessages
  CLockObject lock(m_Mutex);
  while (!m_processQueue.empty())
  {
    Message* pMsg = m_processQueue.front();
    m_processQueue.pop();

    std::shared_ptr<CEvent> waitEvent = pMsg->waitEvent;
    lock.Unlock(); // <- see the large comment in SendMessage ^
    switch (pMsg->dwMessage)
    {
      case TMSG_SET_CONTROL_READY:
        LOG_INTERNAL_MESSAGE(LOG_DEBUG, "Web control %s", pMsg->param1 ? "ready" : "failed");
        WEB->Control_SetControlReady(&m_addonHandle, pMsg->param1);
        break;
      case TMSG_SET_OPENED_ADDRESS:
        LOG_INTERNAL_MESSAGE(LOG_DEBUG, "Opened web site url '%s'", pMsg->strParam.c_str());
        WEB->Control_SetOpenedAddress(&m_addonHandle, pMsg->strParam.c_str());
        break;
      case TMSG_SET_OPENED_TITLE:
        LOG_INTERNAL_MESSAGE(LOG_DEBUG, "Opened web site title '%s'", pMsg->strParam.c_str());
        WEB->Control_SetOpenedTitle(&m_addonHandle, pMsg->strParam.c_str());
        break;
      case TMSG_SET_ICON_URL:
        LOG_INTERNAL_MESSAGE(LOG_DEBUG, "Opened web site set icon url '%s'", pMsg->strParam.c_str());
        WEB->Control_SetIconURL(&m_addonHandle, pMsg->strParam.c_str());
        break;
      case TMSG_SET_LOADING_STATE:
        WEB->Control_SetLoadingState(&m_addonHandle, pMsg->param1, pMsg->param2, pMsg->param3);
        break;
      case TMSG_SET_TOOLTIP:
        WEB->Control_SetTooltip(&m_addonHandle, pMsg->strParam.c_str());
        break;
      case TMSG_SET_STATUS_MESSAGE:
        WEB->Control_SetStatusMessage(&m_addonHandle, pMsg->strParam.c_str());
        break;
      case TMSG_HANDLE_ON_PAINT:
      {
        OnPaintMessage *data = static_cast<OnPaintMessage*>(pMsg->lpVoid);
        data->client.callback(pMsg->lpVoid);
        m_bIsDirty = true;
        break;
      }
      case TMSG_BROWSER_CLOSE:
      {
        CefRefPtr<CefBrowser> browser = (CefBrowser*)pMsg->lpVoid;

        LOG_INTERNAL_MESSAGE(LOG_DEBUG, "Browser close");
        break;
      }
      default:
        break;
    };

    if (waitEvent)
      waitEvent->Signal();
    delete pMsg;

    lock.Lock();
  }
}

bool CWebBrowserClientBase::OnAction(int actionId, int &nextItem)
{
  if (!m_Browser.get())
    return false;

  /*
   * Following actions forced to use on kodi itself
   * Need to be rechecked on chromium about usage, e.g. volume which also changeable there
   */
  switch (actionId)
  {
    case ADDON_ACTION_VOLUME_UP:
    case ADDON_ACTION_VOLUME_DOWN:
    case ADDON_ACTION_VOLAMP:
    case ADDON_ACTION_MUTE:
      return false;
    case ADDON_ACTION_NAV_BACK:
    case ADDON_ACTION_MENU:
    case ADDON_ACTION_PREVIOUS_MENU:
      return false;
    default:
      break;
  };

  CefRefPtr<CefBrowserHost> host = m_Browser->GetHost();

  CefKeyEvent key_event;
  key_event.windows_key_code = ActionIdToKeyboardCode(actionId);
  key_event.native_key_code = 0;
  key_event.modifiers = 0;
  key_event.is_system_key = false;
  if (key_event.windows_key_code == VKEY_RETURN)
  {
    // We need to treat the enter key as a key press of character \r.  This
    // is apparently just how webkit handles it and what it expects.
    key_event.unmodified_character = '\r';
  }

  key_event.type = KEYEVENT_RAWKEYDOWN;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_KEYUP;
  host->SendKeyEvent(key_event);
  key_event.type = KEYEVENT_CHAR;
  host->SendKeyEvent(key_event);
  return true;
}

bool CWebBrowserClientBase::OnMouseEvent(int id, double x, double y, double offsetX, double offsetY, int state)
{
  if (!m_Browser.get())
    return true;

  static const int scrollbarPixelsPerTick = 40;

  CefRefPtr<CefBrowserHost> host = m_Browser->GetHost();

  CefMouseEvent mouse_event;
  mouse_event.x = x * m_fMouseXScaleFactor;
  mouse_event.y = y * m_fMouseYScaleFactor;

  switch (id)
  {
    case ADDON_ACTION_MOUSE_LEFT_CLICK:
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_LEFT;
      break;
    case ADDON_ACTION_MOUSE_RIGHT_CLICK:
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_RIGHT, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_RIGHT, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_RIGHT;
      break;
    case ADDON_ACTION_MOUSE_MIDDLE_CLICK:
      mouse_event.modifiers = 0;
      mouse_event.modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
      host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, false, 1);
      host->SendMouseClickEvent(mouse_event, MBT_MIDDLE, true, 1);
      m_iMousePreviousFlags = mouse_event.modifiers;
      m_iMousePreviousControl = MBT_MIDDLE;
      break;
    case ADDON_ACTION_MOUSE_DOUBLE_CLICK:
      mouse_event.modifiers = m_iMousePreviousFlags;
      host->SendMouseClickEvent(mouse_event, m_iMousePreviousControl, false, 1);
      host->SendMouseClickEvent(mouse_event, m_iMousePreviousControl, true, 1);
      m_iMousePreviousControl = MBT_LEFT;
      m_iMousePreviousFlags = 0;
      break;
    case ADDON_ACTION_MOUSE_WHEEL_UP:
      host->SendMouseWheelEvent(mouse_event, 0, scrollbarPixelsPerTick);
      break;
    case ADDON_ACTION_MOUSE_WHEEL_DOWN:
      host->SendMouseWheelEvent(mouse_event, 0, -scrollbarPixelsPerTick);
      break;
    case ADDON_ACTION_MOUSE_DRAG:

      break;
    case ADDON_ACTION_MOUSE_MOVE:
    {
      bool mouse_leave = state == 3 ? true : false;
      host->SendMouseMoveEvent(mouse_event, mouse_leave);
      break;
    }
    case ADDON_ACTION_MOUSE_LONG_CLICK:

      break;
    default:
      break;
  }

  return true;
}

bool CWebBrowserClientBase::Initialize()
{
  return true;
}

bool CWebBrowserClientBase::Dirty()
{
  HandleMessages();
  bool ret = m_bIsDirty;
  m_bIsDirty = false;
  return ret;
}

static bool fir = false;

bool CWebBrowserClientBase::OpenWebsite(const char* strURL, bool single, bool allowMenus)
{
  if (!m_Browser.get())
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Called without present browser", __FUNCTION__);
    return false;
  }

  CefRefPtr<CefFrame> frame = m_Browser->GetMainFrame();
  if (!frame.get())
  {
    LOG_MESSAGE(LOG_ERROR, "%s - Called without present frame", __FUNCTION__);
    return false;
  }

  if (m_strStartupURL.empty())
    m_strStartupURL = strURL;

  frame->LoadURL(strURL);
  return true;
}

void CWebBrowserClientBase::CallSingleCommand(WEB_ADDON_SINGLE_COMMANDS command)
{
  switch (command)
  {
    case WEB_CMD_RELOAD:
      m_Browser->Reload();
      break;
    case WEB_CMD_STOP_LOAD:
      m_Browser->StopLoad();
      break;
    case WEB_CMD_NAV_BACK:
      m_Browser->GoBack();
      break;
    case WEB_CMD_NAV_FORWARD:
      m_Browser->GoForward();
      break;
    default:
      break;
  }
}

void CWebBrowserClientBase::SetAddonHandle(ADDON_HANDLE addonHandle)
{
  m_addonHandle.callerAddress = addonHandle->callerAddress;
  m_addonHandle.dataIdentifier = addonHandle->dataIdentifier;
  m_addonHandle.dataAddress = m_pControlIdent;

  WEB->GetUsedSkinNames(&m_addonHandle, *m_strTempStoreA, *m_strTempStoreB, TEMP_STORE_SIZE);

  std::string path = g_strUserPath;
  if (path.at(path.size() - 1) != '\\' &&
      path.at(path.size() - 1) != '/')
    path.append("/");

  m_strActiveSkinPath = StringUtils::Format("%sresources/skins/%s/", path.c_str(), m_strTempStoreA);
  if (!KODI->DirectoryExists(m_strActiveSkinPath.c_str()))
  {
    LOG_INTERNAL_MESSAGE(LOG_NOTICE, "Currently used skin '%s' not supported, falling back to default '%s'", m_strTempStoreA, m_strTempStoreB);
    m_strActiveSkinPath = StringUtils::Format("%sresources/skins/%s/", path.c_str(), m_strTempStoreB);
  }
}

/*!
 * @brief CefClient methods
 *
 * Implement this interface to provide handler implementations.
 */
//{
bool CWebBrowserClientBase::OnProcessMessageReceived(
    CefRefPtr<CefBrowser>                 browser,
    CefProcessId                          source_process,
    CefRefPtr<CefProcessMessage>          message)
{
  CEF_REQUIRE_UI_THREAD();

//  if (m_pMessageRouter->OnProcessMessageReceived(browser, source_process, message))
//    return true;

  // Check for messages from the client renderer.
  std::string message_name = message->GetName();
  if (message_name == "ClientRenderer.FocusedNodeChanged")
  {
    // A message is sent from ClientRenderDelegate to tell us whether the
    // currently focused DOM node is editable. Use of |focus_on_editable_field_|
    // is redundant with CefKeyEvent.focus_on_editable_field in OnPreKeyEvent
    // but is useful for demonstration purposes.
    m_bFocusOnEditableField = message->GetArgumentList()->GetBool(0);
    return true;
  }

  return false;
}
//}

/*! @brief CefDialogHandler methods */
//{
bool CWebBrowserClientBase::OnFileDialog(
    CefRefPtr<CefBrowser>                 browser,
    FileDialogMode                        mode,
    const CefString&                      title,
    const CefString&                      default_file_path,
    const std::vector<CefString>&         accept_filters,
    int                                   selected_accept_filter,
    CefRefPtr<CefFileDialogCallback>      callback)
{
  LOG_MESSAGE(LOG_DEBUG, "%s - Title: %s - default_file_path: %s - selected_accept_filter: %i - mode: %i",
    __FUNCTION__, title.ToString().c_str(), default_file_path.ToString().c_str(), selected_accept_filter, (int)mode);
  for (unsigned int i = 0; i < accept_filters.size(); i++)
    LOG_MESSAGE(LOG_DEBUG, "  - %02i: %s", i, accept_filters[i].ToString().c_str());
  return false;
}
//}

/*!
 * @brief CefContextMenuHandler methods
 */
//{
void CWebBrowserClientBase::OnBeforeContextMenu(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefContextMenuParams>       params,
    CefRefPtr<CefMenuModel>               model)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);

    LOG_MESSAGE(LOG_DEBUG, "CefContextMenuParams");
    LOG_MESSAGE(LOG_DEBUG, "- %ix%i - TypeFlags: 0x%X - ImageContents: %s - MediaType: %i - MediaStateFlags %i - EditStateFlags %i", params->GetXCoord(),
        params->GetYCoord(), (int)params->GetTypeFlags(), params->HasImageContents() ? "yes" : "no",
        (int)params->GetMediaType(), (int)params->GetMediaStateFlags(), (int)params->GetEditStateFlags());
    LOG_MESSAGE(LOG_DEBUG, "- LinkUrl:                %s", params->GetLinkUrl().ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- UnfilteredLinkUrl:      %s", params->GetUnfilteredLinkUrl().ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- SourceUrl:              %s", params->GetSourceUrl().ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- PageUrl:                %s", params->GetPageUrl().ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- FrameUrl :              %s", params->GetFrameUrl().ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- FrameCharset :          %s", params->GetFrameCharset().ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- SelectionText :         %s", params->GetSelectionText().ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- MisspelledWord :        %s", params->GetMisspelledWord().ToString().c_str());
    std::vector<CefString> suggestions;
    LOG_MESSAGE(LOG_DEBUG, "- DictionarySuggestions : %s", params->GetDictionarySuggestions(suggestions) ? "OK" : "fail");
    for (unsigned int i = 0; i < suggestions.size(); i++)
      LOG_MESSAGE(LOG_DEBUG, "  - %02i: %s", i, suggestions[i].ToString().c_str());
    LOG_MESSAGE(LOG_DEBUG, "- IsEditable :            %s", params->IsEditable() ? "yes" : "no");
    LOG_MESSAGE(LOG_DEBUG, "- IsSpellCheckEnabled :   %s", params->IsSpellCheckEnabled() ? "yes" : "no");
    LOG_MESSAGE(LOG_DEBUG, "- IsCustomMenu :          %s", params->IsCustomMenu() ? "yes" : "no");
    LOG_MESSAGE(LOG_DEBUG, "- IsPepperMenu :          %s", params->IsPepperMenu() ? "yes" : "no");
    LOG_MESSAGE(LOG_DEBUG, "CefMenuModel");
    LOG_MESSAGE(LOG_DEBUG, "- Count:                  %i", model->GetCount());
    for (unsigned int i = 0; i < model->GetCount(); i++)
      LOG_MESSAGE(LOG_DEBUG, "  - %02i: %s", i, model->GetLabelAt(i).ToString().c_str());



//    m_contextMenu.Open(browser, frame, params, model);
//    model->Clear();
//    model = new CWebGUIDialogContextMenu;
}

bool CWebBrowserClientBase::RunContextMenu(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefContextMenuParams>       params,
    CefRefPtr<CefMenuModel>               model,
    CefRefPtr<CefRunContextMenuCallback>  callback)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnContextMenuCommand(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefContextMenuParams>       params,
    int                                   command_id,
    EventFlags                            event_flags)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return true;
}

void CWebBrowserClientBase::OnContextMenuDismissed(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefDisplayHandler methods
 */
//{
void CWebBrowserClientBase::OnAddressChange(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      url)
{
  if (frame->IsMain())
  {
    Message tMsg = {TMSG_SET_OPENED_ADDRESS};
    tMsg.strParam = url.ToString().c_str();
    SendMessage(tMsg, false);
  }
}

void CWebBrowserClientBase::OnTitleChange(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      title)
{
  if (m_lastTitle != title.ToString().c_str())
  {
    m_lastTitle = title.ToString().c_str();

    Message tMsg = {TMSG_SET_OPENED_TITLE};
    tMsg.strParam = m_lastTitle.c_str();
    SendMessage(tMsg, false);
  }
}

void CWebBrowserClientBase::OnFaviconURLChange(
    CefRefPtr<CefBrowser>                 browser,
    const std::vector<CefString>&         icon_urls)
{
  LOG_INTERNAL_MESSAGE(LOG_DEBUG, "From currently opened web site given icon urls (first one used)");
  unsigned int listSize = icon_urls.size();
  for (unsigned int i = 0; i < listSize; ++i)
    LOG_INTERNAL_MESSAGE(LOG_DEBUG, " - Icon %i - %s", i+1, icon_urls[i].ToString().c_str());

  Message tMsg = {TMSG_SET_ICON_URL};
  if (listSize > 0)
    tMsg.strParam = icon_urls[0].ToString().c_str();
  else
    tMsg.strParam = "";
  SendMessage(tMsg, false);
}

bool CWebBrowserClientBase::OnTooltip(
    CefRefPtr<CefBrowser>                 browser,
    CefString&                            text)
{
  if (m_lastTooltip != text.ToString().c_str())
  {
    m_lastTooltip = text.ToString().c_str();

    Message tMsg = {TMSG_SET_TOOLTIP};
    tMsg.strParam = m_lastTooltip.c_str();
    SendMessage(tMsg, false);
  }

  return false;
}

void CWebBrowserClientBase::OnStatusMessage(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      value)
{
  if (m_lastStatusMsg != value.ToString().c_str())
  {
    m_lastStatusMsg = value.ToString().c_str();

    Message tMsg = {TMSG_SET_STATUS_MESSAGE};
    tMsg.strParam = m_lastStatusMsg.c_str();
    SendMessage(tMsg, false);
  }
}

bool CWebBrowserClientBase::OnConsoleMessage(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      message,
    const CefString&                      source,
    int                                   line)
{
  LOG_INTERNAL_MESSAGE(LOG_ERROR, "%s - Message: %s - Source: %s - Line: %i", __FUNCTION__, message.ToString().c_str(), source.ToString().c_str(), line);
  return true;
}
//}

/*!
 * @brief CefDownloadHandler methods
 */
/*//{
void CWebBrowserClientBase::OnBeforeDownload(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefDownloadItem>            download_item,
    const CefString&                      suggested_name,
    CefRefPtr<CefBeforeDownloadCallback>  callback)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnDownloadUpdated(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefDownloadItem>            download_item,
    CefRefPtr<CefDownloadItemCallback>    callback)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}*/

/*!
 * @brief CefDragHandler methods
 */
//{
bool CWebBrowserClientBase::OnDragEnter(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDragData> dragData,
    CefRenderHandler::DragOperationsMask mask)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

  /* Forbid dragging of link URLs */
  if(mask & DRAG_OPERATION_LINK)
    return true;

  return false;
}
//}

/*!
 * @brief CefGeolocationHandler methods
 */
//{
bool CWebBrowserClientBase::OnRequestGeolocationPermission(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      requesting_url,
    int                                   request_id,
    CefRefPtr<CefGeolocationCallback>     callback)
{
  CEF_REQUIRE_UI_THREAD();

  if (g_pWebManager->GetSettings()->GeolocationAllowance())
  {
    // Allow geolocation access from all websites.
    callback->Continue(true);
    return true;
  }
  return false;
}


void CWebBrowserClientBase::OnCancelGeolocationPermission(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      requesting_url,
    int                                   request_id)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefJSDialogHandler methods
 */
//{
bool CWebBrowserClientBase::OnJSDialog(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      origin_url,
    const CefString&                      accept_lang,
    JSDialogType                          dialog_type,
    const CefString&                      message_text,
    const CefString&                      default_prompt_text,
    CefRefPtr<CefJSDialogCallback>        callback,
    bool&                                 suppress_message)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::OnBeforeUnloadDialog(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      message_text,
    bool                                  is_reload,
    CefRefPtr<CefJSDialogCallback>        callback)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnResetDialogState(
    CefRefPtr<CefBrowser>                 browser)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnDialogClosed(
    CefRefPtr<CefBrowser>                 browser)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefFindHandler methods
 *
 * Implement this interface to handle events related to find results. The
 * methods of this class will be called on the UI thread.
 */
//{
void CWebBrowserClientBase::OnFindResult(
    CefRefPtr<CefBrowser>                 browser,
    int                                   identifier,
    int                                   count,
    const CefRect&                        selectionRect,
    int                                   activeMatchOrdinal,
    bool                                  finalUpdate)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefFocusHandler methods
 */
//{
void CWebBrowserClientBase::OnTakeFocus(
    CefRefPtr<CefBrowser>                 browser,
    bool                                  next)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnSetFocus(
    CefRefPtr<CefBrowser>                 browser,
    FocusSource                           source)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnGotFocus(
    CefRefPtr<CefBrowser>                 browser)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}
//}

/*!
 * @brief CefLifeSpanHandler methods
 */
//{
bool CWebBrowserClientBase::OnBeforePopup(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      target_url,
    const CefString&                      target_frame_name,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool                                  user_gesture,
    const CefPopupFeatures&               popupFeatures,
    CefWindowInfo&                        windowInfo,
    CefRefPtr<CefClient>&                 client,
    CefBrowserSettings&                   settings,
    bool*                                 no_javascript_access)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  if(browser->GetHost()->IsWindowRenderingDisabled())
      return true; /* Cancel popups in off-screen rendering mode */

  return false;
}

void CWebBrowserClientBase::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

  if (!m_Browser.get())
  {
    LOG_MESSAGE(LOG_DEBUG, "---------------------------------------------------------%s", __FUNCTION__);
    m_Browser   = browser;
    m_BrowserId = browser->GetIdentifier();

    Message tMsg = {TMSG_SET_CONTROL_READY};
    tMsg.param1 = true;
    SendMessage(tMsg, false);
  }
  else if(browser->IsPopup())
    m_popupBrowsers.push_back(browser); /* Add to the list of popup browsers */

  m_iBrowserCount++;
}

bool CWebBrowserClientBase::RunModal(CefRefPtr<CefBrowser> browser)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::DoClose(CefRefPtr<CefBrowser> browser)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

  /*
   * Closing the main window requires special handling. See the DoClose()
   * documentation in the CEF header for a detailed destription of this process
   */

  if (m_BrowserId == browser->GetIdentifier())
  {
    //FIXME: browser->GetHost(); /* Notify the browser that the parent window is about to close. */
    m_isClosing = true; /* Set a flag to indicate that the window close should be allowed */
  }

  return false; /* Allow the close. For windowed browsers this will result in the OS close event being sent */
}

void CWebBrowserClientBase::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);

  CEF_REQUIRE_UI_THREAD();

  if (m_BrowserId == browser->GetIdentifier())
  {
    m_Browser = nullptr;/* Free the browser pointer so that the browser can be destroyed */

    Message tMsg = {TMSG_BROWSER_CLOSE};
    tMsg.lpVoid = browser;
    SendMessage(tMsg, true);
  }
  else if (browser->IsPopup())
  {
    // Remove from the browser popup list.
    for (browserList::iterator it = m_popupBrowsers.begin(); it != m_popupBrowsers.end(); ++it)
    {
      if((*it)->IsSame(browser))
      {
        m_popupBrowsers.erase(it);
        break;
      }
    }
  }
}
//}

/*!
 * @brief CefLoadHandler methods
 */
//{
void CWebBrowserClientBase::OnLoadingStateChange(
    CefRefPtr<CefBrowser>                 browser,
    bool                                  isLoading,
    bool                                  canGoBack,
    bool                                  canGoForward)
{
  Message tMsg = {TMSG_SET_LOADING_STATE};
  tMsg.param1 = isLoading;
  tMsg.param2 = canGoBack;
  tMsg.param3 = canGoForward;
  SendMessage(tMsg, false);
}

//void CWebBrowserClientBase::OnLoadStart(
//    CefRefPtr<CefBrowser>                 browser,
//    CefRefPtr<CefFrame>                   frame)
//{
//    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
//}

//void CWebBrowserClientBase::OnLoadEnd(
//    CefRefPtr<CefBrowser>                 browser,
//    CefRefPtr<CefFrame>                   frame,
//    int                                   httpStatusCode)
//{
//    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
//}

void CWebBrowserClientBase::OnLoadError(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    ErrorCode                             errorCode,
    const CefString&                      errorText,
    const CefString&                      failedUrl)
{
  LOG_INTERNAL_MESSAGE(LOG_DEBUG, "Failed to load %s %s", errorText.ToString().c_str(), failedUrl.ToString().c_str());

  //! Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  //! Don't display an error for external protocols that we allow the Kodi to
  //! handle. See OnProtocolExecution().
  if (errorCode == ERR_UNKNOWN_URL_SCHEME)
  {
    std::string urlStr = frame->GetURL();
    if (urlStr.find("spotify:") == 0)
      return;
  }

  //! Load the error page.
  LoadErrorPage(frame, failedUrl, errorCode, errorText);
}
//}

/*!
 * @brief CefKeyboardHandler methods
 */
/*//{
bool CWebBrowserClientBase::OnPreKeyEvent(
    CefRefPtr<CefBrowser>                 browser,
    const CefKeyEvent&                    event,
    CefEventHandle                        os_event,
    bool*                                 is_keyboard_shortcut)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::OnKeyEvent(
    CefRefPtr<CefBrowser>                 browser,
    const CefKeyEvent&                    event,
    CefEventHandle                        os_event)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}
//}*/

/*!
 * @brief CefRequestHandler methods
 */
//{
bool CWebBrowserClientBase::OnBeforeBrowse(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    bool                                  is_redirect)
{
  std::string strURL = request->GetURL();
  LOG_MESSAGE(LOG_DEBUG, "%s - %s", __FUNCTION__, strURL.c_str());

  // We only care about these on the main frame
  if (!frame.get() || !frame->IsMain())
    return false;

//  m_pMessageRouter->OnBeforeBrowse(browser, frame);
  return false;
}

bool CWebBrowserClientBase::OnOpenURLFromTab(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    const CefString&                      target_url,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool                                  user_gesture)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

CefRequestHandler::ReturnValue CWebBrowserClientBase::OnBeforeResourceLoad(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefRefPtr<CefRequestCallback>         callback)
{
  CEF_REQUIRE_IO_THREAD();

  return m_resourceManager->OnBeforeResourceLoad(browser, frame, request, callback);
}

CefRefPtr<CefResourceHandler> CWebBrowserClientBase::GetResourceHandler(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request)
{
  CEF_REQUIRE_IO_THREAD();

  return m_resourceManager->GetResourceHandler(browser, frame, request);
}

void CWebBrowserClientBase::OnResourceRedirect(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefString&                            new_url)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

bool CWebBrowserClientBase::OnResourceResponse(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    CefRefPtr<CefRequest>                 request,
    CefRefPtr<CefResponse>                response)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::GetAuthCredentials(
    CefRefPtr<CefBrowser>                 browser,
    CefRefPtr<CefFrame>                   frame,
    bool                                  isProxy,
    const CefString&                      host,
    int                                   port,
    const CefString&                      realm,
    const CefString&                      scheme,
    CefRefPtr<CefAuthCallback>            callback)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

bool CWebBrowserClientBase::OnQuotaRequest(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      origin_url,
    int64                                 new_size,
    CefRefPtr<CefRequestCallback>         callback)
{
  CEF_REQUIRE_IO_THREAD();

  static const int64 max_size = 1024 * 1024 * 20;  // 20mb.

  // Grant the quota request if the size is reasonable.
  callback->Continue(new_size <= max_size);
  return true;
}

void CWebBrowserClientBase::OnProtocolExecution(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      url,
    bool&                                 allow_os_execution)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);

  CEF_REQUIRE_UI_THREAD();

  std::string urlStr = url;

  // Allow OS execution of Spotify URIs.
  if (urlStr.find("spotify:") == 0)
    allow_os_execution = true;
}

bool CWebBrowserClientBase::OnCertificateError(
    CefRefPtr<CefBrowser>                 browser,
    ErrorCode                             cert_error,
    const CefString&                      request_url,
    CefRefPtr<CefSSLInfo>                 ssl_info,
    CefRefPtr<CefRequestCallback>         callback)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return false;
}

void CWebBrowserClientBase::OnPluginCrashed(
    CefRefPtr<CefBrowser>                 browser,
    const CefString&                      plugin_path)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnRenderViewReady(
    CefRefPtr<CefBrowser>                 browser)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
}

void CWebBrowserClientBase::OnRenderProcessTerminated(
    CefRefPtr<CefBrowser>                 browser,
    TerminationStatus                     status)
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  CEF_REQUIRE_UI_THREAD();

//  m_pMessageRouter->OnRenderProcessTerminated(browser);

  // Don't reload if there's no start URL, or if the crash URL was specified.
  if (m_strStartupURL.empty() || m_strStartupURL == "chrome://crash")
    return;

  CefRefPtr<CefFrame> frame = browser->GetMainFrame();
  std::string url = frame->GetURL();

  // Don't reload if the termination occurred before any URL had successfully
  // loaded.
  if (url.empty())
    return;

  std::string start_url = m_strStartupURL;
  StringUtils::ToLower(url);
  StringUtils::ToLower(start_url);

  // Don't reload the URL that just resulted in termination.
  if (url.find(start_url) == 0)
    return;

  frame->LoadURL(m_strStartupURL);
}
//}


/*!
 * @brief CefRequestContextHandler methods
 */
//{
CefRefPtr<CefCookieManager> CWebBrowserClientBase::GetCookieManager()
{
  LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  return NULL;
}

bool CWebBrowserClientBase::OnBeforePluginLoad(
    const CefString&                      mime_type,
    const CefString&                      plugin_url,
    const CefString&                      top_origin_url,
    CefRefPtr<CefWebPluginInfo>           plugin_info,
    PluginPolicy*                         plugin_policy)
{
    LOG_MESSAGE(LOG_DEBUG, "%s", __FUNCTION__);
  // Always allow the PDF plugin to load.
  if (*plugin_policy != PLUGIN_POLICY_ALLOW && mime_type == "application/pdf")
  {
    *plugin_policy = PLUGIN_POLICY_ALLOW;
    return true;
  }

  return false;
}
//}

/*!
 * @brief Own internal handle functions
 */
//{
void CWebBrowserClientBase::LoadErrorPage(
    CefRefPtr<CefFrame>                   frame,
    const std::string&                    failed_url,
    cef_errorcode_t                       error_code,
    const std::string&                    other_info)
{
  std::stringstream ss;
  ss << "<html><head><title>Page failed to load</title></head>"
        "<body bgcolor=\"white\">"
        "<h3>Page failed to load.</h3>"
        "URL: <a href=\"" << failed_url << "\">"<< failed_url << "</a>"
        "<br/>Error: " << CURICheck::GetErrorString(error_code) <<
        " (" << error_code << ")";

  if (!other_info.empty())
    ss << "<br/>" << other_info;

  ss << "</body></html>";
  frame->LoadURL(CURICheck::GetDataURI(ss.str(), "text/html"));
}
//}
