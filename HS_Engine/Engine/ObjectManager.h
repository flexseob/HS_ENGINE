#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "MeshManager.h"

namespace HS_Engine
{
	class Object;
	enum E_MeshType
	{
		PATH, Precedural,
	};
	
	class ObjectManager
	{
	public:
		ObjectManager() = default;
		void AddObject(Object* object);
		void DeleteObject(std::string objname);
		void DeleteObjectAll();
	
		Object* GetObject(std::string objname);	
		

	private:
		std::unordered_map<std::string,Object*> mObjects;
		MeshManager mMeshManager;
		
	};



	
}
