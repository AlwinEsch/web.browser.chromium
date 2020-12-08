#version 150

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Texture1;

void main()
{
   FragColor = texture(Texture1, TexCoords);
}
