// uniforms
uniform mat4 u_modelProjMatrix;

// attributes
attribute vec4 a_position;
attribute vec2 a_coord;

// varyings
varying vec2 v_coord;

void main()
{
  gl_Position = u_modelProjMatrix * a_position;
  v_coord = a_coord;
}
