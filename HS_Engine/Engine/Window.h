/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Window.h
Purpose : For setting window and init the window
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

#pragma once
#include <memory>
#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Context.h"

namespace HS_Engine
{
	static bool GLFW_IsInit = false;
	
	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowProperties(const std::string& title = "HS_Engine", uint32_t width = 1600, uint32_t height = 900) : Title(title), Width(width), Height(height) {}
	};

	class Window
	{
		struct WindowData;
	public:
		Window() = delete;
		Window(const WindowProperties& window_properties)
		{
			// make unique pointer
			Init(window_properties);
		}

		WindowData GetWindowData() { return m_window_data; }
		unsigned int GetWidth() { return m_window_data.m_width; }
		unsigned int GetHeight() { return m_window_data.m_height; }
		void Update();
		void Shutdown();

	private:
		void Init(const WindowProperties& window_properties);

		struct WindowData
		{
			GLFWwindow* m_window =nullptr;
			std::string m_title;
			unsigned int m_width = std::numeric_limits<unsigned int>::max();
			unsigned int m_height = std::numeric_limits<unsigned int>::max();
			std::shared_ptr<Context> m_RenderContextData;
		};
		WindowData m_window_data;
	};


	
}
