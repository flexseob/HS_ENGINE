/* Start Header -------------------------------------------------------
Copyright (C) <2021> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Material.h
Purpose: Material class for object
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_1>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <09/11/21>
End Header --------------------------------------------------------*/
#pragma once

#include "Texture.h"
#include "../Engine/Mesh/ProceduralMesh.h"

namespace HS_Engine
{
	class Material
	{
	public:
		Material() = default;
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 emissive, std::string materialname = "Custom")
			:m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess), m_emissive(emissive), m_MaterialName(materialname) {}
		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::string materialname = "Custom")
			:m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess), m_MaterialName(materialname)
		{
			m_emissive = glm::vec3{ 0.f };
		}
		Material(const Material& material);
		Material& operator=(const Material& material);
		glm::vec3 GetAmbient() const;
		glm::vec3 Getdiffuse() const;
		glm::vec3 GetSpecular() const;
		glm::vec3 GetEmissive() const;
		float GetShinness() const;
		std::string GetMaterialName() const;

		void SetDiffuseTexture(Texture* texture);
		void SetSpecularTexture(Texture* texture);
		bool IsExistDiffuseTexture() const;
		bool IsExistSpecularTexture() const;

		bool GetIsNormalUV() const { return m_IsNormal_UV; }
		void SetIsNormalUV(bool isnormal) { m_IsNormal_UV = isnormal; }

		Texture* GetDiffuseTexture();
		Texture* GetSpecularTexture();
	
	private:
		glm::vec3 m_ambient{1.f};
		glm::vec3 m_diffuse{ 1.f };
		glm::vec3 m_specular{ 1.f };
		glm::vec3 m_emissive{ 10.f };
		float m_shininess{ 1.f };
		std::string m_MaterialName = "Custom";
		Texture* m_diffuseTexture = nullptr;
		Texture* m_specularTexture = nullptr;

		bool m_IsNormal_UV = false;
	};

}
