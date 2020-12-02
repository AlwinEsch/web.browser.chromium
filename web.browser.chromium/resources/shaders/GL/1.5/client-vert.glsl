#version 150

// uniforms
uniform mat4 u_modelProjMatrix;

// attributes
in vec4 a_position;
in vec2 a_coord;

// varyings
out vec2 v_coord;

void main()
{
  gl_Position = u_modelProjMatrix * a_position;
  v_coord = a_coord;
}
