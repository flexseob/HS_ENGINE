
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

	float Material::GetShinness() const
	{
		return m_shininess;
	}

	std::string Material::GetMaterialName() const
	{
		return m_MaterialName;
	}
}
