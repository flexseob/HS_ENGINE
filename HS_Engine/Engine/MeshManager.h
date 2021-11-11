/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : MeshManager.h
Purpose : MeshManager Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
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
