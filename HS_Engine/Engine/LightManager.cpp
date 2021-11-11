/* Start Header-------------------------------------------------------
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : LightManager.cpp
Purpose : LightManager class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <10 / 11 / 21>
End Header-------------------------------------------------------- */

#include "LightManager.h"
#include "Engine.h"
#include <random>
namespace HS_Engine
{
	LightManager::~LightManager()
	{
		DeleteAllLight();
	}

	void LightManager::AddLight(Light* light)
	{
		HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
		HS_Engine::MeshManager& mMeshManager = mObjectManager.GetMeshManager();
		bool isexist = false;
		for (auto& findlight : m_Lights)
		{
			if (findlight.first == light->GetObjectName())
			{
				isexist = true;
			}
		}
		if (!isexist)
		{
			std::string objectname = light->GetObjectName();
			if (light->GetMesh() == nullptr)
			{
				Mesh* mesh = nullptr;
				switch (light->GetMeshType())
				{
				case E_MeshTypes::BY_PATHMESH:
					mesh = mMeshManager.AddMesh(light->GetMeshName(), light->GetObjPath(), light->GetRenderType());
					mesh->SetRenderType(light->GetRenderType());
					light->SetMesh(mesh);
					m_Lights.push_back(std::make_pair(light->GetObjectName(), light));
					m_LightRawDatas.push_back(light->GetLightDataRef());
					break;

				case E_MeshTypes::BY_PROCEDURALMESH:
					switch (light->GetPreceduralmeshType())
					{
					case E_Proceduralmesh::SPHERE:
						mesh = mMeshManager.AddMesh(light->GetMeshName(), E_Proceduralmesh::SPHERE, light->GetRenderType());
						light->SetMesh(mesh);
						//m_Lights.push_back(std::make_pair(light->GetObjectName(), light));
						break;
					case E_Proceduralmesh::CUBE:
						mesh = mMeshManager.AddMesh(light->GetMeshName(), E_Proceduralmesh::CUBE, light->GetRenderType());
						light->SetMesh(mesh);
						break;
					case E_Proceduralmesh::CYLINDER:
						mesh = mMeshManager.AddMesh(light->GetMeshName(), E_Proceduralmesh::CYLINDER, light->GetRenderType());
						light->SetMesh(mesh);
						break;
					case E_Proceduralmesh::PLANE:
						mesh = mMeshManager.AddMesh(light->GetMeshName(), E_Proceduralmesh::PLANE, light->GetRenderType());
						light->SetMesh(mesh);
						break;
					case E_Proceduralmesh::CIRCLE:
						mesh = mMeshManager.AddMesh(light->GetMeshName(), E_Proceduralmesh::CIRCLE, light->GetRenderType());
						light->SetMesh(mesh);
						break;
					}
					m_Lights.push_back(std::make_pair(light->GetObjectName(), light));
					m_LightRawDatas.push_back(light->GetLightDataRef());

			
					break;
				}

			}
			else
			{
				m_Lights.push_back(std::make_pair(light->GetObjectName(), light));
				m_LightRawDatas.push_back(light->GetLightDataRef());
			}
			m_NumberOfLight++;
		}
		else
		{
			std::cout << "Object name is Exist! : " << light->GetObjectName() << std::endl;
			delete light;
		}
		
	}

	void LightManager::DeleteLight(std::string lightname)
	{
		HS_Engine::ObjectManager& mObjectManager = HS_Engine::Engine::GetObjectManager();
		HS_Engine::MeshManager& mMeshManager = mObjectManager.GetMeshManager();
		for(int idx =0; idx<m_Lights.size(); ++idx)
		{
			auto& findlight = m_Lights.at(idx);
			if(findlight.first == lightname)
			{
				Light* light = findlight.second;
				mNeedtoDeleteLight.push(light);
				
				findlight = std::move(m_Lights.back());
				m_Lights.pop_back();

				m_LightRawDatas[idx] = std::move(m_LightRawDatas.back());
				m_LightRawDatas.pop_back();
				m_NumberOfLight--;
			}
			
		}
	}

	void LightManager::DeleteAllLight()
	{
		for (auto& object : m_Lights)
		{
			Object* objptr = object.second;
			delete objptr;
			object.second = nullptr;
		}

		m_Lights.clear();
		m_LightRawDatas.clear();
		m_NumberOfLight = 0;
	
	}

	void LightManager::DeleteLightQueue()
	{
		while (!mNeedtoDeleteLight.empty())
		{
			Object* obj = mNeedtoDeleteLight.front();
			delete obj;
			obj = nullptr;
			mNeedtoDeleteLight.pop();
		}
	}

	void LightManager::RenderAll(double dt)
	{

		
		for (auto& light : m_Lights)
		{
			Light* obj = light.second;
			obj->PreRender();
			obj->Render();
			obj->PostRender(dt);
		}
		
	}

