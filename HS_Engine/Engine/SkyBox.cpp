/* Start Header -------------------------------------------------------
Copyright (C) <current year in format YYYY> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SkyBox.cpp
Purpose: SkyBox object
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_3>
class CS300 and this file is a part of assignment 3, then write: foo_CS300_3>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <11/11/21>
End Header --------------------------------------------------------*/
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

