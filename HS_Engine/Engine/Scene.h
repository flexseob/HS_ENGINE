/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#pragma once
#include <string>

#include "Core/Camera.h"

namespace HS_Engine
{
	class Scene
	{
	public:
		virtual void Load() {}
		virtual void Update(double dt) {}
		virtual void UnLoad() {}
		virtual void ImGuiUpdate() {}
		virtual std::string GetSceneName() { return "Default Sence"; }
		virtual ~Scene() = default;
	protected:
		static HS_Engine::Camera* m_Camera;
	};

}
