#include "MaterialManager.h"

#include <iostream>

namespace HS_Engine
{
	MaterialManager::MaterialManager()
	{
		AddMaterial("emerald", new Material({ 0.0215f,	0.1745f,	0.0215f }, { 0.07568f,	0.61424f,	0.07568f }, { 0.633f,	0.727811f,	0.633f }, 0.6f));
		AddMaterial("jade", new Material({ 0.135f,	0.2225f,	0.1575f }, { 0.54f,	0.89f,	0.63f }, { 0.316228f,	0.316228f,	0.316228f }, 0.1f));
		AddMaterial("obsidian", new Material({ 0.05375f,	0.05f,	0.06625f }, { 0.18275f,	0.17f,	0.22525f }, { 0.332741f,	0.328634f,	0.346435f }, 0.3f));
		AddMaterial("pearl", new Material({ 0.25f,	0.20725f,	0.20725f }, { 1.f,	0.829f,	0.829f }, { 0.296648f,	0.296648f,	0.296648f }, 0.088f));
		AddMaterial("ruby", new Material({ 0.1745f,	0.01175f,	0.01175f }, { 0.61424f,	0.04136f,	0.04136f }, { 0.727811f,	0.626959f,	0.626959f }, 0.6f));
		AddMaterial("bronze", new Material({ 0.2125f,	0.1275f,	0.054f }, { 0.714f,	0.4284f,	0.18144f }, { 0.393548f,	0.271906f,	0.166721f }, 0.2f));
		AddMaterial("gold", new Material({ 0.24725f,	0.1995f,	0.0745f }, { 0.75164f,	0.60648f,	0.22648f }, { 0.628281f,	0.555802f,	0.366065f }, 0.4f));
		AddMaterial("silver", new Material({ 0.19225f,	0.19225f,	0.19225f }, { 0.50754f,	0.50754f,	0.50754f }, { 0.508273f,	0.508273f,	0.508273f }, 0.4f));
		AddMaterial("chrome", new Material({ 0.25f,	0.25f,	0.25f }, { 0.4f,	0.4f,	0.4f }, { 0.774597f,	0.774597f,	0.774597f }, 0.6f));

	}

	void MaterialManager::AddMaterial(std::string materialname, Material* material)
	{
		auto findMaterial = m_Materials.find(materialname);

		if(findMaterial == m_Materials.end())
		{
			m_Materials.insert({ materialname, material });
		}
		else
		{
			std::cout << "Material name is Exist! : " << materialname << std::endl;
		}

		
	}

	void MaterialManager::DeleteMaterial(std::string materialname)
	{
		auto findMaterial = m_Materials.find(materialname);
		if(findMaterial != m_Materials.end())
		{
			Material* material = findMaterial->second;
			delete material;
			material = nullptr;
			m_Materials.erase(findMaterial);
			std::cout << "Material is deleted : " << materialname << std::endl;
		}
		else
		{
			std::cout << "Material name is not Exist! : " << materialname << std::endl;
		}
		
	}

	Material* MaterialManager::GetMaterial(std::string materialname) const
	{
		auto findMaterial = m_Materials.find(materialname);

		if (findMaterial != m_Materials.end())
		{
			return findMaterial->second;
		}
		else
		{
			std::cout << "Cannot find the object : " << materialname << std::endl;
			return nullptr;
		}
	}

	void MaterialManager::DeleteAllMaterial()
	{
		for (auto& material : m_Materials)
		{
			Material* objptr = material.second;
			delete objptr;
			material.second = nullptr;
		}

		m_Materials.clear();
	}

	MaterialManager::~MaterialManager()
	{
		DeleteAllMaterial();
	}
}
