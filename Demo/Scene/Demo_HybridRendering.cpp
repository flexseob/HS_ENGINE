/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */

#include "Demo_HybridRendering.h"

#include "Engine/Engine.h"
#include "Engine/Light.h"
#include "Engine/Render.h"
#include "Engine/Object.h"


constexpr unsigned int MaxLight = 500;
void Demo_HybridRendering::Load()
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
	auto& mTextureManager = mObjectManager.GetTextureManager();

	m_Camera->ResetCamera();
	m_Camera->Position = glm::vec3{ 0.f,1.57f,11.65f };
	m_Shader_DeferredGeometry = std::make_shared<HS_Engine::Shader>("Common/shaders/GeometryStageDeferred.vert", "Common/shaders/GeometryStageDeferred.frag");
	m_Shader_DeferredLight = std::make_shared<HS_Engine::Shader>("Common/shaders/LightStageDeferred.vert", "Common/shaders/LightStageDeferred.frag");

	m_Shader_DeferredLight->LinkShader();
	m_Shader_DeferredGeometry->LinkShader();
	m_Shader_DeferredGeometry->SetIsDebugShader(true);



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


	m_Shader_DeferredGeometry->FindUniformLocation("view");
	m_Shader_DeferredGeometry->FindUniformLocation("projection");
	m_Shader_DeferredGeometry->FindUniformLocation("model");

	m_Shader_DeferredLight->FindUniformLocation("attenuation");
	m_Shader_DeferredLight->FindUniformLocation("globalAmbientColor");
	m_Shader_DeferredLight->FindUniformLocation("FogColor");
	m_Shader_DeferredLight->FindUniformLocation("FogDistanceFar");
	m_Shader_DeferredLight->FindUniformLocation("FogDistanceNear");
	m_Shader_DeferredLight->FindUniformLocation("CamPos");


	m_Shader_Debug->FindUniformLocation("view");
	m_Shader_Debug->FindUniformLocation("model");
	m_Shader_Debug->FindUniformLocation("projection");
	m_Shader_Debug->FindUniformLocation("DebugColor");



	mTextureManager.AddTexture("diffuse_texture", "Common/images/metal_roof_diff_512x512.png", 0x0);
	mTextureManager.AddTexture("specular_texture", "Common/images/metal_roof_spec_512x512.png", 0x1);


	mTextureManager.AddTexture("viewPosBuffer", new HS_Engine::Texture(0x0, 1600, 900, GL_RGB32F));
	mTextureManager.AddTexture("normalBuffer", new HS_Engine::Texture(0x1, 1600, 900, GL_RGB32F));
	mTextureManager.AddTexture("uvBuffer", new HS_Engine::Texture(0x2, 1600, 900, GL_RGB32F));
	mTextureManager.AddTexture("depthBuffer", new HS_Engine::Texture(0x3, 1600, 900, GL_RGB32F));
	mTextureManager.AddTexture("diffuseBuffer", new HS_Engine::Texture(0x4, 1600, 900, GL_RGB32F));
	mTextureManager.AddTexture("specularBuffer", new HS_Engine::Texture(0x5, 1600, 900, GL_RGB32F));
	mTextureManager.AddTexture("ambientBuffer", new HS_Engine::Texture(0x6, 1600, 900, GL_RGBA));
	mTextureManager.AddTexture("emissiveBuffer", new HS_Engine::Texture(0x7, 1600, 900, GL_RGB32F));

	m_Shader_DeferredLight->FindUniformLocation("viewPosBuffer");
	m_Shader_DeferredLight->FindUniformLocation("normalBuffer");
	m_Shader_DeferredLight->FindUniformLocation("diffuseBuffer");
	m_Shader_DeferredLight->FindUniformLocation("specularBuffer");
	m_Shader_DeferredLight->FindUniformLocation("ambientBuffer");
	m_Shader_DeferredLight->FindUniformLocation("emissiveBuffer");

	HS_Engine::Object* object = new HS_Engine::Object("bunny", "bunny", "Common/models/bunny.obj", E_MeshTypes::BY_PATHMESH, E_RenderTypes::TRIANGLES);
	object->SetShader(m_Shader_DeferredGeometry);
	object->SetShaderForDebug(m_Shader_Debug);
	object->SetPosition({ 0.f, 0.f, 0.f });
	object->SetMaterial({ mObjectManager.GetMaterialManager().GetMaterial("ruby") });
	object->SetMaterialDiffuseTexture(mObjectManager.GetTextureManager().GetTexture("diffuse_texture"));
	object->SetMaterialSpecularTexture(mObjectManager.GetTextureManager().GetTexture("specular_texture"));
	object->Init();
	mObjectManager.AddObject(object);
	mLightManager.SetShader(m_Shader_DeferredGeometry);


	for (int i = 1; i < 9; i++)
	{
		std::string sphereObject = "Light" + std::to_string(i);
		HS_Engine::Light* light = new HS_Engine::Light(sphereObject, "SphereProcedural", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::SPHERE, E_RenderTypes::TRIANGLES);
		light->SetShader(m_Shader_LightObjectShader);
		light->SetShaderForDebug(m_Shader_Debug);
		light->SetMaterial(HS_Engine::Material());
		light->Init();
		light->SetPosition({ 1.f, 0.f, 1.f });
		light->SetScale({ 0.1f, 0.1f, 0.1f });
		light->SetPostRenderFunction([&, sphereObject, i, light](double dt)
			{
				glm::vec3 newPosition = { 3.f * cos(+0.8f * static_cast<float>(i) + light->count),0.f,3.f * sin(+0.8f * static_cast<float>(i) + light->count) };
				light->SetPosition(newPosition);
				light->GetLightData()->lightPosition = newPosition;
				light->IsCacheDirty = true;
				light->count += static_cast<float>(dt);

			});
		mLightManager.AddLight(light);
	}

	object = new HS_Engine::Object("orbit", "orbit", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::CIRCLE, E_RenderTypes::LINES);
	object->SetShader(m_Shader_DeferredGeometry);
	object->SetShaderForDebug(m_Shader_Debug);
	object->Init();
	object->SetPosition({ 0.f, 0.f, 0.f });
	object->SetScale({ 3.f,3.f,3.f });
	object->SetRotate(90.f, { 1.f,0.f,0.f });
	mObjectManager.AddObject(object);


	unsigned lightuniformidx = m_Shader_DeferredLight->FindUniformBlockIndex("light");
	mLightManager.SetInit(MaxLight, lightuniformidx);

	mObjectManager.GetMeshManager().AddMesh("Lucy", "Common/models/lucy_princeton.obj", E_RenderTypes::TRIANGLES);
	mObjectManager.GetMeshManager().AddMesh("cubeOBJ", "Common/models/cube.obj", E_RenderTypes::TRIANGLES);
	mObjectManager.GetMeshManager().AddMesh("cubeOBJ2", "Common/models/cube2.obj", E_RenderTypes::TRIANGLES);
	mObjectManager.GetMeshManager().AddMesh("M4A1", "Common/models/M4A1.obj", E_RenderTypes::TRIANGLES);


	object = new HS_Engine::Object("plane", "planeobj", "Common/models/triangle.obj", E_MeshTypes::BY_PATHMESH, E_RenderTypes::TRIANGLES);
	object->SetShader(m_Shader_DeferredGeometry);
	object->SetShaderForDebug(m_Shader_Debug);
	object->SetPosition({ 0.f, -1.f, 0.f });
	object->SetScale({ 5.f,1.f,5.f });
	object->SetRotate(270.f, { 1.f,0.f,0.f });
	object->SetMaterial(mObjectManager.GetMaterialManager().GetMaterial("silver"));
	object->Init();
	mObjectManager.AddObject(object);

	HS_Engine::Engine::Instance().SetFPS(144.f);
	HS_Engine::Render::SetClearColor({ 0.f,0.f,0.f,1.0f });


	m_Frame_Buffer_0 = std::make_shared<HS_Engine::FrameBuffer>(1600, 900, GL_DRAW_FRAMEBUFFER);
	m_Frame_Buffer_0->UnBind();



	//for Render using a Full-screen Quad/////////////////////////////////
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	m_VertexArray_Quad = std::make_shared<HS_Engine::VertexArray>();
	std::shared_ptr<HS_Engine::VertexBuffer> QuadVertexBuffer = std::make_shared<HS_Engine::VertexBuffer>(quadVertices, (unsigned int)sizeof(quadVertices));
	QuadVertexBuffer->DescribeData({ {HS_Engine::DataType::Float3,0},{HS_Engine::DataType::Float2,1} });

	m_VertexArray_Quad->AddVertexBuffer(QuadVertexBuffer);

	std::shared_ptr<HS_Engine::IndexBuffer> QuadIndexBuffer = std::make_shared<HS_Engine::IndexBuffer>(4);
	m_VertexArray_Quad->AddIndexBuffer(QuadIndexBuffer);

}

