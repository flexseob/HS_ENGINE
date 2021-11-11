/* Start Header-------------------------------------------------------
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : SceneManager.cpp
Purpose : SceneManager class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

#include "SceneManager.h"
#include <memory>
#include <GL/glew.h>

#include "Engine.h"
#include "render.h"

namespace HS_Engine
{

	SceneManager::SceneManager() = default;

	SceneManager::SceneManager(Engine* a_engine) : m_engine(a_engine) {}

	void SceneManager::AddScene(Scene* scene)
	{
		m_Scenes.push_back(scene);
	}

	void SceneManager::SetNextScene(int initScene)
	{
		m_NextScene = m_Scenes[initScene];
	}
	void SceneManager::UnloadAll()
	{
		m_Scene = e_Scene::UNLOAD;
		m_NextScene = nullptr;
	}
	
	void SceneManager::Shutdown()
	{
		for(Scene* scene : m_Scenes)
		{
			delete scene;
		}
		m_Scenes.clear();
	}

	void SceneManager::ReloadScene()
	{
		m_Scene = e_Scene::UNLOAD;
	}


	Scene* SceneManager::GetCurrentScene()
	{
		return m_CurrentScene;
	}

	void SceneManager::Update(double dt)
	{
		switch (m_Scene)
		{
			case e_Scene::START:
			{
				SetNextScene(0);
				m_Scene = e_Scene::LOAD;
				break;
			}
		case e_Scene::LOAD:
		{
				m_CurrentScene = m_NextScene;
				std::cout << "CurrentScene : " + m_CurrentScene->GetSceneName() + " is Loading!" << std::endl;
				m_CurrentScene->Load();
				std::cout << "CurrentScene : " + m_CurrentScene->GetSceneName() + " is Loaded!" << std::endl;
				m_Scene = e_Scene::RUNNING;
				break;
		}

		case e_Scene::RUNNING:
		{
			if (m_CurrentScene != m_NextScene) 
			{
				m_Scene = e_Scene::UNLOAD;
			}
			else 
			{
				m_CurrentScene->Update(dt);
			}
			break;
		}

		case e_Scene::UNLOAD:
		{
			std::cout << "CurrentScene : " + m_CurrentScene->GetSceneName() + " is UnLoaded!" << std::endl;
			m_CurrentScene->UnLoad();

			if (m_NextScene == nullptr)
			{
				m_Scene = e_Scene::SHUTDOWN;
			}
			else
			{
				m_Scene = e_Scene::LOAD;
			}
			break;
		}

		case e_Scene::SHUTDOWN:
		{
			m_Scene = e_Scene::EXIT;
			break;
		}

		}
	}


}
