#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPosition;
out vec3 Normal;
out vec3 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 CamPos;

void main()
{
    FragPosition = vec4(model * vec4(aPos, 1.0)).xyz;
    Normal = (transpose(inverse(model)) * vec4(aNormal,0.f)).xyz;
    TexCoord = vec3(aTexCoord.st,1.0);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
