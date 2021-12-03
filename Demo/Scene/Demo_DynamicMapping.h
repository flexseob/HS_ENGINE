/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#pragma once

#include <memory>

#include "Engine/Engine.h"
#include "Engine/Scene.h"
#include "Engine/Shader.h"
#include "Engine/Core/Camera.h"
#include "Engine/ObjectManager.h"
#include "Engine/Types.h"

namespace HS_Engine {
	class Window;
}

class Demo_DynamicMapping : public HS_Engine::Scene
{
public:
	void Load() override;
	void Update(double dt) override;
	void UnLoad() override;
	void ImGuiUpdate() override;
	void processInput(HS_Engine::Window* window, double dt);
	std::string GetSceneName() override { return "Demo_DynamicMapping"; }
private:
	std::shared_ptr<HS_Engine::Shader> m_Shader_PhongShading;
	std::shared_ptr<HS_Engine::Shader> m_Shader_PhongLight;
	std::shared_ptr<HS_Engine::Shader> m_Shader_Blinn;
	std::shared_ptr<HS_Engine::Shader> m_Shader_SkyBox;
	std::shared_ptr<HS_Engine::Shader> m_Shader_FrameBuffer_debug;
	
	std::shared_ptr<HS_Engine::Shader> m_Shader_Debug;
	std::shared_ptr<HS_Engine::Shader> m_Shader_LightObjectShader;

	std::vector< std::shared_ptr<HS_Engine::Shader>> m_Shaders;

	
	std::shared_ptr<HS_Engine::FrameBuffer> m_FrameBuffer;
	HS_Engine::LightManager mLightManager;


	float near_plane = 1.0f, far_plane = 25.5f;
	glm::vec3 m_LightPosition = { -2.0f, 4.0f, 20.0f };
	glm::vec3 m_LightAmbient = { 1.f, 1.f, 1.f };
	glm::vec3 m_LightDiffuse = { 1.f, 1.f, 1.f };
	glm::vec3 m_LightSpecular = { 1.f,1.f,1.f };

	float fov = 45.f;
	glm::vec3 m_LightCenter = glm::vec3{ 1.f };
	glm::vec3 m_LightColor{ 1.f };

	glm::vec3 m_Attenuation = { 1.f ,0.22f,0.2f };
	glm::vec3 m_globalAmbientColor = { 0.f,0.f,0.f };
	glm::vec3 m_FogColor = { 1.f,1.f,1.f };

	glm::vec2 m_FogDistance = { 0.1f,100.f };

	glm::vec3 m_DebugColor = { 1.f,1.f,1.f };

	E_Current_Shader m_CurrentShader = E_Current_Shader::PHONG_SHADING;
};
