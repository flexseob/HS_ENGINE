/* Start Header -------------------------------------------------------
Copyright (C) <current year in format YYYY> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment2.cpp
Purpose: Assignment2 Scene
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_2>
class CS300 and this file is a part of assignment 2, then write: foo_CS300_2>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <10/11/21>
End Header --------------------------------------------------------*/
#include "Assignment2.h"
#include "Engine/Engine.h"
#include "Engine/Light.h"
#include "Engine/Render.h"
#include "Engine/Object.h"
#include "Engine/SkyBox.h"

//HS_Engine::Camera Assginment2_Scene::m_Camera{ {0.f,1.57f,11.65f }};
constexpr unsigned int MaxLight = 16;
void Assginment2_Scene::Load()
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
	auto& mTextureManager = mObjectManager.GetTextureManager();
	m_Camera = new HS_Engine::Camera( glm::vec3{ 0.f,1.57f,11.65f } );

	m_Shader_PhongShading = std::make_shared<HS_Engine::Shader>("Common/shaders/PhongShader.vert", "Common/shaders/PhongShader.frag");
	m_Shader_PhongLight = std::make_shared<HS_Engine::Shader>("Common/shaders/PhongLightingShader.vert", "Common/shaders/PhongLightingShader.frag");
	m_Shader_Blinn = std::make_shared<HS_Engine::Shader>("Common/shaders/BlinnShader.vert", "Common/shaders/BlinnShader.frag");
	m_Shader_SkyBox = std::make_shared<HS_Engine::Shader>("Common/shaders/SkyBoxShader.vert", "Common/shaders/SkyBoxShader.frag");

	m_Shader_PhongLight->LinkShader();
	m_Shader_PhongShading->LinkShader();
	m_Shader_Blinn->LinkShader();
	m_Shader_SkyBox->LinkShader();

	
	m_Shaders.push_back(m_Shader_PhongShading);
	m_Shaders.push_back(m_Shader_PhongLight);
	m_Shaders.push_back(m_Shader_Blinn);

	m_Shader_Debug = std::make_shared<HS_Engine::Shader>("Common/shaders/defaultDebug.vert", "Common/shaders/defaultDebug.frag");
	m_Shader_Debug->LinkShader();


	m_Shader_LightObjectShader = std::make_shared<HS_Engine::Shader>("Common/shaders/LightDiffuse.vert", "Common/shaders/LightDiffuse.frag");
	m_Shader_LightObjectShader->LinkShader();

	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BACK);

	
	
	mLightManager.SetShader(m_Shader_LightObjectShader);
	
	m_Shader_LightObjectShader->FindUniformLocation("view");
	m_Shader_LightObjectShader->FindUniformLocation("projection");
	m_Shader_LightObjectShader->FindUniformLocation("model");

	for (auto& shader : m_Shaders)
	{
		shader->FindUniformLocation("view");
		shader->FindUniformLocation("projection");

		shader->FindUniformLocation("lightPos");
		shader->FindUniformLocation("model");


		shader->FindUniformLocation("attenuation");
		shader->FindUniformLocation("globalAmbientColor");
		shader->FindUniformLocation("FogColor");
		shader->FindUniformLocation("FogDistanceFar");
		shader->FindUniformLocation("FogDistanceNear"); 
		shader->FindUniformLocation("CamPos");
	}
	
	m_Shader_Debug->FindUniformLocation("view");
	m_Shader_Debug->FindUniformLocation("model");
	m_Shader_Debug->FindUniformLocation("projection");
	m_Shader_Debug->FindUniformLocation("DebugColor");

	
	
	mTextureManager.AddTexture("diffuse_texture", "Common/images/metal_roof_diff_512x512.png", 0x0);
	mTextureManager.AddTexture("specular_texture", "Common/images/metal_roof_spec_512x512.png", 0x1);
	mTextureManager.AddTextureSkyBox("SkyBox_back", "Common/images/skybox/back.jpg", 0x0);
	mTextureManager.AddTextureSkyBox("SkyBox_bottom", "Common/images/skybox/bottom.jpg", 0x1);
	mTextureManager.AddTextureSkyBox("SkyBox_front", "Common/images/skybox/front.jpg", 0x2);
	mTextureManager.AddTextureSkyBox("SkyBox_left", "Common/images/skybox/left.jpg", 0x3);
	mTextureManager.AddTextureSkyBox("SkyBox_right", "Common/images/skybox/right.jpg", 0x4);
	mTextureManager.AddTextureSkyBox("SkyBox_top", "Common/images/skybox/top.jpg", 0x5);
	

	//HS_Engine::SkyBox* skybox = new HS_Engine::SkyBox("skybox", "skybox", "Common/models/cube2.obj", E_MeshTypes::BY_PATHMESH, E_RenderTypes::TRIANGLES, 
	//	{ {E_CUBE_MAP::BACK,mTextureManager.GetTexture("SkyBox_back") },
	//		{ E_CUBE_MAP::BOTTOM,mTextureManager.GetTexture("SkyBox_bottom") },
	//	{E_CUBE_MAP::FRONT,mTextureManager.GetTexture("SkyBox_front")},
	//	{E_CUBE_MAP::LEFT,mTextureManager.GetTexture("SkyBox_left")},
	//	{E_CUBE_MAP::RIGHT,mTextureManager.GetTexture("SkyBox_right")},
	//	{E_CUBE_MAP::TOP,mTextureManager.GetTexture("SkyBox_top")}});
	//
	//skybox->SetShader(m_Shader_SkyBox);
	//skybox->SetShaderForDebug(m_Shader_Debug);
	//mObjectManager.AddObject(skybox);

	
	HS_Engine::Object* object = new HS_Engine::Object("bunny", "bunny", "Common/models/bunny.obj", E_MeshTypes::BY_PATHMESH, E_RenderTypes::TRIANGLES);
	object->SetShader(m_Shader_PhongShading);
	object->SetShaderForDebug(m_Shader_Debug);
	object->SetPosition({ 0.f, 0.f, 0.f });
	object->SetMaterial({ mObjectManager.GetMaterialManager().GetMaterial("ruby") });
	object->SetMaterialDiffuseTexture(mObjectManager.GetTextureManager().GetTexture("diffuse_texture"));
	object->SetMaterialSpecularTexture(mObjectManager.GetTextureManager().GetTexture("specular_texture"));
	object->Init();
	mObjectManager.AddObject(object);



	for (int i = 1; i <9; i++)
	{
		std::string sphereObject = "Light" + std::to_string(i);
		HS_Engine::Light * light = new HS_Engine::Light(sphereObject, "SphereProcedural", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::SPHERE, E_RenderTypes::TRIANGLES);
		light->SetShader(m_Shader_LightObjectShader);
		light->SetShaderForDebug(m_Shader_Debug);
		light->SetMaterial(HS_Engine::Material());
		light->Init();
		light->SetPosition({ 1.f, 0.f, 1.f });
		light->SetScale({ 0.1f, 0.1f, 0.1f });
		light->SetPostRenderFunction([&, sphereObject, i, light](double dt)
			{
				glm::vec3 newPosition = { 3.f * cos(+0.8f * static_cast<float>(i) + light->count),0.f,3.f * sin(+0.8f * static_cast<float>(i)+ light->count) };
				light->SetPosition(newPosition);
				light->GetLightData()->lightPosition = newPosition;
				light->IsCacheDirty = true;
				light->count += static_cast<float>(dt);
			
			});
		mLightManager.AddLight(light);
	}

	object = new HS_Engine::Object("orbit", "orbit", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::CIRCLE, E_RenderTypes::LINES);
	object->SetShader(m_Shader_PhongShading);
	object->SetShaderForDebug(m_Shader_Debug);
	object->Init();
	object->SetPosition({ 0.f, 0.f, 0.f });
	object->SetScale({ 3.f,3.f,3.f });
	object->SetRotate(90.f, { 1.f,0.f,0.f });
	mObjectManager.AddObject(object);

	
	unsigned lightuniformidx = m_Shader_PhongShading->FindUniformBlockIndex("light");
	mLightManager.SetInit(MaxLight, lightuniformidx);
	
	mObjectManager.GetMeshManager().AddMesh("Lucy", "Common/models/lucy_princeton.obj", E_RenderTypes::TRIANGLES);
	mObjectManager.GetMeshManager().AddMesh("cubeOBJ", "Common/models/cube.obj", E_RenderTypes::TRIANGLES);
	mObjectManager.GetMeshManager().AddMesh("cubeOBJ2", "Common/models/cube2.obj", E_RenderTypes::TRIANGLES);

	object = new HS_Engine::Object("plane", "planeobj", "Common/models/triangle.obj", E_MeshTypes::BY_PATHMESH, E_RenderTypes::TRIANGLES);
	object->SetShader(m_Shader_PhongShading);
	object->SetShaderForDebug(m_Shader_Debug);
	object->SetPosition({ 0.f, -1.f, 0.f });
	object->SetScale({ 5.f,1.f,5.f });
	object->SetRotate(270.f, { 1.f,0.f,0.f });
	object->SetMaterial(mObjectManager.GetMaterialManager().GetMaterial("silver"));
	object->Init();
	mObjectManager.AddObject(object);

	HS_Engine::Engine::Instance().SetFPS(165.f);
	HS_Engine::Render::SetClearColor({ 0.f,0.f,0.f,1.0f });
}

