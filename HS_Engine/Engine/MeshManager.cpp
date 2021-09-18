#include "MeshManager.h"

#include <iostream>

namespace HS_Engine
{
	Mesh* MeshManager::AddMesh(std::string meshname, std::string path)
	{
		auto mesh_iter = mMeshdata.find(meshname);

		if(mesh_iter == mMeshdata.end())
		{
			Mesh* new_mesh = m_ObjLoader.Load_raw_ptr(path, true);
			new_mesh->InitBuffer();
			mMeshdata.insert({ meshname,new_mesh });
			return new_mesh;
		}
		
		return mesh_iter->second;
		
	}

	void MeshManager::DeleteMesh(std::string meshname)
	{
		auto mesh_iter = mMeshdata.find(meshname);
		if(mesh_iter == mMeshdata.end())
		{
			std::cout << "Cannot delete Mesh data! It doesn't not exist! : " << meshname << std::endl;
		}
	}

	void MeshManager::DeleteAllMesh()
	{
		for(auto& mesh : mMeshdata)
		{
			delete mesh.second;
			mesh.second = nullptr;
		}
		
		mMeshdata.clear();
	}

	Mesh* MeshManager::GetMesh(std::string meshname)
	{
		auto mesh_iter = mMeshdata.find(meshname);
		if(mesh_iter == mMeshdata.end())
		{
			std::cout << "Cannot Find the Mesh! : " << meshname << std::endl;
			return nullptr;
		}
		
		return mesh_iter->second;
	}
}
