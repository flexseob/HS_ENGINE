/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */


#include "Context.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void HS_Engine::Context::SetupOpenGLVersion(int major, int minor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

HS_Engine::Context::Context(GLFWwindow* glfwwindow) : m_window(glfwwindow) {}

void HS_Engine::Context::Init()
{
	glfwMakeContextCurrent(m_window);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Failed to init GLEW" << std::endl;
	}
	std::cout << "OpenGL Info :" << std::endl;
	std::cout << "OpenGL Version :" << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL Renderer:" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
}

void HS_Engine::Context::SwapBuffer()
{
	glfwSwapBuffers(m_window);
}