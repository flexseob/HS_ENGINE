/* Start Header------------------------------------------------------ -
Hoseob Jeong
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