#version 460 core

out vec4 FragColor;

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoord;

struct Material
{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
    vec3 emissive;
};

uniform Material material;

void main()
{
    FragColor = vec4(material.diffuse , 1.0);
}
