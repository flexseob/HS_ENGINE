/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#include <iostream>

#include "Engine/Engine.h"
#include "Scene/Demo_DynamicMapping.h"
#include "Scene/Demo_HybridRendering.h"
HS_Engine::Camera* HS_Engine::Scene::m_Camera = new HS_Engine::Camera{ {0.f,1.57f,11.65f } };
int main ()
{
	HS_Engine::Engine& engine = HS_Engine::Engine::Instance();
	
	Demo_DynamicMapping* assginment3 = new Demo_DynamicMapping();
	Demo_HybridRendering* demoHybrid = new Demo_HybridRendering();

	try
	{
		engine.Init();
		engine.Instance().GetSceneManger().AddScene(demoHybrid);
		engine.Update();
		engine.Unload();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}

	
	return 0;
	
}