/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

namespace SandboxControl
{
/*!
   * @brief To Set access rights of "chrome_sandbox" binary
   *
   * This call open dialog where user becomes asked about root password.
   *
   * Chromium documentation:
   * https://chromium.googlesource.com/chromium/src/+/master/docs/linux_suid_sandbox_development.md
   *
   * @todo Check and confirm still needed?:
   * Chromium documentation note: The Linux SUID sandbox is almost but not completely removed.
   * See https://bugs.chromium.org/p/chromium/issues/detail?id=598454 This page is mostly out-of-date.
   *
   * @return true if successfull done
   */
bool SetSandbox();
} // namespace SandboxControl
