// uniforms
uniform sampler2D u_sampler;
uniform bool u_clearColor;
uniform vec4 u_backgroundColor;

// varyings
varying vec2 v_coord;

void main()
{
  if (u_clearColor)
    gl_FragColor = u_backgroundColor;
  else
    gl_FragColor = vec4(texture2D(u_sampler, v_coord));
}
