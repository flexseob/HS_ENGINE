#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "MaterialManager.h"
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
		~ObjectManager();
		void AddObject(Object* object);
		void DeleteObject(std::string objname);
		void DeleteObjectAll();
		void RenderAll(double dt);
		Object* GetObject(std::string objname);
		//MeshType is Precedual
		void HelperPrecedualMesh(Object* object);
		MeshManager& GetMeshManager() { return mMeshManager; }
		MaterialManager& GetMaterialManager() { return mMaterialManager; }
		void GUIViewer();
		glm::mat4 m_projection;
		glm::mat4 m_view;
	
	private:
		std::unordered_map<std::string,Object*> mObjects;
		MeshManager mMeshManager;
		MaterialManager mMaterialManager;
	};



	
}
