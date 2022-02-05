#version 460 core
out vec4 FragColor;

in vec3 FragPosition;
in vec3 Normal;  
in vec2 TexCoord;

in vec3 PureNormal;
in vec3 PurePos;

// in float Ratio;

in float EtaR;
in float EtaG;
in float EtaB;


uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

uniform bool diffuse_texture_isexist;
uniform bool specular_texture_isexist;

uniform sampler2D TopFrame;
uniform sampler2D BottomFrame;
uniform sampler2D LeftFrame;
uniform sampler2D FrontFrame;
uniform sampler2D BackFrame;
uniform sampler2D RightFrame;

uniform bool isEnvironmentMapping;
uniform float RefractionIdx;
uniform int EnvironmentMode;
uniform bool isPhongshading;

uniform bool gpu_calculation;

uniform bool UV_Entity_Normal;

uniform int UV_mapping_mode;
const float FresnelPower = 5.0;

struct Material
{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
    vec3 emissive;
};
const int MAX_LIGHT = 500;
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
uniform float PhongShadingvalue;
vec2 Final_TexCoord;


vec3 DirLight(Light lig, vec3 normal,vec3 fragposition, vec3 viewDir)
{
    vec3 lightDir = normalize(-lig.direction);
    vec3 norm = normalize(normal);

  
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDirection = 2*dot(norm, lightDir)*norm - lightDir;

     vec3 specular =vec3(0.0);
     
   
        float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess*128);

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
  
    vec3 reflectDir = 2*dot(norm, lightDir)*norm - lightDir;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*128);
    
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

    vec3 reflectDir = 2*dot(norm, lightDir)*norm - lightDir;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*128);
    
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

vec3 RefractionOnly()
{
    float r = 1/RefractionIdx;
    vec3 I = normalize(FragPosition - CamPos);
    float k = 1.0-r*r *(1.0 -dot(Normal,I)* dot(Normal, I));
    vec3 mappingPos = r *I -(r * dot(Normal, I) + sqrt(k)) * Normal;
 
    return mappingPos;
}
vec3 Refraction(float r)
{
    vec3 I = normalize(FragPosition - CamPos);
    float k = 1.0-r*r *(1.0 -dot(Normal,I)* dot(Normal, I));
    vec3 mappingPos;
    mappingPos = r *I -(r * dot(Normal, I) + sqrt(k)) * Normal;
 
    return mappingPos;
}

vec3 Reflection()
{
    vec3 normalDirection = normalize(FragPosition-CamPos);
    vec3 norm = normalize(Normal);
    vec3 mappingPos = 2*dot(normalDirection,norm)*norm - normalDirection;

    return -mappingPos;
}

vec4 EnvironmentTextureSetup(vec3 mappingPos)
{   
    vec4 result;
    vec3 absvertex  = abs(mappingPos);
    vec3 vertex = mappingPos;

                if (absvertex.x >= absvertex.y && absvertex.x >= absvertex.z)
            	{
                    if(vertex.x < 0)
                    {
                		Final_TexCoord.x = -vertex.z/absvertex.x;
                        Final_TexCoord.y = vertex.y/absvertex.x;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        result = texture(LeftFrame,Final_TexCoord);
                    }
                	else
                    {
                		Final_TexCoord.x = vertex.z/absvertex.x;
                        Final_TexCoord.y = vertex.y/absvertex.x;  
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        result = texture(RightFrame,Final_TexCoord);
                    } 
	            }
	            if (absvertex.y >= absvertex.x && absvertex.y >= absvertex.z)
	            {
	                if(vertex.y < 0)
                    {
	                	Final_TexCoord.x = -vertex.x/absvertex.y;
                        Final_TexCoord.y = vertex.z/absvertex.y;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        result = texture(BottomFrame,Final_TexCoord);
                    }
	                else
                    {
	                	Final_TexCoord.x = -vertex.x/absvertex.y;
                        Final_TexCoord.y = -vertex.z/absvertex.y;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        result = texture(TopFrame,Final_TexCoord);
                    }
	            }
	            if (absvertex.z >= absvertex.y && absvertex.z >= absvertex.x)
	            {
	                if(vertex.z < 0) 
                    {
	                	Final_TexCoord.x = vertex.x/absvertex.z;
                        Final_TexCoord.y = vertex.y/absvertex.z;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        result = texture(BackFrame,Final_TexCoord);
                    }
	                else
                    {
	                	Final_TexCoord.x = -vertex.x/absvertex.z;
                        Final_TexCoord.y = vertex.y/absvertex.z;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        result = texture(FrontFrame,Final_TexCoord);
                    }  
	            }

    return result;
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

    if(isEnvironmentMapping == true)
    {       
        vec4 EnvironmentColor;
        if(EnvironmentMode == 0)
        {
            EnvironmentColor = EnvironmentTextureSetup(Reflection());
        }
        else if(EnvironmentMode == 1)
        {
            EnvironmentColor = EnvironmentTextureSetup(RefractionOnly());
        }
        else if(EnvironmentMode == 2)
        {
            vec4 refractcolor, reflectcolor;

           
            
            refractcolor.r = EnvironmentTextureSetup(Refraction(EtaR)).r;
            refractcolor.g = EnvironmentTextureSetup(Refraction(EtaG)).g;
            refractcolor.b = EnvironmentTextureSetup(Refraction(EtaB)).b;

            reflectcolor = EnvironmentTextureSetup(Reflection());
            const float F = ((1.0-EtaG) * (1.0-EtaG)) / ((1.0+EtaG) * (1.0+EtaG));
            float Ratio = F + (1.0 - F) * pow((1.0 - dot(-normalize(FragPosition-CamPos), normalize(Normal))), FresnelPower);
            vec4 mixvalue = mix(refractcolor,reflectcolor,Ratio);
            

            EnvironmentColor = mixvalue;
        }
        if(isPhongshading == true)
        {
            EnvironmentColor = vec4(mix(result, EnvironmentColor.xyz,PhongShadingvalue),1.f); 
        }
          FragColor= EnvironmentColor;
    }
    else
    {
        FragColor = vec4(result, 1.0);
    }

    
}

