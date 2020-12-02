/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IOffscreenBase.h"

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace chromium
{
namespace app
{
namespace main
{
namespace renderer
{

class COffsceenBasePosix : public IOffsceenBase
{
public:
  COffsceenBasePosix(CMainCEFProcess& mainProcess);
  ~COffsceenBasePosix();

  bool Create() override;
  void Destroy() override;

  EGLDisplay GetEGLDisplay() { return m_eglDisplay; }
  EGLConfig GetEGLConfig() { return m_eglConfig; }
  EGLContext GetEGLContext() { return m_eglContext; }

private:
  bool m_started = false;

  EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
  EGLConfig m_eglConfig = EGL_NO_CONFIG_KHR;
  EGLContext m_eglContext = EGL_NO_CONTEXT;
};

} /* renderer */
} /* main */
} /* app */
} /* chromium */
