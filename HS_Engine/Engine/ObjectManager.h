/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : ObjectManager.h
Purpose : ObjectManager Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "LightManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "TextureManager.h"

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
		void DeleteObjectQueue();
		void DeleteObjectAll();
		void RenderAll(double dt);
		void ChangeAllObjectShader(std::shared_ptr<Shader> shader);
		Object* GetObject(std::string objname);
		//MeshType is Precedual
		void HelperPrecedualMesh(Object* object);
		MeshManager& GetMeshManager() { return mMeshManager; }
		MaterialManager& GetMaterialManager() { return mMaterialManager; }
		TextureManager& GetTextureManager() { return mTextureManager; }
		void GUIViewer();
		void GUILightViewer();
		glm::mat4 m_projection;
		glm::mat4 m_view;

	
	private:
		std::unordered_map<std::string,Object*> mObjects;
		std::queue<Object*> mNeedtoDeleteObjects;
		MeshManager mMeshManager;
		MaterialManager mMaterialManager;
		TextureManager mTextureManager;

		int m_Light_count;
	};



	
}
