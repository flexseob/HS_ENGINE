/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
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
