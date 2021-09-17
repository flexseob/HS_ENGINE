#pragma once
#include <string>

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
	};

}
