/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

namespace chromium
{
namespace app
{
namespace main
{

class CMainCEFProcess;

namespace renderer
{

class IOffsceenBase
{
public:
  IOffsceenBase(CMainCEFProcess& mainProcess);
  virtual ~IOffsceenBase();

  virtual bool Create() { return true; }
  virtual void Destroy() { }

private:
  CMainCEFProcess& m_mainProcess;
};

} /* renderer */
} /* main */
} /* app */
} /* chromium */

