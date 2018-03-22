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

#include "GUIShader.h"

CGUIShader::CGUIShader(std::string vert, std::string frag ) : CShaderProgram(vert, frag)
{
}

void CGUIShader::OnCompiledAndLinked()
{
  // Variables passed directly to the Fragment shader
  m_hTex = glGetUniformLocation(ProgramHandle(), "m_samp");

  // Variables passed directly to the Vertex shader
  m_hProj = glGetUniformLocation(ProgramHandle(), "m_proj");
  m_hModel = glGetUniformLocation(ProgramHandle(), "m_model");
  m_hPos = glGetAttribLocation(ProgramHandle(), "m_attrpos");
  m_hCord = glGetAttribLocation(ProgramHandle(), "m_attrcord");

  // It's okay to do this only one time. Textures units never change.
  glUseProgram(ProgramHandle());
  glUniform1i(m_hTex, 0);
  glUseProgram(0);
}

bool CGUIShader::OnEnabled()
{
  // This is called after glUseProgram()
  glUniformMatrix4fv(m_hProj, 1, GL_FALSE, GetMatrix(MM_PROJECTION));
  glUniformMatrix4fv(m_hModel, 1, GL_FALSE, GetMatrix(MM_MODELVIEW));

  return true;
}

void CGUIShader::Free()
{
  CShaderProgram::Free();
}
