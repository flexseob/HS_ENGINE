
#version 460 core
out vec4 FragColor;


in vec3 PurePos;

uniform sampler2D SkyBox_back;
uniform sampler2D SkyBox_bottom;
uniform sampler2D SkyBox_front;
uniform sampler2D SkyBox_left;
uniform sampler2D SkyBox_right;
uniform sampler2D SkyBox_top;

vec2 Final_TexCoord;

void main()
{   
    vec3 absvertex = abs(PurePos);
    vec3 vertex = PurePos;


                 if (absvertex.x >= absvertex.y && absvertex.x >= absvertex.z)
            	{
                    if(vertex.x < 0)
                    {
                		Final_TexCoord.x = vertex.z/absvertex.x;
                        Final_TexCoord.y = -vertex.y/absvertex.x;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        FragColor = texture(SkyBox_left,Final_TexCoord);
                    }
                	else
                    {
                		Final_TexCoord.x = -vertex.z/absvertex.x;
                        Final_TexCoord.y = -vertex.y/absvertex.x;  
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        FragColor = texture(SkyBox_right,Final_TexCoord);
                    } 
	            }
	            if (absvertex.y >= absvertex.x && absvertex.y >= absvertex.z)
	            {
	                if(vertex.y < 0)
                    {
	                	Final_TexCoord.x = vertex.x/absvertex.y;
                        Final_TexCoord.y = -vertex.z/absvertex.y;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        FragColor = texture(SkyBox_bottom,Final_TexCoord);
                    }
	                else
                    {
	                	Final_TexCoord.x = vertex.x/absvertex.y;
                        Final_TexCoord.y = vertex.z/absvertex.y;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        FragColor = texture(SkyBox_top,Final_TexCoord);
                    }
	            }
	            if (absvertex.z >= absvertex.y && absvertex.z >= absvertex.x)
	            {
	                if(vertex.z < 0) 
                    {
	                	Final_TexCoord.x = -vertex.x/absvertex.z;
                        Final_TexCoord.y = -vertex.y/absvertex.z;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        FragColor = texture(SkyBox_back,Final_TexCoord);
                    }
	                else
                    {
	                	Final_TexCoord.x = vertex.x/absvertex.z;
                        Final_TexCoord.y = -vertex.y/absvertex.z;
                        Final_TexCoord= (Final_TexCoord + vec2(1,1)) * 0.5f;
                        FragColor = texture(SkyBox_front,Final_TexCoord);
                    }  
	            }

}