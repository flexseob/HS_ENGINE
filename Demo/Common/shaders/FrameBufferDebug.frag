#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D Frame;

void main()
{             
  FragColor = vec4( vec3(texture(Frame, TexCoords)),1.f);
}