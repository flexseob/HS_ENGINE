#pragma once
#include "Material.h"

namespace HS_Engine
{
	class MaterialManager
	{
	public:
		MaterialManager();
		void AddMaterial(std::string materialname, Material* material);
		void DeleteMaterial(std::string materialname);
		Material* GetMaterial(std::string materialname) const;
		void DeleteAllMaterial();
		~MaterialManager();


	
	private:
		std::unordered_map<std::string, Material*> m_Materials;
		
	};
	
}