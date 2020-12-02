/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <string>
#include <vector>

namespace kodi
{
namespace gui
{
namespace dialogs
{
namespace FileBrowser
{

bool ShowAndGetDirectory(const std::string& shares,
                         const std::string& heading,
                         std::string& path,
                         bool writeOnly = false);
bool ShowAndGetFile(const std::string& shares,
                    const std::string& mask,
                    const std::string& heading,
                    std::string& path,
                    bool useThumbs = false,
                    bool useFileDirectories = false);
bool ShowAndGetFileFromDir(const std::string& directory,
                           const std::string& mask,
                           const std::string& heading,
                           std::string& path,
                           bool useThumbs = false,
                           bool useFileDirectories = false,
                           bool singleList = false);
bool ShowAndGetFileList(const std::string& shares,
                        const std::string& mask,
                        const std::string& heading,
                        std::vector<std::string>& fileList,
                        bool useThumbs = false,
                        bool useFileDirectories = false);
bool ShowAndGetSource(std::string& path,
                      bool allowNetworkShares,
                      const std::string& additionalShare = "",
                      const std::string& type = "");
bool ShowAndGetImage(const std::string& shares, const std::string& heading, std::string& path);
bool ShowAndGetImageList(const std::string& shares,
                         const std::string& heading,
                         std::vector<std::string>& file_list);


} /* namespace FileBrowser */
} /* namespace dialogs */
} /* namespace gui */
} /* namespace kodi */