void Assginment2_Scene::Update(double dt)
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
	
	HS_Engine::Render::ClearScreen();
	

	glm::mat4 projection;
	glm::mat4 view;
	int width, height;

	mLightManager.DeleteLightQueue();
	mObjectManager.DeleteObjectQueue();
	mLightManager.UpdateLight();
	
	glfwGetWindowSize(HS_Engine::Engine::GetWindow()->GetWindowData().m_window, &width, &height);
	projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)width / (float)height, 0.1f, 1000.f);
	view = m_Camera->GetCameraMat();

	const int current = static_cast<int>(m_CurrentShader);
	mLightManager.LightUniformBufferUpdate();
	m_Shaders[current]->Bind();
	m_Shaders[current]->BindUniformVariable("lightPos", m_LightPosition);
	m_Shaders[current]->BindUniformVariable("view", view);
	m_Shaders[current]->BindUniformVariable("projection", projection);


	 
	m_Shaders[current]->FindUniformLocation("NumOfLight");
	m_Shaders[current]->BindUniformVariable("NumOfLight", mLightManager.GetNumberOfLight());
	m_Shaders[current]->BindUniformVariable("CamPos", m_Camera->Position);

	m_Shaders[current]->BindUniformVariable("globalAmbientColor",m_globalAmbientColor);
	
	m_Shaders[current]->BindUniformVariable("FogDistanceNear",m_FogDistance.x);
	m_Shaders[current]->BindUniformVariable("FogDistanceFar",m_FogDistance.y);
	m_Shaders[current]->BindUniformVariable("FogColor",m_FogColor);
	
	m_Shaders[current]->BindUniformVariable("attenuation", m_Attenuation);
	
	m_Shaders[current]->Unbind();

	m_Shader_LightObjectShader->Bind();
	m_Shader_LightObjectShader->BindUniformVariable("projection", projection);
	m_Shader_LightObjectShader->BindUniformVariable("view", view);
	m_Shader_LightObjectShader->Unbind();

	
	m_Shader_Debug->Bind();
	m_Shader_Debug->BindUniformVariable("view", view);
	m_Shader_Debug->BindUniformVariable("projection", projection);
	m_Shader_Debug->BindUniformVariable("DebugColor", m_DebugColor);
	m_Shader_Debug->Unbind();

	mObjectManager.m_projection = projection;
	mObjectManager.m_view = view;

	mLightManager.RenderAll(dt);
	mObjectManager.RenderAll(dt);

	
	processInput(HS_Engine::Engine::GetWindow(), dt);


}

