/* Start Header -------------------------------------------------------
Copyright (C) <2021> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Context.h
Purpose: Context For Setup and Swap the Buffer and info
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_1>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <09/11/21>
End Header --------------------------------------------------------*/
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace  HS_Engine
{
	class Context
	{
	public:
		static void SetupOpenGLVersion(int major, int minor);
		Context(GLFWwindow* glfwwindow);
		void Init();
		void SwapBuffer();

	private:
		GLFWwindow* m_window;
	};

}
