/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : SceneManager.h
Purpose : Scene Manager Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#pragma once
#include <vector>
#include <memory>
#include "../Engine/Scene.h"
namespace HS_Engine
{
	class Engine;

	class SceneManager
	{
	public:
		SceneManager();
		explicit SceneManager(Engine* a_engine);
		void AddScene(Scene* scene);
		void SetNextScene(int initScene);
		void Shutdown();
		void UnloadAll();
		void ReloadScene();
		bool HasAllSceneEnded() { return m_Scene == e_Scene::EXIT; }
		bool IsCurrentSceneRunning() { return m_Scene == e_Scene::RUNNING; }
		Scene* GetCurrentScene();
		void Update(double dt);

	private:
		enum class e_Scene
		{
			START,
			LOAD,
			RUNNING,
			UNLOAD,
			SHUTDOWN,
			EXIT,
		};
		Engine* m_engine = nullptr;
		std::vector<Scene*> m_Scenes;
		e_Scene m_Scene =e_Scene::START;
		Scene* m_CurrentScene = nullptr;
		Scene* m_NextScene =nullptr;
		
		bool is_loaded = false;
	};
}