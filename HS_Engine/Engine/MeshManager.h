#pragma once

#include <string>
#include <unordered_map>

#include "OBJLoader.h"

namespace HS_Engine
{
	class MeshManager
	{
	public:
		MeshManager() = default;
		Mesh* AddMesh(std::string meshname,std::string path);
		void DeleteMesh(std::string meshname);
		void DeleteAllMesh();
		Mesh* GetMesh(std::string meshname);
		
		


	private:
		ObjectLoader m_ObjLoader;
		std::unordered_map<std::string, Mesh*> mMeshdata;
	};


	
}
