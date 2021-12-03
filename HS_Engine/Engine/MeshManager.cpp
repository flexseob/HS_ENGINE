/* Start Header-------------------------------------------------------
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : MeshManager.cpp
Purpose : MeshManager class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#include "MeshManager.h"
#include "Mesh/ProceduralMesh.h"
#include <iostream>

namespace HS_Engine
{
	Mesh* MeshManager::AddMesh(std::string meshname, std::string path, E_RenderTypes render_types)
	{
		auto mesh_iter = mMeshdata.find(meshname);

		if(mesh_iter == mMeshdata.end())
		{
			Mesh* new_mesh = m_ObjLoader.Load_raw_ptr(path);
			new_mesh->SetRenderType(render_types);
			new_mesh->InitBuffer();
			new_mesh->SetMeshName(meshname);
			new_mesh->SetMeshType(E_MeshTypes::BY_PATHMESH);
			mMeshdata.insert({ meshname,new_mesh });
			m_ObjLoader.ClearObjectData();
			return new_mesh;
		}
		
		return mesh_iter->second;
	}

	Mesh* MeshManager::AddMesh(std::string meshname, E_Proceduralmesh preceduralmesh,E_RenderTypes render_types)
	{
		auto mesh_iter = mMeshdata.find(meshname);

		if (mesh_iter == mMeshdata.end())
		{
			Mesh* new_mesh = nullptr;
			switch(preceduralmesh)
			{
			case E_Proceduralmesh::SPHERE:
				new_mesh = CreateSphere(1.f, 20);
				break;
			case E_Proceduralmesh::CUBE:
				//TODO : need to Implement
				break;
			case E_Proceduralmesh::CYLINDER:
				//TODO : need to Implement
				break;
			case E_Proceduralmesh::PLANE:
				//TODO : need to Implement
				break;
			case E_Proceduralmesh::CIRCLE:
				new_mesh = CreateCircle(100);
				break;
			}
			new_mesh->SetMeshType(E_MeshTypes::BY_PROCEDURALMESH);
			new_mesh->SetMeshName(meshname);
			new_mesh->SetRenderType(render_types);
			dynamic_cast<ProceduralMesh*>(new_mesh)->InitBuffer();
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
		else
		{
			delete mesh_iter->second;
			mesh_iter->second = nullptr;
			mMeshdata.erase(mesh_iter);
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