	void LightManager::UpdateLight()
	{
		int idx = 0;
		for(auto& light : m_Lights)
		{

			m_LightRawDatas[idx] = light.second->GetLightDataRef();

			light.second->GetLightData()->direction.x = -light.second->GetLightData()->lightPosition.x;
			light.second->GetLightData()->direction.z = -light.second->GetLightData()->lightPosition.z;
			light.second->IsCacheDirty = false;
			idx++;
		}
		
	}

	void LightManager::SetShader(std::shared_ptr<Shader> shader)
	{
		m_LightShader = shader;
	}

	void LightManager::ChangeAllLightShader(std::shared_ptr<Shader> shader)
	{
		for (auto& light : m_Lights)
		{
			Light* obj = light.second;
			obj->SetShader(shader);
		}
		
	}

	Light* LightManager::GetLight(std::string name)
	{
		
		for (auto& findlight : m_Lights)
		{
			if (findlight.first == name)
			{
				return findlight.second;
			}
		}
		return nullptr;
	}
	static int lightnum = 1;
	
	void LightManager::GUILightViewer()
	{
		static const char* current_light = nullptr;
	
		bool is_selected = true;


		
		if(ImGui::Button("Add NewLight") && m_Lights.size() <16)
		{
			HS_Engine::Light* object = new HS_Engine::Light("NewLight"+std::to_string(lightnum), "SphereProcedural", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::SPHERE, E_RenderTypes::TRIANGLES);
			
			object->SetShader(m_LightShader);
			object->SetMaterial(HS_Engine::Material());
			object->Init();
			object->SetPosition({ 3.f * cos(glm::radians(360.f / m_Lights.size()) + object->count),0.f,3.f * sin(glm::radians(360.f / m_Lights.size()) + object->count) });
			object->GetLightData()->lightPosition = { 4.f * cos(glm::radians(360.f / m_Lights.size()) + object->count),0.f,4.f * sin(glm::radians(360.f / m_Lights.size()) + object->count) };
			object->IsCacheDirty = true;
			object->SetScale({ 0.1f, 0.1f, 0.1f });
			lightnum++;
			object->SetPostRenderFunction([&, object](double dt)
				{
				
					glm::vec3 newPosition = { 3.f * cos(glm::radians(360.f / m_Lights.size()) + object->count),0.f,3.f * sin(glm::radians(360.f / m_Lights.size()) + object->count) };
					object->SetPosition(newPosition);
					object->GetLightData()->lightPosition = newPosition;
					object->IsCacheDirty = true;
					object->count += static_cast<float>(dt);
				});
			AddLight(object);
			
	
			int idx = 0;
			
			for(auto& light : m_Lights)
			{
				Light* lightptr = light.second;
				lightptr->count = 0;

				glm::vec3 newPosition = { 3.f * cos(glm::radians(360.f / m_Lights.size() * idx) + lightptr->count),0.f,3.f * sin( glm::radians(360.f / m_Lights.size() * idx) + lightptr->count) };
				lightptr->SetPosition(newPosition);
				lightptr->GetLightData()->lightPosition = newPosition;
				lightptr->IsCacheDirty = true;
				lightptr->count =0;
				
				lightptr->SetPostRenderFunction([&, lightptr, idx](double dt)
					{
						glm::vec3 newPosition = { 3.f * cos(glm::radians(360.f /m_Lights.size() * idx) + lightptr->count),0.f,3.f * sin( glm::radians(360.f / m_Lights.size() * idx )+ lightptr->count) };
						lightptr->SetPosition(newPosition);
						lightptr->GetLightData()->lightPosition = newPosition;
						lightptr->IsCacheDirty = true;
						lightptr->count += static_cast<float>(dt);
					});
				idx++;
			}

			if(m_IsAllRotating == false)
			{
				object->IsMoving = false;
			}

			
		}
		
		if(ImGui::Button("Stop Rotating"))
		{
			for(auto& light : m_Lights)
			{
				light.second->IsMoving = false;
			}
			m_IsAllRotating = false;
		}
		ImGui::SameLine();
		if(ImGui::Button("Begin Rotating"))
		{
			for (auto& light : m_Lights)
			{
				light.second->IsMoving = true;
			}
			m_IsAllRotating = true;
		}
		
		if(ImGui::Button("SetAllDirctionalLight"))
		{
			for(auto& lightdata : m_Lights)
			{
				lightdata.second->GetLightData()->lightType = E_LightType::DIRLIGHT;
			}
		}
		if (ImGui::Button("SetAllPointLight"))
		{
			for (auto& lightdata : m_Lights)
			{
				lightdata.second->GetLightData()->lightType = E_LightType::POINTLIGHT;
			}
		}
		if (ImGui::Button("SetAllSpotLight"))
		{
			for (auto& lightdata : m_Lights)
			{
				lightdata.second->GetLightData()->lightType = E_LightType::SPOTLIGHT;
			}
		}
		
		 
		if (ImGui::BeginCombo("Light", current_light))
		{
			for (auto& obj : m_Lights)
			{
					is_selected = (current_light == obj.first.c_str());

					if (ImGui::Selectable(obj.first.c_str(), is_selected))
					{
						current_light = obj.first.c_str();
					}
				
			}


			ImGui::EndCombo();
		}
		if (is_selected)
		{
			if (current_light != nullptr)
			{
				Light* light = GetLight(current_light);
				if (light != nullptr)
				{
					glm::vec3 LightPosition = light->GetPosition();
					bool Is_change_Light = ImGui::DragFloat3("lightPos", reinterpret_cast<float*>(&LightPosition), 0.01f, -1000.000f, 1000.000f, "%.2f", 1);
					light->GetLightData()->lightPosition = LightPosition;
					light->SetPosition(LightPosition);
					
					Is_change_Light |= ImGui::ColorEdit3("lightDiffuse", reinterpret_cast<float*>(&light->GetLightData()->lightDiffuse), 1);
					Is_change_Light |= ImGui::ColorEdit3("lightAmbient", reinterpret_cast<float*>(&light->GetLightData()->lightAmbient), 1);
					Is_change_Light |= ImGui::ColorEdit3("lightSpecular", reinterpret_cast<float*>(&light->GetLightData()->lightSpecular), 1);
					Is_change_Light |= ImGui::DragFloat3("lightDirection", reinterpret_cast<float*>(&light->GetLightData()->direction), 0.01f, -1000.000f, 1000.000f, "%.2f", 1);
					if (light->GetLightData()->lightType == E_LightType::SPOTLIGHT)
					{
						float angle = light->GetLightData()->outcutoff;;
						Is_change_Light |= ImGui::DragFloat("outcutoffangle", &angle, 0.001f, light->GetLightData()->cutoff, 1000.000f, "%.3f", 1);
						light->GetLightData()->outcutoff = angle;
						float inner_angle = light->GetLightData()->cutoff;
						Is_change_Light |= ImGui::DragFloat("innercutoffangle", &inner_angle, 0.001f, 0.000f, light->GetLightData()->outcutoff, "%.3f", 1);;
						light->GetLightData()->cutoff = inner_angle;

						float falloff = light->GetLightData()->falloff;
						Is_change_Light |= ImGui::DragFloat("falloff", &falloff, 1.f, 0.000f, 1000.000f, "%.3f", 1);;
						light->GetLightData()->falloff = falloff;
					}
						Is_change_Light |= ImGui::RadioButton("DirectLight", &light->GetLightData()->lightType, 0);
						ImGui::SameLine();
						Is_change_Light |= ImGui::RadioButton("PointLight", &light->GetLightData()->lightType, 1);
						ImGui::SameLine();
						Is_change_Light |= ImGui::RadioButton("SpotLight", &light->GetLightData()->lightType, 2);
					

				
					if(Is_change_Light)
					{
						light->IsCacheDirty = true;
					}

					if(ImGui::Button("DeleteLight"))
					{
						if(m_Lights.size() != 1)
						DeleteLight(current_light);
						DeleteLightQueue();
						int idx = 0;

						for (auto& light : m_Lights)
						{
							Light* lightptr = light.second;
							lightptr->SetPostRenderFunction([&, lightptr, idx](double dt)
								{
									glm::vec3 newPosition = { 3.f * cos( glm::radians(360.f / m_Lights.size() * idx) + lightptr->count),0.f,3.f * sin(glm::radians(360.f / m_Lights.size() * idx) + lightptr->count) };
									lightptr->SetPosition(newPosition);
									lightptr->GetLightData()->lightPosition = newPosition;
									lightptr->IsCacheDirty = true;
									lightptr->count += static_cast<float>(dt);
								});
							idx++;
						}
					}
					
				}


			}
		}
	}

