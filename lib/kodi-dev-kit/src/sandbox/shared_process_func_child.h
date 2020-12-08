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
// typedef enum funcIdChild : int
// {
//   master_KillChild,
//
//   instance_web_set_mute,
//   instance_web_set_language,
//   instance_web_create_control,
//   instance_web_destroy_control,
//   instance_web_MainLoop,
//   instance_control_set_active,
//   instance_control_set_inactive,
//   instance_control_get_unique_id,
//
//   instance_control_Render,
//   instance_control_Dirty,
//   instance_control_OnInit,
//   instance_control_OnAction,
//   instance_control_OnMouseEvent,
//   instance_control_OpenWebsite,
//   instance_control_GetHistory,
//   instance_control_SearchText,
//   instance_control_StopSearch,
//   instance_control_Reload,
//   instance_control_StopLoad,
//   instance_control_GoBack,
//   instance_control_GoForward,
//   instance_control_OpenOwnContextMenu,
//   instance_control_ScreenSizeChange,
// } funcIdChild;
//
// typedef std::tuple<std::string> msg_child_master_KillChild;
//
// typedef uint64_t stream_msg_instance_web_MainLoop;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_set_language;
// typedef std::tuple<uint64_t, bool> stream_msg_instance_set_mute;
// typedef std::tuple<uint64_t, uint64_t, int, std::string> stream_msg_instance_web_create_control;
// typedef std::tuple<uint64_t, uint64_t> stream_msg_instance_web_destroy_control;
// typedef std::tuple<uint64_t> stream_msg_instance_control_set_active;
// typedef std::tuple<uint64_t> stream_msg_instance_control_set_inactive;
// typedef std::tuple<uint64_t> stream_msg_instance_control_get_unique_id;
// typedef std::tuple<uint64_t> stream_msg_instance_control_Render;
// typedef std::tuple<uint64_t> stream_msg_instance_control_Dirty;
// typedef std::tuple<uint64_t> stream_msg_instance_control_OnInit;
// typedef std::tuple<uint64_t,
//                    int,
//                    std::string,
//                    float,
//                    float,
//                    float,
//                    float,
//                    float,
//                    float,
//                    float,
//                    unsigned int,
//                    unsigned int,
//                    uint32_t,
//                    std::string,
//                    int>
//     stream_msg_instance_control_OnAction;
// typedef std::tuple<uint64_t, int, double, double, double, double, int>
//     stream_msg_instance_control_OnMouseEvent;
// typedef std::tuple<uint64_t, std::string> stream_msg_instance_control_OpenWebsite;
// typedef std::tuple<uint64_t, std::vector<std::string>, bool> stream_msg_instance_control_GetHistory;
// typedef std::tuple<uint64_t, std::string, bool, bool, bool> stream_msg_instance_control_SearchText;
// typedef std::tuple<uint64_t, bool> stream_msg_instance_control_StopSearch;
// typedef std::tuple<uint64_t> stream_msg_instance_control_Reload;
// typedef std::tuple<uint64_t> stream_msg_instance_control_StopLoad;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GoBack;
// typedef std::tuple<uint64_t> stream_msg_instance_control_GoForward;
// typedef std::tuple<uint64_t> stream_msg_instance_control_OpenOwnContextMenu;
// typedef std::tuple<uint64_t, float, float, float, float, bool>
//     stream_msg_instance_control_ScreenSizeChange;
