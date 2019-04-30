/*
 *  Copyright (C) 2015-2019 Alwin Esch (Team Kodi)
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "GUIManager.h"

CBrowserGUIManager::CBrowserGUIManager(CWebBrowser* instance) : m_instance(instance)
{
  m_file = new CBrowserDialogFile;
}

bool CBrowserGUIManager::Create()
{

  //CreateThread();
  return true;
}

void CBrowserGUIManager::Destroy()
{
  //StopThread();
}

void CBrowserGUIManager::Process()
{

  while (!IsStopped())
  {

  }
}

