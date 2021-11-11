/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : MaterialManager.h
Purpose :MaterialManager Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
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
		std::unordered_map<std::string, Material*>& GetMaterials() { return m_Materials; };
		void DeleteAllMaterial();
		~MaterialManager();


	
	private:
		std::unordered_map<std::string, Material*> m_Materials;
		
	};
	
}