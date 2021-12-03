#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TexCoord;
out vec3 PureNormal;
out vec3 PurePos;

// out float Ratio;
out float EtaR;
out float EtaG;
out float EtaB;
// out float Ratio;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 CamPos;

uniform float RefractionIdx;

const float FresnelPower = 5.0;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPosition = vec3( model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse( model))) * aNormal;
    TexCoord = aTexCoord;
    PureNormal = Normal;
    PurePos= aPos;


    float Eta  = 1/ RefractionIdx;

    EtaR = Eta - 0.01f;
    EtaG = Eta + 0.00f; // Ratio of indices of refraction
    EtaB = Eta + 0.01f;


    
}
