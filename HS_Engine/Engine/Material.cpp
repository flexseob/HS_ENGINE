/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Material.cpp
Purpose : Material class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

#include "Material.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/vec2.hpp>



namespace HS_Engine
{
	Material::Material(const Material& material)
	{
		m_ambient = material.m_ambient;
		m_diffuse = material.m_diffuse;
		m_specular = material.m_specular;
		m_shininess = material.m_shininess;
		m_emissive = material.m_emissive;
		m_MaterialName = material.m_MaterialName;

	}

	Material& Material::operator=(const Material& material)
	{
		if (this == &material)
		{
			return *this;
		}
		m_ambient = material.m_ambient;
		m_diffuse = material.m_diffuse;
		m_specular = material.m_specular;
		m_shininess = material.m_shininess;
		m_emissive = material.m_emissive;
		m_MaterialName = material.m_MaterialName;
		return *this;
	}

	glm::vec3 Material::GetAmbient() const
	{
		return m_ambient;
	}

	glm::vec3 Material::Getdiffuse() const
	{
		return m_diffuse;
	}

	glm::vec3 Material::GetSpecular() const
	{
		return m_specular;
	}

	glm::vec3 Material::GetEmissive() const
	{
		return m_emissive;
	}

	float Material::GetShinness() const
	{
		return m_shininess;
	}

	std::string Material::GetMaterialName() const
	{
		return m_MaterialName;
	}

	void Material::SetDiffuseTexture(Texture* texture)
	{
		m_diffuseTexture = texture;
	}

	void Material::SetSpecularTexture(Texture* texture)
	{
		m_specularTexture = texture;
	}

	bool Material::IsExistDiffuseTexture() const
	{
		return m_diffuseTexture;
	}

	bool Material::IsExistSpecularTexture() const
	{
		return m_specularTexture;
	}

	void Material::AddCubeMappingTexture(std::initializer_list<CUBEMAP_INFO> mapping)
	{
		for(auto info : mapping)
		{
			std::string name;
			switch(info.Position)
			{
			case E_CUBE_MAP::TOP:
				name = "TopFrame";
				break;
			case E_CUBE_MAP::LEFT: 
				name = "LeftFrame";
				break;
			case E_CUBE_MAP::FRONT: 
				name = "FrontFrame";
				break;
			case E_CUBE_MAP::RIGHT:
				name = "RightFrame";
				break;
			case E_CUBE_MAP::BACK:
				name = "BackFrame";
				break;
			case E_CUBE_MAP::BOTTOM: 
				name = "BottomFrame";
				break;
			default: break;
			}
			auto iter = m_MappingTexture.find(name);
			if (iter == m_MappingTexture.end())
				m_MappingTexture.insert({ name, info.texture });
			else
			{
				//std::cout << "Existing mapping texture " + name << " it was erased old one, Now add new one" << std::endl;
				m_MappingTexture.erase(name);
				m_MappingTexture.insert({ name, info.texture });
			}
		}		
	}

	Texture* Material::GetDiffuseTexture() 
	{
		return m_diffuseTexture;
	}

	Texture* Material::GetSpecularTexture() 
	{
		return m_specularTexture;
	}
}
