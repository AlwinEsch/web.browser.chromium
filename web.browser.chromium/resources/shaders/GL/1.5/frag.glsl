#version 150

// uniforms
uniform sampler2D u_sampler;
uniform bool u_clearColor;
uniform vec4 u_backgroundColor;

// varyings
in vec2 v_coord;

out vec4 fragColor;

void main()
{
  if (u_clearColor)
    fragColor = u_backgroundColor;
  else
    fragColor = vec4(texture(u_sampler, v_coord));
}
