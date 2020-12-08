/*
 *  Copyright (C) 2020 Alwin Esch (Team Kodi) <https://kodi.tv>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "OffscreenBasePosix.h"

namespace chromium
{
namespace app
{
namespace main
{
namespace renderer
{

COffsceenBasePosix::COffsceenBasePosix(CMainCEFProcess& mainProcess)
  : IOffsceenBase(mainProcess)
{

}

COffsceenBasePosix::~COffsceenBasePosix()
{
  Destroy();
}

bool COffsceenBasePosix::Create()
{
  if (m_started)
    return true;

  m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  // initialize the EGL display connection
  EGLint major, minor;
  eglInitialize(m_eglDisplay, &major, &minor);

  // Set OpenGL rendering API
  eglBindAPI(EGL_OPENGL_API);

  EGLint num_config;
  const EGLint attribute_list_config[] = {
      EGL_SURFACE_TYPE,
      EGL_PBUFFER_BIT,
      EGL_RENDERABLE_TYPE,
      EGL_OPENGL_BIT,
      EGL_RED_SIZE,
      1,
      EGL_GREEN_SIZE,
      1,
      EGL_BLUE_SIZE,
      1,
      EGL_ALPHA_SIZE,
      1,
      EGL_NONE,
  };

  eglChooseConfig(m_eglDisplay, attribute_list_config, &m_eglConfig, 1, &num_config);

  // create an EGL rendering context
  const EGLint attrib_list[] = {EGL_CONTEXT_MAJOR_VERSION_KHR,
                                3,
                                EGL_CONTEXT_MINOR_VERSION_KHR,
                                2,
                                EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,
                                EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
                                EGL_NONE};
  m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, attrib_list);

  m_started = true;
  return true;
}

void COffsceenBasePosix::Destroy()
{
  if (!m_started)
    return;

  m_started = false;
}

} /* renderer */
} /* main */
} /* app */
} /* chromium */


