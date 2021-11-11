/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Engine.h
Purpose : Engine Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

#pragma once
#include <chrono>
#include <memory>
#include "Window.h"
#include "DemoState.h"
#include "ObjectManager.h"
#include "../Imgui/ImguiManager.h"
#include "../Engine/SceneManager.h"

namespace HS_Engine
{
	class Engine
	{
	public:
		Engine();

		double deltatime = 0;
		std::chrono::time_point<std::chrono::high_resolution_clock> pastTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> nowTime;

		static bool mIsDemoEnded;
	
		double elapsedTime = 0.f;

		void Init();
		void Update();
		void Unload();
		void CleanUp();
		void SetFPS(float FPS);
		bool IsDemoEnded();

		static void MakeDemoEnded()
		{
			mIsDemoEnded = true;
		}
	
	private:
		static std::unique_ptr<Window> m_Window;
		ImguiManager m_ImguiManager;
		SceneManager m_SceneManager;
		ObjectManager m_ObjectManager;
		float m_FPS = 60;
	public:
		static Engine& Instance() { static Engine Instance; return Instance; }
		static Window* GetWindow()
		{
			return m_Window.get();
		}
		static ImguiManager& GetImguiManger()
		{
			return Instance().m_ImguiManager;
		}
		static SceneManager& GetSceneManger()
		{
			return Instance().m_SceneManager;
		}

		static ObjectManager& GetObjectManager()
		{
			return Instance().m_ObjectManager;
		}
	};
}
