/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/

#include "Engine.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Render.h"

namespace HS_Engine
{
	bool Engine::mIsDemoEnded = false;
	std::unique_ptr<Window> Engine::m_Window = std::make_unique<Window>(WindowProperties("CS350 Project"));
	Engine::Engine()	:	m_SceneManager(this), m_ImguiManager(&m_SceneManager, m_Window.get()->GetWindowData().m_window)
	{

	}

	void Engine::Init()
	{
		std::cout << "HS(HoSeob) Engine Init" <<  std::endl;
		Render::Init();
		Render::SetClearColor({ 1.f,1.f,0.f,1.f });
		m_ImguiManager.ImguiInit();
	}



	void Engine::Update()
	{
		while (!IsDemoEnded())
		{
			nowTime = std::chrono::high_resolution_clock::now();
			const std::chrono::duration deltaTick = nowTime - pastTime;
			deltatime = std::chrono::duration<double>(deltaTick).count();

		/*	if (deltatime > 0.1)
			{
				deltatime = 0.033;
			}
			*/
			
			if(1/m_FPS <= deltatime)
			{
				m_Window->Update();
				pastTime = nowTime;
				m_SceneManager.Update(deltatime);
				if(m_SceneManager.IsCurrentSceneRunning())
				m_ImguiManager.ImguiUpdate(deltatime);
			}
		}
		
	}

	void Engine::Unload()
	{
		m_ImguiManager.ImguiShutdown();
		m_SceneManager.Shutdown();
		Render::Shutdown();
		m_Window->Shutdown();
		std::cout << "HS(HoSeob) Engine Shutdown" << std::endl;
	}

	void Engine::CleanUp()
	{
		
	}

	void Engine::SetFPS(float FPS)
	{
		m_FPS = FPS;
	}

	bool Engine::IsDemoEnded()
	{
		return mIsDemoEnded;
	}
}
