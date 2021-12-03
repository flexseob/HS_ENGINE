/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Scene.h
Purpose : Scene Base Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
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
