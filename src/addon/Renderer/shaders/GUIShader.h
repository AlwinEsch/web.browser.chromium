#pragma once
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
#include "Matrix.h"

class ATTRIBUTE_HIDDEN CGUIShader : public CShaderProgram, public CMatrix
{
public:
  CGUIShader(std::string vert, std::string frag);

  void OnCompiledAndLinked() override;
  bool OnEnabled() override;
  void Free();

  GLint GetPosLoc() { return m_hPos; }
  GLint GetCordLoc() { return m_hCord; }

protected:
  GLint m_hTex = -1;
  GLint m_hProj = -1;
  GLint m_hModel = -1;
  GLint m_hPos = -1;
  GLint m_hCord = -1;

  GLfloat *m_proj = nullptr;
  GLfloat *m_model = nullptr;
};
