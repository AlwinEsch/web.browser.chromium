/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "Shader.h"
#include <stdio.h>
#include <kodi/General.h>
#include <kodi/Filesystem.h>

#define LOG_SIZE 1024
#define GLchar char

//////////////////////////////////////////////////////////////////////
// CShader
//////////////////////////////////////////////////////////////////////
bool CShader::LoadSource(std::string &file)
{
  char buffer[1024];

  kodi::vfs::CFile handle;
  if (!handle.OpenFile(file.c_str(), 0))
    return false;

  size_t len = handle.Read(buffer, sizeof(buffer));
  m_source.assign(buffer);
  m_source[len] = 0;

  return true;
}

//////////////////////////////////////////////////////////////////////
// CGLSLVertexShader
//////////////////////////////////////////////////////////////////////

bool CVertexShader::Compile()
{
  GLint params[4];

  Free();

  m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *ptr = m_source.c_str();
  glShaderSource(m_vertexShader, 1, &ptr, 0);
  glCompileShader(m_vertexShader);
  glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, params);
  if (params[0] != GL_TRUE)
  {
    GLchar log[LOG_SIZE];
    glGetShaderInfoLog(m_vertexShader, LOG_SIZE, nullptr, log);
    m_lastLog = log;
    m_compiled = false;
  }
  else
  {
    GLchar log[LOG_SIZE];
    glGetShaderInfoLog(m_vertexShader, LOG_SIZE, nullptr, log);
    m_lastLog = log;
    m_compiled = true;
  }
  return m_compiled;
}

void CVertexShader::Free()
{
  if (m_vertexShader)
    glDeleteShader(m_vertexShader);
  m_vertexShader = 0;
}

//////////////////////////////////////////////////////////////////////
// CVisGLSLPixelShader
//////////////////////////////////////////////////////////////////////
bool CPixelShader::Compile()
{
  GLint params[4];

  Free();

  if (m_source.length()==0)
    return true;

  m_pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *ptr = m_source.c_str();
  glShaderSource(m_pixelShader, 1, &ptr, 0);
  glCompileShader(m_pixelShader);
  glGetShaderiv(m_pixelShader, GL_COMPILE_STATUS, params);
  if (params[0] != GL_TRUE)
  {
    GLchar log[LOG_SIZE];
    glGetShaderInfoLog(m_pixelShader, LOG_SIZE, nullptr, log);
    m_lastLog = log;
    m_compiled = false;
  }
  else
  {
    GLchar log[LOG_SIZE];
    glGetShaderInfoLog(m_pixelShader, LOG_SIZE, nullptr, log);
    m_lastLog = log;
    m_compiled = true;
  }
  return m_compiled;
}

void CPixelShader::Free()
{
  if (m_pixelShader)
    glDeleteShader(m_pixelShader);
  m_pixelShader = 0;
}

//////////////////////////////////////////////////////////////////////
// CShaderProgram
//////////////////////////////////////////////////////////////////////

CShaderProgram::CShaderProgram(std::string &vert, std::string &frag)
{
  std::string path;

#if defined(HAS_GL)
  int major = 0;
  int minor = 0;
  const char* ver = (const char*)glGetString(GL_VERSION);
  if (ver != 0)
  {
    sscanf(ver, "%d.%d", &major, &minor);
  }

  if (major > 3 || (major == 3 && minor >= 2))
    path = kodi::GetAddonPath("/resources/shaders/1.5/");
  else
    path = kodi::GetAddonPath("/resources/shaders/1.2/");
#else
  path = kodi::GetAddonPath("/resources/shaders/1.2/");
#endif
  
  std::string file;

  m_pFP = new CPixelShader();
  file = path + frag;
  m_pFP->LoadSource(file);
  m_pVP = new CVertexShader();
  file = path + vert;
  m_pVP->LoadSource(file);
}

void CShaderProgram::Free()
{
  m_pVP->Free();
  m_pFP->Free();
  if (m_shaderProgram)
    glDeleteProgram(m_shaderProgram);
  m_shaderProgram = 0;
  m_ok = false;
}

bool CShaderProgram::CompileAndLink()
{
  GLint params[4];

  // free resources
  Free();
  
  // compiled vertex shader
  if (!m_pVP->Compile())
    return false;

  // compile pixel shader
  if (!m_pFP->Compile())
  {
    m_pVP->Free();
    return false;
  }

  // create program object
  if (!(m_shaderProgram = glCreateProgram()))
    goto error;

  // attach the vertex shader
  glAttachShader(m_shaderProgram, m_pVP->Handle());
  glAttachShader(m_shaderProgram, m_pFP->Handle());

  // link the program
  glLinkProgram(m_shaderProgram);
  glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, params);
  if (params[0] != GL_TRUE)
  {
    GLchar log[LOG_SIZE];
    glGetProgramInfoLog(m_shaderProgram, LOG_SIZE, nullptr, log);
    fprintf(stderr, "--------------------CompileAndLink - 5 ERR '%s'\n", log);
    goto error;
  }

  m_validated = false;
  m_ok = true;
  OnCompiledAndLinked();
  return true;

 error:
  m_ok = false;
  Free();
  return false;
}

bool CShaderProgram::Enable()
{
  if (OK())
  {
    glUseProgram(m_shaderProgram);
    if (OnEnabled())
    {
      if (!m_validated)
      {
        // validate the program
        GLint params[4];
        glValidateProgram(m_shaderProgram);
        glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, params);
        if (params[0] != GL_TRUE)
        {
          GLchar log[LOG_SIZE];
          glGetProgramInfoLog(m_shaderProgram, LOG_SIZE, nullptr, log);
        }
        m_validated = true;
      }
      return true;
    }
    else
    {
      glUseProgram(0);
      return false;
    }
    return true;
  }
  return false;
}

void CShaderProgram::Disable()
{
  if (OK())
  {
    glUseProgram(0);
    OnDisabled();
  }
}