	void LightManager::GUILightScenario()
	{
			
			ImGui::NewLine();
			ImGui::Text("Scenario");
		if(ImGui::Button("Scenario1"))
		{
			DeleteAllLight();
			DeleteLightQueue();
			lightnum = 1;
			for (int idx = 0; idx < 16; idx++)
			{
				HS_Engine::Light* object = new HS_Engine::Light("NewLight" + std::to_string(lightnum), "SphereProcedural", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::SPHERE, E_RenderTypes::TRIANGLES);
				object->SetShader(m_LightShader);
				object->SetMaterial(HS_Engine::Material());
				object->Init();
				object->SetPosition({ 3.f * cos(+0.8f * static_cast<float>(lightnum)), 0.f, 3.f * sin(+0.8f * static_cast<float>(lightnum) + object->count) });
				object->SetScale({ 0.1f, 0.1f, 0.1f });
				object->SetPostRenderFunction([&, object,idx](double dt)
					{
						glm::vec3 newPosition = { 3.f * cos(glm::radians(360.f / m_Lights.size() *idx) + object->count),0.f,3.f * sin(glm::radians(360.f / m_Lights.size() * idx) + object->count) };
						object->SetPosition(newPosition);
						object->GetLightData()->lightPosition = newPosition;
						object->IsCacheDirty = true;
						object->count += static_cast<float>(dt);
					});

				AddLight(object);
				lightnum++;
			}
			
		}
		ImGui::SameLine();
		
		
		if(ImGui::Button("Scenario2"))
		{

			std::random_device device;
			std::mt19937 generator(device());
			std::uniform_int_distribution<int> distribution(0, 255);
			
			lightnum = 1;
			DeleteAllLight();
			DeleteLightQueue();
			for (int idx = 0; idx < 16; idx++)
			{
				HS_Engine::Light* object = new HS_Engine::Light("NewLight" + std::to_string(lightnum), "SphereProcedural", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::SPHERE, E_RenderTypes::TRIANGLES);
				object->SetShader(m_LightShader);
				object->SetMaterial(HS_Engine::Material());
				object->Init();
				object->GetLightData()->lightDiffuse = { distribution(device)/255.f,distribution(device) / 255.f ,distribution(device) / 255.f };
				object->GetLightData()->lightAmbient = { distribution(device) / 255.f,distribution(device) / 255.f ,distribution(device) / 255.f };
				object->GetLightData()->lightSpecular = { distribution(device) / 255.f,distribution(device) / 255.f ,distribution(device) / 255.f };
				object->SetPosition({ 3.f * cos(+0.8f * static_cast<float>(lightnum)), 0.f, 3.f * sin(+0.8f * static_cast<float>(lightnum) + object->count) });
				object->SetScale({ 0.1f, 0.1f, 0.1f });
				object->SetPostRenderFunction([&, object, idx](double dt)
					{
						glm::vec3 newPosition = { 3.f * cos(glm::radians(360.f / m_Lights.size() * idx) + object->count),0.f,3.f * sin(glm::radians(360.f / m_Lights.size() * idx) + object->count) };
						object->SetPosition(newPosition);
						object->GetLightData()->lightPosition = newPosition;
						object->IsCacheDirty = true;
						object->count += static_cast<float>(dt);
					});

				AddLight(object);
				lightnum++;
			}
		}

		ImGui::SameLine();
		
		if (ImGui::Button("Scenario3"))
		{

			std::random_device device;
			std::mt19937 generator(device());
			std::uniform_int_distribution<int> distribution(0, 255);
			std::uniform_int_distribution<int> distributionLight(1, 2);
			lightnum = 1;
			DeleteAllLight();
			DeleteLightQueue();
			for (int idx = 0; idx < 16; idx++)
			{
				HS_Engine::Light* object = new HS_Engine::Light("NewLight" + std::to_string(lightnum), "SphereProcedural", E_MeshTypes::BY_PROCEDURALMESH, E_Proceduralmesh::SPHERE, E_RenderTypes::TRIANGLES);
				object->SetShader(m_LightShader);
				object->SetMaterial(HS_Engine::Material());
				object->Init();
				object->GetLightData()->lightDiffuse = { distribution(device) / 255.f,distribution(device) / 255.f ,distribution(device) / 255.f };
				object->GetLightData()->lightAmbient = { distribution(device) / 255.f,distribution(device) / 255.f ,distribution(device) / 255.f };
				object->GetLightData()->lightSpecular = { distribution(device) / 255.f,distribution(device) / 255.f ,distribution(device) / 255.f };
				object->GetLightData()->lightType = distributionLight(device);
				object->SetPosition({ 3.f * cos(+0.8f * static_cast<float>(lightnum)), 0.f, 3.f * sin(+0.8f * static_cast<float>(lightnum) + object->count) });
				object->SetScale({ 0.1f, 0.1f, 0.1f });
				object->SetPostRenderFunction([&, object, idx](double dt)
					{
						glm::vec3 newPosition = { 3.f * cos(glm::radians(360.f / m_Lights.size() * idx) + object->count),0.f,3.f * sin(glm::radians(360.f / m_Lights.size() * idx) + object->count) };
						object->SetPosition(newPosition);
						object->GetLightData()->lightPosition = newPosition;
						object->IsCacheDirty = true;
						object->count += static_cast<float>(dt);
					});

				AddLight(object);
				lightnum++;
			}
		}
		
	}
}
