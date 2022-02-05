/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#pragma once
#include <string>

#include "Core/Camera.h"

namespace HS_Engine
{
	class Scene
	{
	public:
		virtual void Load() = 0;
		virtual void Update(double dt) = 0;
		virtual void UnLoad() = 0;
		virtual void ImGuiUpdate() = 0;
		virtual std::string GetSceneName() { return "Default Sence"; }
		virtual ~Scene() = default;
	protected:
		static HS_Engine::Camera* m_Camera;
	};

}
