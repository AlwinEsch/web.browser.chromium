/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "OpenGLExtensionCheck.h"

#include <algorithm>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <vector>

namespace GLExtCheck
{

static std::vector<std::string> displayExts;

std::vector<std::string> SplitExtensions(const std::string& str)
{
  std::vector<std::string> strings;
  size_t start;
  size_t end = 0;
  while ((start = str.find_first_not_of(' ', end)) != std::string::npos)
  {
    end = str.find(' ', start);
    strings.push_back(str.substr(start, end - start));
  }
  return strings;
}

void FindSupportedExtensions(EGLDisplay m_eglDisplay)
{
  // Get the GL version number
  int renderVersionMajor = 0;
  int renderVersionMinor = 0;

  displayExts = SplitExtensions(eglQueryString(m_eglDisplay, EGL_EXTENSIONS));

  const char* ver = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  if (ver != 0)
  {
    sscanf(ver, "%d.%d", &renderVersionMajor, &renderVersionMinor);
  }

  if (renderVersionMajor > 3 || (renderVersionMajor == 3 && renderVersionMinor >= 2))
  {
    GLint n = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    if (n > 0)
    {
      GLint i;
      for (i = 0; i < n; i++)
      {
        displayExts.emplace_back(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
      }
    }
  }
  else
  {
    auto extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    if (extensions)
    {
      std::vector<std::string> ext = SplitExtensions(extensions);
      displayExts.insert(displayExts.begin(), ext.begin(), ext.end());
    }
  }
}

bool CheckExtensionSupported(const std::string& name)
{
  if (displayExts.empty())
  {
    FindSupportedExtensions(eglGetCurrentDisplay());
  }
  return std::find(displayExts.begin(), displayExts.end(), name) != displayExts.end();
}

void* GetEGLFunc(const std::string& name)
{
  return reinterpret_cast<void*>(eglGetProcAddress(name.c_str()));
}

const char* GetEGLErrorString(uint32_t error)
{
  switch (error)
  {
    case EGL_SUCCESS:
      return "EGL_SUCCESS";
    case EGL_NOT_INITIALIZED:
      return "EGL_NOT_INITIALIZED";
    case EGL_BAD_ACCESS:
      return "EGL_BAD_ACCESS";
    case EGL_BAD_ALLOC:
      return "EGL_BAD_ALLOC";
    case EGL_BAD_ATTRIBUTE:
      return "EGL_BAD_ATTRIBUTE";
    case EGL_BAD_CONFIG:
      return "EGL_BAD_CONFIG";
    case EGL_BAD_CONTEXT:
      return "EGL_BAD_CONTEXT";
    case EGL_BAD_CURRENT_SURFACE:
      return "EGL_BAD_CURRENT_SURFACE";
    case EGL_BAD_DISPLAY:
      return "EGL_BAD_DISPLAY";
    case EGL_BAD_MATCH:
      return "EGL_BAD_MATCH";
    case EGL_BAD_NATIVE_PIXMAP:
      return "EGL_BAD_NATIVE_PIXMAP";
    case EGL_BAD_NATIVE_WINDOW:
      return "EGL_BAD_NATIVE_WINDOW";
    case EGL_BAD_PARAMETER:
      return "EGL_BAD_PARAMETER";
    case EGL_BAD_SURFACE:
      return "EGL_BAD_SURFACE";
    case EGL_CONTEXT_LOST:
      return "EGL_CONTEXT_LOST";
    default:
      return "UNKNOWN";
  }
}

// Returns the last EGL error as a string.
const char* GetLastEGLErrorString()
{
  return GetEGLErrorString(eglGetError());
}

} /* namespace OpenGLExtensionCheck */
