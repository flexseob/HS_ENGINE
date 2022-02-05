/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#pragma once

#include <string>
#include <unordered_map>

#include "OBJLoader.h"
#include "Types.h"
namespace HS_Engine
{
	class MeshManager
	{
	public:
		MeshManager() = default;
		Mesh* AddMesh(std::string meshname,std::string path, E_RenderTypes render_types);
		Mesh* AddMesh(std::string meshname, E_Proceduralmesh preceduralmesh, E_RenderTypes render_types);
		void DeleteMesh(std::string meshname);
		void DeleteAllMesh();
		Mesh* GetMesh(std::string meshname);
		std::unordered_map<std::string, Mesh*>& GetMeshes() { return mMeshdata; };


	private:
		ObjectLoader m_ObjLoader;
		std::unordered_map<std::string, Mesh*> mMeshdata;
	};


	
}