void Demo_HybridRendering::Update(double dt)
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
	HS_Engine::Render::ClearScreen();

	glm::mat4 projection;
	glm::mat4 view;
	int width, height;

	mLightManager.DeleteLightQueue();
	mObjectManager.DeleteObjectQueue();
	mLightManager.UpdateLight();
	mLightManager.LightUniformBufferUpdate();

	glfwGetWindowSize(HS_Engine::Engine::GetWindow()->GetWindowData().m_window, &width, &height);
	projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)width / (float)height, 0.1f, 1000.f);
	view = m_Camera->GetCameraMat();

	mObjectManager.ChangeAllObjectShader(m_Shader_DeferredGeometry);

	/// GeometryPass
	mObjectManager.DeferredShading([&]()
		{
			HS_Engine::Render::Init();
			HS_Engine::Render::ClearScreen();
			glm::mat4 projection;
			glm::mat4 view;
			int width, height;

			glfwGetWindowSize(HS_Engine::Engine::GetWindow()->GetWindowData().m_window, &width, &height);
			projection = glm::perspective(glm::radians(m_Camera->Zoom), (float)1600 / (float)900, 0.1f, 1000.f);
			view = m_Camera->GetCameraMat();

			m_Shader_DeferredGeometry->Bind();
			m_Shader_DeferredGeometry->BindUniformVariable("view", view);
			m_Shader_DeferredGeometry->BindUniformVariable("projection", projection);
			m_Shader_DeferredGeometry->BindUniformVariable("CamPos", m_Camera->Position);
			m_Shader_DeferredGeometry->Unbind();

			m_Shader_Debug->Bind();
			m_Shader_Debug->BindUniformVariable("view", view);
			m_Shader_Debug->BindUniformVariable("projection", projection);
			m_Shader_Debug->BindUniformVariable("DebugColor", m_DebugColor);
			m_Shader_Debug->Unbind();

			mObjectManager.m_projection = projection;
			mObjectManager.m_view = view;

			mObjectManager.RenderAll(dt);


		}, m_Frame_Buffer_0);



	//Light pass//////////////////
	HS_Engine::Render::Init();
	HS_Engine::Render::ClearScreen();
	m_Shader_DeferredLight->Bind();
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("viewPosBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("viewPosBuffer", 0);
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("normalBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("normalBuffer", 1);
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("uvBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("uvBuffer", 2);
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("depthBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("depthBuffer", 3);
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("diffuseBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("diffuseBuffer", 4);
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("specularBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("specularBuffer", 5);
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("ambientBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("ambientBuffer", 6);
	HS_Engine::Engine::GetObjectManager().GetTextureManager().GetTexture("emissiveBuffer")->Bind();
	m_Shader_DeferredLight->BindUniformVariable("emissiveBuffer", 7);



	m_Shader_DeferredLight->FindUniformLocation("NumOfLight");
	m_Shader_DeferredLight->BindUniformVariable("NumOfLight", mLightManager.GetNumberOfLight());
	m_Shader_DeferredLight->BindUniformVariable("CamPos", m_Camera->Position);
	m_Shader_DeferredLight->BindUniformVariable("globalAmbientColor", m_globalAmbientColor);
	m_Shader_DeferredLight->BindUniformVariable("FogDistanceNear", m_FogDistance.x);
	m_Shader_DeferredLight->BindUniformVariable("FogDistanceFar", m_FogDistance.y);
	m_Shader_DeferredLight->BindUniformVariable("FogColor", m_FogColor);
	m_Shader_DeferredLight->BindUniformVariable("attenuation", m_Attenuation);

	m_VertexArray_Quad->Bind();
	HS_Engine::Render::DrawTriangleStrip(m_VertexArray_Quad);
	m_VertexArray_Quad->Unbind();
	m_Shader_DeferredLight->Unbind();

	//depth copy/////
	if (m_bCopyDepthInfo == true)
	{
		m_Frame_Buffer_0->BindReadBuffer();
		m_Frame_Buffer_0->BindDrawBuffer();
		m_Frame_Buffer_0->BlitFrameBuffer();
		m_Frame_Buffer_0->UnBind();
	}


	m_Shader_LightObjectShader->Bind();
	m_Shader_LightObjectShader->BindUniformVariable("projection", projection);
	m_Shader_LightObjectShader->BindUniformVariable("view", view);
	m_Shader_LightObjectShader->Unbind();

	mObjectManager.RenderDebugDraw(dt);

	mLightManager.SetShader(m_Shader_LightObjectShader);
	mLightManager.RenderAll(dt);



	processInput(HS_Engine::Engine::GetWindow(), dt);


}

void Demo_HybridRendering::UnLoad()
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
	mObjectManager.DeleteObjectAll();
	mLightManager.DeleteAllLight();
	m_Shader_DeferredGeometry.reset();
	m_Shader_Debug.reset();
	m_Shader_LightObjectShader.reset();
	delete m_Camera;
}

static std::string Current_Shader = "Current Shader : LigthPassShader";

void Demo_HybridRendering::ImGuiUpdate()
{
	HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();

	ImGui::TextColored({ 1.f,0.f,0.f,1.f }, Current_Shader.c_str());

	if (ImGui::Button("Reload Shader"))
	{
		printf("--Reload Shader--\n");
		m_Shader_DeferredGeometry->ReloadShader();
		m_Shader_DeferredLight->ReloadShader();
	}

	ImGui::ColorEdit3("DebugColor", reinterpret_cast<float*>(&m_DebugColor));
	ImGui::DragFloat3("CamPosition", reinterpret_cast<float*>(&m_Camera->Position), 0.01f, -1000.000f, 1000.000f, "%.2f", 1);
	ImGui::DragFloat3("attenuation", reinterpret_cast<float*>(&m_Attenuation), 0.01f, 0.000f, 20.000f, "%.2f", 1);
	ImGui::ColorEdit3("GlobalAmbientColor", reinterpret_cast<float*>(&m_globalAmbientColor), 1);

	ImGui::NewLine();
	ImGui::Text("Fog");
	ImGui::DragFloat2("FogDistance", reinterpret_cast<float*>(&m_FogDistance), 0.01f, 0.000f, 1000.000f, "%.2f", 1);
	ImGui::ColorEdit3("FogColor", reinterpret_cast<float*>(&m_FogColor), 1);
	ImGui::Checkbox("bCopyDepthInfo", &m_bCopyDepthInfo);


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

	ImGui::Begin("Debugview");
	{

		auto& mTextureManager = HS_Engine::Engine::GetObjectManager().GetTextureManager();
		ImGui::SetCursorPos(ImVec2(0, 0));
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("viewPosBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("normalBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("uvBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("depthBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("diffuseBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("emissiveBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("ambientBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::SameLine();
		ImGui::Image((void*)(intptr_t)mTextureManager.GetTexture("specularBuffer")->GetTextureID(), ImVec2(200, 200), ImVec2(-1, 1), ImVec2(0, 0));
		ImGui::SameLine();
	}
	ImGui::End();

}

void Demo_HybridRendering::processInput(HS_Engine::Window* window, double dt)
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
	if (glfwGetKey(glfwwindow, GLFW_KEY_Y) == GLFW_PRESS)
	{
		m_Camera->MouseInput(0, offset);
	}
	if (glfwGetKey(glfwwindow, GLFW_KEY_H) == GLFW_PRESS)
	{
		m_Camera->MouseInput(0, -offset);
	}
	if (glfwGetKey(glfwwindow, GLFW_KEY_G) == GLFW_PRESS)
	{
		m_Camera->MouseInput(-offset, 0);
	}
	if (glfwGetKey(glfwwindow, GLFW_KEY_J) == GLFW_PRESS)
	{
		m_Camera->MouseInput(offset, 0);
	}
}
