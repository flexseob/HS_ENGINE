/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#include "Engine/Engine.h"
#include "Scene/Demo_DynamicMapping.h"
int main ()
{
	HS_Engine::Engine& engine = HS_Engine::Engine::Instance();
	
	Demo_DynamicMapping* assginment3 = new Demo_DynamicMapping();


	try
	{
		engine.Init();
		engine.Instance().GetSceneManger().AddScene(assginment3);
		engine.Update();
		engine.Unload();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}

	
	return 0;
	
}