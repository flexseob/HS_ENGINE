/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#include "SkyBox.h"
#include "Shader.h"

namespace HS_Engine
{

	SkyBox::SkyBox(std::string objectname, std::string meshname, std::string path, E_MeshTypes meshtype, E_RenderTypes rendertype ,const std::initializer_list<CUBEMAP_INFO>& textures) : Object(objectname,  meshname, path,  meshtype,  rendertype)
	{
		m_Textures = textures;
	}

	void SkyBox::Render()
	{
		if(!m_shader)
			return;
		glDepthFunc(GL_LEQUAL);
		m_shader->Bind();
		for(auto& texture : m_Textures)
		{
			Texture* texturePtr = texture.texture;
			texturePtr->Bind();
			int textureIdx = static_cast<int>(texture.texture->GetTextureIdx());
			
			switch(texture.Position)
			{
			case E_CUBE_MAP::BACK:
				m_shader->FindUniformLocation("SkyBox_back");
				m_shader->BindUniformVariable("SkyBox_back", textureIdx);
				break;
			case E_CUBE_MAP::TOP: 
				m_shader->FindUniformLocation("SkyBox_top");
				m_shader->BindUniformVariable("SkyBox_top", textureIdx);
				break;
			case E_CUBE_MAP::LEFT: 
				m_shader->FindUniformLocation("SkyBox_left");
				m_shader->BindUniformVariable("SkyBox_left", textureIdx);
				break;
			case E_CUBE_MAP::FRONT: 
				m_shader->FindUniformLocation("SkyBox_front");
				m_shader->BindUniformVariable("SkyBox_front", textureIdx);
				break;
			case E_CUBE_MAP::RIGHT: 
				m_shader->FindUniformLocation("SkyBox_right");
				m_shader->BindUniformVariable("SkyBox_right", textureIdx);
				break;
			case E_CUBE_MAP::BOTTOM: 
				m_shader->FindUniformLocation("SkyBox_bottom");
				m_shader->BindUniformVariable("SkyBox_bottom", textureIdx);
				break;
			}
		
		}

		m_mesh->Render(m_RenderType);
	
		glDepthFunc(GL_LESS);
	

		m_shader->Unbind();
		
	}

	SkyBox::~SkyBox()
	{
		m_Textures.clear();
	}
}

