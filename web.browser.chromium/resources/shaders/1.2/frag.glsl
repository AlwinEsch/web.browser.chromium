#ifndef GL_ES
#define lowp
#endif

uniform sampler2D m_samp;
varying lowp vec4 m_cord;

void main ()
{
  gl_FragColor = vec4(texture2D(m_samp, m_cord.xy));
}