void Assginment2_Scene::UnLoad()
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
	mObjectManager.DeleteObjectAll();
	mLightManager.DeleteAllLight();
	m_Shader_PhongShading.reset();
	m_Shader_PhongLight.reset();
	m_Shader_Blinn.reset();
	m_Shader_SkyBox.reset();

	m_Shader_Debug.reset();
	m_Shader_LightObjectShader.reset();
	for (auto& shader : m_Shaders)
	{
		shader.reset();
	}
	m_Shaders.clear();
	delete m_Camera;
}

static std::string Current_Shader = "Current Shader : PhongShader";

void Assginment2_Scene::ImGuiUpdate()
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();

	ImGui::TextColored({ 1.f,0.f,0.f,1.f }, Current_Shader.c_str());
	
	if(ImGui::Button("Reload Shader"))
	{
		m_Shader_PhongShading->ReloadShader();
		m_Shader_PhongLight->ReloadShader();
		m_Shader_Blinn->ReloadShader();
		m_Shader_SkyBox->ReloadShader();
	}

	if(ImGui::Button("PhongShader") && m_CurrentShader != E_Current_Shader::PHONG_SHADING)
	{
		m_CurrentShader = E_Current_Shader::PHONG_SHADING;
		mObjectManager.ChangeAllObjectShader(m_Shader_PhongShading);
		Current_Shader = "Current Shader : PhongShader";
	}
	ImGui::SameLine();
	if (ImGui::Button("PhongLightingShader") && m_CurrentShader != E_Current_Shader::PHONG_LIGHTING)
	{
		m_CurrentShader = E_Current_Shader::PHONG_LIGHTING;
		mObjectManager.ChangeAllObjectShader(m_Shader_PhongLight);
		Current_Shader = "Current Shader : PhongLightingShader";
	}
	ImGui::SameLine();
	if (ImGui::Button("BlinnShader") && m_CurrentShader != E_Current_Shader::BLINN_SHADING)
	{
		m_CurrentShader = E_Current_Shader::BLINN_SHADING;
		mObjectManager.ChangeAllObjectShader(m_Shader_Blinn);
		Current_Shader = "Current Shader : BlinnShader";
	}

	
	ImGui::ColorEdit3("DebugColor", reinterpret_cast<float*>(&m_DebugColor));
	ImGui::DragFloat3("CamPosition", reinterpret_cast<float*>(&m_Camera->Position), 0.01f, -1000.000f, 1000.000f, "%.2f", 1);
	ImGui::DragFloat3("attenuation", reinterpret_cast<float*>(&m_Attenuation), 0.01f, 0.000f, 20.000f, "%.2f", 1);
	ImGui::ColorEdit3("GlobalAmbientColor", reinterpret_cast<float*>(&m_globalAmbientColor), 1);

	ImGui::NewLine();
	ImGui::Text("Fog");
	ImGui::DragFloat2("FogDistance", reinterpret_cast<float*>(&m_FogDistance), 0.01f, 0.000f, 1000.000f, "%.2f", 1);
	ImGui::ColorEdit3("FogColor", reinterpret_cast<float*>(&m_FogColor), 1);
	mLightManager.GUILightScenario();
	ImGui::End();
	ImGui::Begin("ObjectManager");
	{
		mObjectManager.GUIViewer();
		ImGui::NewLine();
	}
	ImGui::End();
	
	ImGui::Begin("LightSetting");
	{
		mLightManager.GUILightViewer();
		ImGui::NewLine();
	}
	ImGui::End();

}

