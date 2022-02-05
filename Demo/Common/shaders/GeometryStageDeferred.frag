#version 460 core
in vec3 FragPosition;
in vec3 Normal;
in vec3 TexCoord;

layout (location=0) out vec3 vPos;
layout (location=1) out vec3 normal;
layout (location=2) out vec3 UVs;
layout (location=3) out vec3 depthValue;
layout (location=4) out vec3 diffuse;
layout (location=5) out vec3 specular;
layout (location=6) out vec4 ambient;
layout (location=7) out vec3 emissive;

struct Material
{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
    vec3 emissive;
};

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

uniform bool diffuse_texture_isexist;
uniform bool specular_texture_isexist;


uniform Material material;

void main()
{
    
    vPos = FragPosition;
    normal = normalize(Normal);
    UVs = TexCoord;

    float distFromCamera = length(FragPosition);
    depthValue = vec3(distFromCamera);
    if(diffuse_texture_isexist)
    {
        diffuse.rgb = texture(diffuse_texture, TexCoord.st).rgb;
    }else
    {
         diffuse = material.diffuse;
    }
    if(specular_texture_isexist)
    {
        specular.rgb = texture(specular_texture, TexCoord.st).rgb;
    }else
    {
        specular = material.specular;
    }
    ambient.rgb = material.ambient;
    ambient.a = material.shininess;
    emissive = material.emissive;
}