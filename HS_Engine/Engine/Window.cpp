
#include "Window.h"
#include "iostream"
#include "Engine.h"
void HS_Engine::Window::Update()
{
	glfwPollEvents();
	m_window_data.m_RenderContextData->SwapBuffer();
	if(glfwWindowShouldClose(m_window_data.m_window))
	{
		Engine::Instance().GetSceneManger().UnloadAll();
		Engine::MakeDemoEnded();
	}
}

void HS_Engine::Window::Init(const WindowProperties& window_properties)
{
	if (!GLFW_IsInit)
	{
		int IsSuccess = glfwInit();
		//set gl version
		if(IsSuccess)
		{
			Context::SetupOpenGLVersion(4, 6);
		}
		else
		{
			Engine::MakeDemoEnded();
		}
		//todo: exit program
		GLFW_IsInit = true;
	}
	
	m_window_data.m_height = window_properties.Height;
	m_window_data.m_width = window_properties.Width;
	m_window_data.m_title = window_properties.Title;
	
	m_window_data.m_window = glfwCreateWindow(m_window_data.m_width, m_window_data.m_height, m_window_data.m_title.c_str(), nullptr, nullptr);
	
	if (m_window_data.m_window == NULL) 
	{
		std::cout << "failed to Create window\n";
	}
	m_window_data.m_RenderContextData = std::make_shared<Context>(m_window_data.m_window);
	m_window_data.m_RenderContextData->Init();

	//to get the glfw callback
	glfwSetWindowUserPointer(m_window_data.m_window, &m_window_data);
}

void HS_Engine::Window::Shutdown()
{
	glfwTerminate();
}

