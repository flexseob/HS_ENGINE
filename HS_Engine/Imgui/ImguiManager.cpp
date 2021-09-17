

#include "ImguiManager.h"
#include <algorithm>
#include <string.h>

#include "../Engine/Engine.h"


namespace HS_Engine
{
	class Demo2DNoise;

	ImguiManager::ImguiManager(SceneManager* a_StateManager, GLFWwindow* a_window)
		: m_SceneManger(a_StateManager), m_window(a_window) {}

	void ImguiManager::ImguiInit()
	{
		ImGui::CreateContext();
		m_io = ImGui::GetIO();
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void ImguiManager::ImguiUpdate(double dt)
	{
		if (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{
				ImGui::Begin("Menu");
				{
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					if(Scene* CurrScene = Engine::GetSceneManger().GetCurrentScene();CurrScene != nullptr)
					{
						CurrScene->ImGuiUpdate();
					}
					ImGui::NewLine();
					
				}
				ImGui::End();
				ImGui::Begin("good");
				{
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					if (Scene* CurrScene = Engine::GetSceneManger().GetCurrentScene(); CurrScene != nullptr)
					{
						CurrScene->ImGuiUpdate();
					}
					ImGui::NewLine();
			
				}
				ImGui::End();
			}

			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(m_window, &display_w, &display_h);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		}
	}


	void HS_Engine::ImguiManager::ImguiShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}
