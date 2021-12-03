#version 460 core
out vec4 FragColor;

in vec3 FragPosition;
in vec3 Normal;  
in vec2 TexCoord;

in vec3 PureNormal;
in vec3 PurePos;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

uniform bool diffuse_texture_isexist;
uniform bool specular_texture_isexist;

uniform bool gpu_calculation;

uniform bool UV_Entity_Normal;

uniform int UV_mapping_mode;

struct Material
{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
    vec3 emissive;
};
const int MAX_LIGHT = 16;
struct Light 
{
    // int LightType;
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

layout(std140) uniform light{
    Light f_light[MAX_LIGHT];
};

uniform Material material;
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


vec3 DirLight(Light lig, vec3 normal,vec3 fragposition, vec3 viewDir)
{
    vec3 lightDir = normalize(-lig.direction);
    vec3 norm = normalize(normal);

    vec3 viewDir1 = normalize(-fragposition);
        
    vec3 viewVector = normalize(CamPos-fragposition);
	vec3 HalfVector = normalize(lightDir+viewVector);
  
    float diff = max(dot(norm, lightDir), 0.0);

     vec3 specular =vec3(0.0);
     
   
        float spec = pow(max(dot(viewDir, HalfVector), 0.0), material.shininess*128);
        if(specular_texture_isexist)
        {
             specular = lig.specular * spec * vec3(texture(specular_texture,Final_TexCoord));
        }
        else
        {
             specular= lig.specular * spec * material.specular;   
        }


    vec3 ambient  = lig.ambient * material.ambient;

    vec3 diffuse;
    if(diffuse_texture_isexist)
    {
        diffuse = lig.diffuse * diff * vec3(texture(diffuse_texture, Final_TexCoord));
    }
    else
    {
        diffuse  = lig.diffuse * diff * material.diffuse;
    }

    return (ambient + diffuse + specular);
}  

vec3 PointLight(Light lig, vec3 normal, vec3 fragposition, vec3 viewDir, vec3 att)
{
    vec3 lightDir = normalize(lig.position - fragposition);
    vec3 norm = normalize(normal);

    float diff = max(dot(norm, lightDir), 0.0);
  


    vec3 viewVector = normalize(CamPos-fragposition);
	vec3 HalfVector = normalize(lightDir+viewVector);

    float spec = pow(max(dot(viewDir, HalfVector), 0.0), material.shininess*128);
    
    float distance    = length(lig.position - fragposition);
    
    float attenuation = min(1.0 / (att.x + att.y * distance +  att.z * (distance * distance)),1.0);    
 
    vec3 ambient = lig.ambient  * material.ambient;
    vec3 diffuse;
    vec3 specular;
        if(specular_texture_isexist)
        {
             specular = lig.specular * spec * vec3(texture(specular_texture,Final_TexCoord));
        }
        else
        {
             specular= lig.specular * spec * material.specular;   
        }
    if(diffuse_texture_isexist)
    {
        diffuse = lig.diffuse * diff * vec3(texture(diffuse_texture, Final_TexCoord));
    }
    else
    {
        diffuse  = lig.diffuse * diff * material.diffuse;
    }
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;


    return (ambient + diffuse + specular);
}


vec3 SpotLight(Light lig, vec3 normal, vec3 fragposition, vec3 viewDir, vec3 att)
{
    vec3 lightDir = normalize(lig.position - fragposition);
    vec3 norm = normalize(normal);

    float diff = max(dot(normal, lightDir), 0.0);


    vec3 viewVector = normalize(CamPos-fragposition);
	vec3 HalfVector = normalize(lightDir+viewVector);

    float spec = pow(max(dot(viewDir, HalfVector), 0.0), material.shininess*128);
    
    float distance = length(lig.position - fragposition);
    float attenuation = min(1.0 / (att.x + att.y * distance +  att.z * (distance * distance)),1.0);    
 
    vec3 ambient  = lig.ambient * material.ambient;
    vec3 diffuse;
    vec3 specular;

        if(specular_texture_isexist)
        {
             specular = lig.specular * spec * vec3(texture(specular_texture,Final_TexCoord));
        }
        else
        {
             specular= lig.specular * spec * material.specular;   
        }

    if(diffuse_texture_isexist)
    {
        diffuse = lig.diffuse * diff * vec3(texture(diffuse_texture, Final_TexCoord));
    }
    else
    {
        diffuse  = lig.diffuse * diff * material.diffuse;
    }

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    //light target
    float alpha = dot(-lightDir,normalize(lig.direction)); 
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
    
    diffuse *= Spotlighteffect;
    specular *= Spotlighteffect;

    return (ambient + diffuse + specular);
}

void UV_GPU_Mode()
{
 
        vec3 vertex;
        vec3 absvertex;

        float u =0;
        float v =0;
        float theta = 0;
        float z = 0;
        if(UV_Entity_Normal == true)
        {
            vertex = normalize(PureNormal);
        }
        else
        {
            vertex = PurePos;
        }

        absvertex = abs(vertex);

        switch(UV_mapping_mode)
        {
            //Cylindical
            case 0:
                theta = degrees(atan(vertex.z,vertex.x));
                  theta +=180.f;


                z = (vertex.y + 1.0f) *0.5f;

                u = theta /360;
                v = z;

                Final_TexCoord = vec2(u,v);
                
            break;
            //Sphereical
            case 1:
                theta = degrees(atan(vertex.z, vertex.x));
                theta +=180.f;
               
                z = vertex.y;

                float phi = degrees(acos(z/sqrt(vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z)));

                u = theta/360.f;
                v = 1- (phi /180.f);
                Final_TexCoord = vec2(u,v);
            break;    
            case 2:
                if (absvertex.x >= absvertex.y && absvertex.x >= absvertex.z)
            	{
                    if(vertex.x < 0)
                    {
                		Final_TexCoord.x = vertex.z/absvertex.x;
                    }
                	else
                    {
                		Final_TexCoord.x = -vertex.z/absvertex.x;
                    }
                	Final_TexCoord.y = vertex.y/absvertex.x;
	            }
	            if (absvertex.y >= absvertex.x && absvertex.y >= absvertex.z)
	            {
	                if(vertex.y < 0)
                    {
	                	Final_TexCoord.x = vertex.x/absvertex.y;
                    }
	                else
                    {
	                	Final_TexCoord.x = -vertex.x/absvertex.y;
                    }
	                Final_TexCoord.y = vertex.z/absvertex.y;
	            }
	            if (absvertex.z >= absvertex.y && absvertex.z >= absvertex.x)
	            {
	                if(vertex.z < 0) 
                    {
	                	Final_TexCoord.x = -vertex.x/absvertex.z;
                    }
	                else
                    {
	                	Final_TexCoord.x = vertex.x/absvertex.z;
                    }
	                Final_TexCoord.y = vertex.y/absvertex.z;
	            }
                Final_TexCoord= (Final_TexCoord+vec2(1,1)) * 0.5f;
            break;
        }
    
 


}


void main()
{
    

    vec3 result = vec3(0.0);

    // Final_TexCoord = TexCoord;
    if(gpu_calculation == true)
    {
       UV_GPU_Mode();
    }
    else
    {
        Final_TexCoord = TexCoord;
    }
    vec3 viewDirection = normalize(CamPos-FragPosition);

    for(int i =0; i < NumOfLight; ++i)
    {
        if(f_light[i].LightType == 0)
        {
            result += DirLight(f_light[i],Normal,FragPosition,viewDirection);
        }
        else if(f_light[i].LightType == 1)
        {
            result += PointLight(f_light[i],Normal,FragPosition,viewDirection, attenuation);
        }
        else if(f_light[i].LightType == 2)
        {
            result += SpotLight(f_light[i],Normal,FragPosition,viewDirection,attenuation);
        }
    }

    result += globalAmbientColor *material.ambient + material.emissive;
    float FogResult = (FogDistanceFar-length(-FragPosition))/(FogDistanceFar-FogDistanceNear);
    result = FogResult*result +(1-FogResult)*FogColor;
    result.x = min(result.x , 1.0);
    result.y = min(result.y ,1.0);
    result.z = min(result.z ,1.0);

    FragColor = vec4(result, 1.0);
}