void Assginment2_Scene::processInput(HS_Engine::Window* window, double dt)
{
	GLFWwindow* glfwwindow = window->GetWindowData().m_window;
	float deltatime = static_cast<float>(dt);
		if (glfwGetKey(glfwwindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Camera->KeyboardInput(HS_Engine::Camera_MoveTo::FORWARD, dt);
		}
		if (glfwGetKey(glfwwindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Camera->KeyboardInput(HS_Engine::Camera_MoveTo::BACKWARD, dt);
		}
		if (glfwGetKey(glfwwindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Camera->KeyboardInput(HS_Engine::Camera_MoveTo::LEFT, dt);
		}
		if (glfwGetKey(glfwwindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Camera->KeyboardInput(HS_Engine::Camera_MoveTo::RIGHT, dt);
		}

		glfwSetScrollCallback(glfwwindow, [](GLFWwindow*, double x, double y)
		{
			m_Camera->MouseScrollInput(static_cast<float>(y));
		});
		const float offset = deltatime * 5.f;
		if(glfwGetKey(glfwwindow,GLFW_KEY_Y) == GLFW_PRESS)
		{
			m_Camera->MouseInput(0, offset);
		}
		if (glfwGetKey(glfwwindow, GLFW_KEY_H) == GLFW_PRESS)
		{
			m_Camera->MouseInput(0, -offset);
		}
		if (glfwGetKey(glfwwindow, GLFW_KEY_G) == GLFW_PRESS)
		{
			m_Camera->MouseInput(-offset,0 );
		}
		if (glfwGetKey(glfwwindow, GLFW_KEY_J) == GLFW_PRESS)
		{
			m_Camera->MouseInput(offset, 0);
		}
		if (glfwGetKey(glfwwindow, GLFW_KEY_1) == GLFW_PRESS)
		{
			HS_Engine::Engine::GetSceneManger().SetNextScene(1);
		}
}
