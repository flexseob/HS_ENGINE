

#include "Material.h"

#include <fstream>
#include <sstream>
#include <glm/vec2.hpp>

#include "../Engine/Core/Vector2d.h"

namespace HS_Engine
{
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
}
