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

	Texture* Material::GetDiffuseTexture() 
	{
		return m_diffuseTexture;
	}

	Texture* Material::GetSpecularTexture() 
	{
		return m_specularTexture;
	}
}
