
#pragma once
#include <memory>

#include <glm/vec3.hpp>
#include "Shader.h"
#include "VertexArrayData.h"


#include "../Engine/Mesh/PreceduralMesh.hpp"

namespace HS_Engine
{
	class Material
	{
	public:
		Material() = default;
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
			:m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {}
		Material(const Material& material);
		glm::vec3 GetAmbient() const;
		glm::vec3 Getdiffuse() const;
		glm::vec3 GetSpecular() const;
		float GetShinness() const;

	private:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;
		
	};

}
