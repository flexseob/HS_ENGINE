
#pragma once

#include "../Engine/Mesh/ProceduralMesh.h"

namespace HS_Engine
{
	class Material
	{
	public:
		Material() = default;
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess,  std::string materialname = "Custom")
			:m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess), m_MaterialName(materialname) {}
		Material(const Material& material);
		Material& operator=(const Material& material);
		glm::vec3 GetAmbient() const;
		glm::vec3 Getdiffuse() const;
		glm::vec3 GetSpecular() const;
		float GetShinness() const;
		std::string GetMaterialName() const;
	private:
		glm::vec3 m_ambient{1.f};
		glm::vec3 m_diffuse{ 1.f };
		glm::vec3 m_specular{ 1.f };
		float m_shininess{ 1.f };
		std::string m_MaterialName = "Custom";
	};

}
