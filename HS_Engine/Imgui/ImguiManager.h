/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : ImguiManager.h
Purpose : Imgui Manager Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#pragma once
#include "../Engine/Window.h"
#include "../Engine/SceneManager.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/vec3.hpp>

namespace HS_Engine
{
	class SceneManager;

	class ImguiManager {
	public:
		ImguiManager() = default;
		explicit ImguiManager(SceneManager* a_StateManager, GLFWwindow* a_window);

		void ImguiInit();
		void ImguiUpdate(double dt);
		void ImguiShutdown();

	private:
		SceneManager* m_SceneManger = nullptr;

		float scale1 =1.f;
		GLFWwindow* m_window = nullptr;
		glm::vec3 backcolor = { 1.0f,1.0f,1.0f };
		bool IscashDirty = false;
		ImGuiIO m_io;
		ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	};
}
