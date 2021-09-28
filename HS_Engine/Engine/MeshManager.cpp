#include "MeshManager.h"
#include "Mesh/PreceduralMesh.hpp"
#include <iostream>

namespace HS_Engine
{
	Mesh* MeshManager::AddMesh(std::string meshname, std::string path, E_RenderTypes render_types)
	{
		auto mesh_iter = mMeshdata.find(meshname);

		if(mesh_iter == mMeshdata.end())
		{
			Mesh* new_mesh = m_ObjLoader.Load_raw_ptr(path, true);
			new_mesh->SetRenderType(render_types);
			new_mesh->InitBuffer();
			mMeshdata.insert({ meshname,new_mesh });
			m_ObjLoader.ClearObjectData();
			return new_mesh;
		}
		
		return mesh_iter->second;
	}

	Mesh* MeshManager::AddMesh(std::string meshname, E_Preceduralmesh preceduralmesh,E_RenderTypes render_types)
	{
		auto mesh_iter = mMeshdata.find(meshname);

		if (mesh_iter == mMeshdata.end())
		{
			Mesh* new_mesh = nullptr;
			switch(preceduralmesh)
			{
			case E_Preceduralmesh::SPHERE:
				new_mesh = CreateSphere(1.f, 10);	
				break;
			case E_Preceduralmesh::CUBE:
				new_mesh = CreateCube(1, 1);
				break;
			case E_Preceduralmesh::CYLINDER:
				new_mesh = CreateCylinder(1, 1);
				break;
			case E_Preceduralmesh::PLANE:
				new_mesh = CreatePlane(1, 1);
				break;
			case E_Preceduralmesh::CIRCLE:
				new_mesh = CreateCircle(100);
				break;
			}
			
			new_mesh->SetRenderType(render_types);
			dynamic_cast<PreceduralMesh*>(new_mesh)->InitBuffer();
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
