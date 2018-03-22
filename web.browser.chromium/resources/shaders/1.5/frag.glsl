#version 150

uniform sampler2D m_samp;
in vec4 m_cord;
out vec4 fragColor;

void main ()
{
  fragColor = vec4(texture(m_samp, m_cord.xy));
}