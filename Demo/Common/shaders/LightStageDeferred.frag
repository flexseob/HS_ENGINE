#version 430
// Input textures for the deferred shading lighting pass
layout(binding=0)uniform sampler2D viewPosBuffer;
layout(binding=1)uniform sampler2D normalBuffer;
layout(binding=2)uniform sampler2D uvBuffer;
layout(binding=3)uniform sampler2D depthBuffer;
layout(binding=4)uniform sampler2D diffuseBuffer;
layout(binding=5)uniform sampler2D specularBuffer;
layout(binding=6)uniform sampler2D ambientBuffer;
layout(binding=7)uniform sampler2D emissiveBuffer;

// Interpolating vertex attributes over the rasterizer
in VS_OUT
{
    vec4 vertexPosition;// interpolated vPosition
    vec2 vertexUV;// interpolated vUV
}fs_in;

out vec3 FragColor;

const int MAX_LIGHT=500;



struct Light
{
    vec3 position;
    float cutoff;
    vec3 diffuse;
    float outcutoff;
    vec3 ambient;
    float falloff;
    vec3 specular;
    float padding4;
    vec3 direction;
    int LightType;
};

layout(std140)uniform light{
    Light f_light[MAX_LIGHT];
};



uniform vec3 attenuation;

uniform int NumOfLight;
uniform vec3 lightColor;
uniform vec3 CamPos;
uniform mat4 view;

uniform vec3 globalAmbientColor;
uniform vec3 FogColor;
uniform float FogDistanceNear;
uniform float FogDistanceFar;
uniform mat4 model;

vec2 Final_TexCoord;

vec3 DirLight(Light lig,vec3 normal,vec3 fragposition,vec3 viewDir)
{
    vec3 lightDir=normalize(-lig.direction);
    vec3 norm=normalize(normal);
    
    float diff=max(dot(norm,lightDir),0.);
    
    vec3 reflectDirection=2*dot(norm,lightDir)*norm-lightDir;
    
    vec3 specular=vec3(0.);
    
    float spec=pow(max(dot(viewDir,reflectDirection),0.),texture(ambientBuffer,fs_in.vertexUV).a *128);
    
    specular=lig.specular*spec*texture(specularBuffer,fs_in.vertexUV).rgb;
    
    vec3 ambient=lig.ambient*texture(ambientBuffer,fs_in.vertexUV).rgb;
    
    vec3 diffuse;
    
    diffuse=lig.diffuse*diff * texture(diffuseBuffer,fs_in.vertexUV).rgb;
    
 

    return(ambient+diffuse+specular);
}

vec3 PointLight(Light lig,vec3 normal,vec3 fragposition,vec3 viewDir,vec3 att)
{
    vec3 lightDir=normalize(lig.position-fragposition);
    vec3 norm=normalize(normal);
    
    float diff=max(dot(norm,lightDir),0.);
    
    vec3 reflectDir=2*dot(norm,lightDir)*norm-lightDir;
    float spec=pow(max(dot(viewDir,reflectDir),0.),texture(ambientBuffer,fs_in.vertexUV).a *128);
    
    float distance=length(lig.position-fragposition);
    
    float attenuation=min(1./(att.x+att.y*distance+att.z*(distance*distance)),1.);
    
    vec3 ambient=lig.ambient * texture(ambientBuffer,fs_in.vertexUV).rgb;
    vec3 diffuse;
    vec3 specular;
    
    specular=lig.specular*spec *texture(specularBuffer,fs_in.vertexUV).rgb;
    
    diffuse=lig.diffuse*diff *texture(diffuseBuffer,fs_in.vertexUV).rgb;
    
    ambient*=attenuation;
    diffuse*=attenuation;
    specular*=attenuation;
    
    return(ambient+diffuse+specular);
}

vec3 SpotLight(Light lig,vec3 normal,vec3 fragposition,vec3 viewDir,vec3 att)
{
    vec3 lightDir=normalize(lig.position-fragposition);
    vec3 norm=normalize(normal);
    
    float diff=max(dot(normal,lightDir),0.);
    
    vec3 reflectDir=2*dot(norm,lightDir)*norm-lightDir;
    float spec=pow(max(dot(viewDir,reflectDir),0.),texture(ambientBuffer,fs_in.vertexUV).a *128);
    
    float distance=length(lig.position-fragposition);
    float attenuation=min(1./(att.x+att.y*distance+att.z*(distance*distance)),1.);
    
    vec3 ambient=lig.ambient;
    vec3 diffuse;
    vec3 specular;
    
    specular=lig.specular*spec*texture(specularBuffer,fs_in.vertexUV).rgb;
    diffuse=lig.diffuse*diff*texture(diffuseBuffer,fs_in.vertexUV).rgb;
    
    ambient*=attenuation*texture(ambientBuffer,fs_in.vertexUV).rgb;
    diffuse*=attenuation;
    specular*=attenuation;
    
    //light target
    float alpha=dot(-lightDir,normalize(lig.direction));

    float Spotlighteffect=0;
    
    if(alpha<lig.outcutoff)
    {
        Spotlighteffect=0;
    }
    else if(alpha>lig.cutoff)
    {
        Spotlighteffect=1;
    }
    else
    {
        Spotlighteffect=pow((alpha-cos(lig.outcutoff))/(cos(lig.cutoff)-cos(lig.outcutoff)),lig.falloff);
    }
    
    diffuse*=Spotlighteffect;
    specular*=Spotlighteffect;
    
    return(ambient+diffuse+specular);
}

void main()
{
    vec3 result=vec3(0.);
    
    vec3 Normal=texture(normalBuffer,fs_in.vertexUV).rgb;
    vec3 FragPosition=texture(viewPosBuffer,fs_in.vertexUV).rgb;
    vec3 viewDirection=normalize(CamPos-FragPosition);


    for(int i=0;i<NumOfLight;++i)
    {
        if(f_light[i].LightType==0)
        {
            result+=DirLight(f_light[i],Normal,FragPosition,viewDirection);
        }
        else if(f_light[i].LightType==1)
        {
            result+=PointLight(f_light[i],Normal,FragPosition,viewDirection,attenuation);
        }
        else if(f_light[i].LightType==2)
        {
            result+=SpotLight(f_light[i],Normal,FragPosition,viewDirection,attenuation);
        }
    }
    
     result += globalAmbientColor *texture(ambientBuffer,fs_in.vertexUV).rgb + texture(emissiveBuffer,fs_in.vertexUV).rgb;
    float FogResult=(FogDistanceFar-length(-FragPosition))/(FogDistanceFar-FogDistanceNear);
    result=FogResult*result+(1-FogResult)*FogColor;
 
    result.x=min(result.x,1.);
    result.y=min(result.y,1.);
    result.z=min(result.z,1.);
    
    FragColor=result;
    
}