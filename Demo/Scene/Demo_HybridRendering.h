/* Start Header -------------------------------------------------------
Copyright (C) <2022> DigiPen Institute of Technology.
/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#pragma once

#include <memory>

#include "Engine/Scene.h"
#include "Engine/Shader.h"
#include "Engine/Core/Camera.h"
#include "Engine/ObjectManager.h"


namespace HS_Engine {
	class Window;
}

class Demo_HybridRendering : public HS_Engine::Scene
{
public:
	void Load() override;
	void Update(double dt) override;
	void UnLoad() override;
	void ImGuiUpdate() override;
	void processInput(HS_Engine::Window* window, double dt);
	std::string GetSceneName() override { return "Demo_HybridRendering"; }
private:


	std::shared_ptr<HS_Engine::Shader> m_Shader_Debug;
	std::shared_ptr<HS_Engine::Shader> m_Shader_LightObjectShader;

	std::shared_ptr<HS_Engine::Shader> m_Shader_DeferredGeometry;
	std::shared_ptr<HS_Engine::Shader> m_Shader_DeferredLight;

	std::shared_ptr<HS_Engine::FrameBuffer> m_Frame_Buffer_0;

	std::shared_ptr<HS_Engine::VertexArray> m_VertexArray_Quad;


	HS_Engine::LightManager mLightManager;


	float near_plane = 1.0f, far_plane = 25.5f;


	float fov = 45.f;

	glm::vec3 m_Attenuation = { 0.01f ,1.14f,0.02f };
	glm::vec3 m_globalAmbientColor = { 0.f,0.f,0.f };
	glm::vec3 m_FogColor = { 1.f,1.f,1.f };

	glm::vec2 m_FogDistance = { 0.1f,100.f };

	glm::vec3 m_DebugColor = { 1.f,1.f,1.f };

	E_Current_Shader m_CurrentShader = E_Current_Shader::PHONG_SHADING;

	bool m_bCopyDepthInfo = true;
};
