/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "IOffscreenBase.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace renderer
{

IOffsceenBase::IOffsceenBase(CMainCEFProcess& mainProcess)
  : m_mainProcess(mainProcess)
{

}

IOffsceenBase::~IOffsceenBase()
{
}

} /* renderer */
} /* main */
} /* app */
} /* chromium */



