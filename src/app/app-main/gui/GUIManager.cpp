/*
 *  Copyright (C) 2015-2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "GUIManager.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace gui
{

CBrowserGUIManager::CBrowserGUIManager(CMainCEFProcess& main) : m_main(main)
{
}

bool CBrowserGUIManager::Create()
{
  m_file = new CBrowserDialogFile;
  return true;
}

void CBrowserGUIManager::Destroy()
{
  m_file = nullptr;
}

} /* namespace gui */
} /* namespace main */
} /* namespace app */
} /* namespace chromium */
