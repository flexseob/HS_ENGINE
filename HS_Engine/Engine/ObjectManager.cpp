/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#include "ObjectManager.h"

#include <imgui.h>

#include "Types.h"
#include <iostream>

#include "Light.h"
#include "MeshManager.h"
#include "Object.h"
#include "Core/Camera.h"


namespace HS_Engine
{
	ObjectManager::~ObjectManager()
	{
		mMeshManager.DeleteAllMesh();
	}

	void ObjectManager::AddObject(Object* object)
	{
		auto findobject = mObjects.find(object->GetObjectName());

		if (findobject == mObjects.end())
		{
			std::string objectname = object->GetObjectName();
			if (object->GetMesh() == nullptr)
			{
				Mesh* mesh = nullptr;
				switch (object->GetMeshType())
				{
				case E_MeshTypes::BY_PATHMESH:
					mesh = mMeshManager.AddMesh(object->GetMeshName(), object->GetObjPath(), object->GetRenderType());
					mesh->SetRenderType(object->GetRenderType());
					object->SetMesh(mesh);
					mObjects.insert({ objectname,object });
					if (object->GetIsEnvironmentMapping() == true)
					{
						mEnvironmentObjects.insert({ objectname,object });
					}
					break;

				case E_MeshTypes::BY_PROCEDURALMESH:
					switch (object->GetPreceduralmeshType())
					{
					case E_Proceduralmesh::SPHERE:
						mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Proceduralmesh::SPHERE, object->GetRenderType());
						object->SetMesh(mesh);
						mObjects.insert({ objectname,object });
						break;
					case E_Proceduralmesh::CUBE:
						mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Proceduralmesh::CUBE, object->GetRenderType());
						object->SetMesh(mesh);
						break;
					case E_Proceduralmesh::CYLINDER:
						mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Proceduralmesh::CYLINDER, object->GetRenderType());
						object->SetMesh(mesh);
						break;
					case E_Proceduralmesh::PLANE:
						mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Proceduralmesh::PLANE, object->GetRenderType());
						object->SetMesh(mesh);
						break;
					case E_Proceduralmesh::CIRCLE:
						mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Proceduralmesh::CIRCLE, object->GetRenderType());
						object->SetMesh(mesh);
						break;
					}
					mObjects.insert({ objectname,object });
					if(object->GetIsEnvironmentMapping() == true)
					{
						mEnvironmentObjects.insert({ objectname,object });
					}
					break;
				}
			}
			else
			{
				mObjects.insert({ objectname,object });
				if (object->GetIsEnvironmentMapping() == true)
				{
					mEnvironmentObjects.insert({ objectname,object });
				}
			}
			if(dynamic_cast<Light*>(object))
			{
				m_Light_count++;
			}
		}
		else
		{
			std::cout << "Object name is Exist! : " << object->GetObjectName() << std::endl;
			delete object;
		}


	}

	void ObjectManager::SetSkyBox(SkyBox* skybox)
	{

			std::string objectname = skybox->GetObjectName();
			if (skybox->GetMesh() == nullptr)
			{
				Mesh* mesh = nullptr;
				switch (skybox->GetMeshType())
				{
				case E_MeshTypes::BY_PATHMESH:
					mesh = mMeshManager.AddMesh(skybox->GetMeshName(), skybox->GetObjPath(), skybox->GetRenderType());
					mesh->SetRenderType(skybox->GetRenderType());
					skybox->SetMesh(mesh);
					break;

				case E_MeshTypes::BY_PROCEDURALMESH:
					switch (skybox->GetPreceduralmeshType())
					{
					case E_Proceduralmesh::SPHERE:
						mesh = mMeshManager.AddMesh(skybox->GetMeshName(), E_Proceduralmesh::SPHERE, skybox->GetRenderType());
						skybox->SetMesh(mesh);
						break;
					case E_Proceduralmesh::CUBE:
						mesh = mMeshManager.AddMesh(skybox->GetMeshName(), E_Proceduralmesh::CUBE, skybox->GetRenderType());
						skybox->SetMesh(mesh);
						break;
					case E_Proceduralmesh::CYLINDER:
						mesh = mMeshManager.AddMesh(skybox->GetMeshName(), E_Proceduralmesh::CYLINDER, skybox->GetRenderType());
						skybox->SetMesh(mesh);
						break;
					case E_Proceduralmesh::PLANE:
						mesh = mMeshManager.AddMesh(skybox->GetMeshName(), E_Proceduralmesh::PLANE, skybox->GetRenderType());
						skybox->SetMesh(mesh);
						break;
					case E_Proceduralmesh::CIRCLE:
						mesh = mMeshManager.AddMesh(skybox->GetMeshName(), E_Proceduralmesh::CIRCLE, skybox->GetRenderType());
						skybox->SetMesh(mesh);
						break;
					}
					break;
				}
			}
			m_skybox = skybox;
			
		
	}

	void ObjectManager::DeleteObject(std::string objname)
	{
		auto findobject = mObjects.find(objname);

		if (findobject != mObjects.end())
		{
			Object* object = findobject->second;
			mNeedtoDeleteObjects.push(object);
			mObjects.erase(findobject);
			if (object->GetIsEnvironmentMapping() == true)
			{
				const auto findEnvirObject = mEnvironmentObjects.find(objname);
				mEnvironmentObjects.erase(findEnvirObject);
			}
			std::cout << "Object is deleted " << objname << std::endl;
		}
		else
		{
			std::cout << "Object name is not exist : " << objname << std::endl;
		}
	}

	void ObjectManager::DeleteObjectQueue()
	{
		while (!mNeedtoDeleteObjects.empty())
		{
			Object* obj = mNeedtoDeleteObjects.front();
			delete obj;
			obj = nullptr;
			mNeedtoDeleteObjects.pop();
		}

	}

	void ObjectManager::DeleteObjectAll()
	{
		for (auto& object : mObjects)
		{
			Object* objptr = object.second;
			objptr->ReleaseShaderPointer();
			delete objptr;
			object.second = nullptr;
		}
		if(m_skybox)
		{
			delete m_skybox;
			m_skybox = nullptr;
		}
		mObjects.clear();
		mEnvironmentObjects.clear();
	}



	void ObjectManager::RenderAll([[maybe_unused]] double dt)
	{
		for (auto& object : mObjects)
		{
			Object* obj = object.second;
			obj->PreRender();
			obj->Render();
			obj->PostRender(dt);
		}
		if (m_skybox)
		{
			m_skybox->PreRender();
			m_skybox->Render();
			m_skybox->PostRender(dt);
		}
	}

	void ObjectManager::RenderAllForFrameBuffer(double dt ,Object* itself)
	{
		for (auto& object : mObjects)
		{
			Object* obj = object.second;
			obj->PreRender();
			if(obj != itself)
			{
				obj->Render();
			}
		}
		if (m_skybox)
		{
			m_skybox->PreRender();
			m_skybox->Render();
			m_skybox->PostRender(dt);
		}
		
	}

	void ObjectManager::RenderAllDeferred(double dt)
	{
		for(auto& object : mObjects)
		{
			Object* obj = object.second;
			obj->PreRender();
		}
		
	}

	void ObjectManager::RenderDebugDraw(double dt)
	{
		for(auto& object : mObjects)
		{
			Object* obj = object.second;
			obj->DebugRender();
		}
		
	}

	void ObjectManager::ChangeAllObjectShader(std::shared_ptr<Shader> shader)
	{
		for (auto& object : mObjects)
		{
			Object* obj = object.second;
			obj->SetShader(shader);
			obj->Init();
		}
	}

	Object* ObjectManager::GetObject(std::string objname)
	{
		auto findobject = mObjects.find(objname);

		if (findobject != mObjects.end())
		{
			return findobject->second;
		}
		else
		{
			return nullptr;
		}

	}

	void ObjectManager::HelperPrecedualMesh(Object* object)
	{
		if (object->GetMeshType() != E_MeshTypes::BY_PATHMESH)
			return;
		std::string precedualmeshname = object->GetMeshName();
		std::string objectname = object->GetObjectName();

		if (precedualmeshname == "Sphere")
		{
			Mesh* mesh = mMeshManager.AddMesh(precedualmeshname, E_Proceduralmesh::SPHERE, object->GetRenderType());
			object->SetMesh(mesh);
			mObjects.insert({ objectname,object });
		}


	}

	void ObjectManager::EnvironmentMapping(std::function<void(Camera&, Object*)> function, std::shared_ptr<FrameBuffer> framebuffer)
	{

		mIsEnvironmentmappingFunctionExist = true;
		GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT };
		GLuint numBuffers = 2;
		
		HS_Engine::Camera camera;
		camera.Zoom = 90.f;
		for (auto& envirObj : mEnvironmentObjects)
		{
			camera.Position = envirObj.second->GetPosition();
			for (int j = 0; j < 6; ++j)
			{
				framebuffer->Bind();
				camera.Yaw = Yaw[j];
				camera.Pitch = Pitch[j];
				camera.updateCameraVectors();
				glDrawBuffers(numBuffers, DrawBuffers);
				framebuffer->CreateFrameTexture(mTextureManager.GetTexture(envirObj.first+mappingname[j]));
				function(camera, envirObj.second);
				framebuffer->UnBind();
			}
			
		}

		
		

		
	}

	void ObjectManager::DeferredShading(std::function<void()> function, std::shared_ptr<FrameBuffer> framebuffer)
	{
		GLenum DrawBuffers[8] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4,GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7 };
		GLuint numBuffers =8;

		framebuffer->Bind();
		function();
		glDrawBuffers(numBuffers, DrawBuffers);
		for(int attachement = 0; attachement < 8; ++attachement)
		{
			framebuffer->CreateFrameTexture(mTextureManager.GetTexture(mDeferredMappingName[attachement]), DrawBuffers[attachement]);
		}
		framebuffer->UnBind();
	}

	void ObjectManager::GUIViewer()
	{
		static const char* current_object = nullptr;
		static const char* current_mesh = nullptr;
		static const char* current_material = nullptr;
		static bool DebugDisplay = false;
		bool is_selected = true;
		bool is_selected1 = true;
		bool is_selected2 = true;
		if (ImGui::BeginCombo("object", current_object))
		{
			for (auto& obj : mObjects)
			{
				is_selected = (current_object == obj.first.c_str());

				if (ImGui::Selectable(obj.first.c_str(), is_selected))
				{
					current_object = obj.first.c_str();
				}
			}


			ImGui::EndCombo();
		}
		if (is_selected)
		{
			if (current_object != nullptr)
			{
				Object* object = GetObject(current_object);
				if (object != nullptr && dynamic_cast<Light*>(object) == nullptr)
				{
					if(ImGui::Button("DeleteObject"))
					{
						DeleteObject(object->GetObjectName());
					}
					Mesh* mesh = object->GetMesh();
					ProceduralMesh* precedural_mesh = dynamic_cast<ProceduralMesh*>(object->GetMesh());
					if (precedural_mesh == nullptr)
					{
						if (mesh->GetNormalType() == E_NormalTypes::VERTEX)
						{
							ImGui::Text("Current RenderType : VertexNormalRender");
						}
						else
						{
							ImGui::Text("Current RenderType : FaceNormalRender");
						}

						if (ImGui::Button("FaceNormalRender"))
						{
							mesh->SetCheckFlag();
							if (mesh->GetNormalType() == E_NormalTypes::VERTEX)
								mesh->ChangeRenderNormalType(true);

						}
						if (ImGui::Button("VertexNormalRender"))
						{
							mesh->SetCheckFlag();
							if (mesh->GetNormalType() == E_NormalTypes::FACE)
								mesh->ChangeRenderNormalType(false);
						}

					}
					DebugDisplay = object->GetIsDisplayDebug();
					if (object->GetObjectName() != "orbit")
					{
						ImGui::Checkbox("DisplayNormalDebug", &DebugDisplay);
						if (DebugDisplay == true)
						{
							if (mesh->m_IsFaceNormalDebug)
								ImGui::Text("DebugDisplay : FaceNormalDebug");
							else
								ImGui::Text("DebugDisplay : VertexNormalDebug");
							object->SetIsDisplayDebug(true);
							if (ImGui::Button("FaceNormalDebug"))
							{
								mesh->SetCheckFlag();
								mesh->ChangeDebugNormalType(true);
								mesh->m_IsFaceNormalDebug = true;

							}
							if (ImGui::Button("VertexNormalDebug"))
							{
								mesh->SetCheckFlag();
								mesh->ChangeDebugNormalType(false);
								mesh->m_IsFaceNormalDebug = false;
							}

						}
						else
						{
							object->SetIsDisplayDebug(false);
						}
					}
					
					if (mIsEnvironmentmappingFunctionExist)
					{
						bool is_change2 = object->GetIsEnvironmentMapping();
						bool check = ImGui::Checkbox("EnvironemntSetting On/off", &is_change2);
						object->SetEnvironmentMapping(is_change2);
						if (is_change2 == false && check == true)
						{
							mEnvironmentObjects.erase(object->GetObjectName());
						}
						else if (is_change2 == true && check == true)
						{
							mEnvironmentObjects.insert({ object->GetObjectName(),object });
							object->GetObjData().m_material.AddCubeMappingTexture({ {E_CUBE_MAP::TOP,mTextureManager.GetTexture(object->GetObjectName() + "TopFrame")},
											{E_CUBE_MAP::BOTTOM,mTextureManager.GetTexture(object->GetObjectName() + "BottomFrame")},
											{E_CUBE_MAP::BACK,mTextureManager.GetTexture(object->GetObjectName() + "BackFrame")},
											{E_CUBE_MAP::FRONT,mTextureManager.GetTexture(object->GetObjectName() + "FrontFrame")},
											{E_CUBE_MAP::RIGHT,mTextureManager.GetTexture(object->GetObjectName() + "RightFrame")},
											{E_CUBE_MAP::LEFT,mTextureManager.GetTexture(object->GetObjectName() + "LeftFrame")} });
						}
					}
					if (object->GetIsEnvironmentMapping() == true)
					{
						ImGui::NewLine();
						ImGui::TextColored({0.f,1.f,0.f,1.f},"Environment");
					
						
						float RefractionIndex = object->GetRefractionIndex();
						ImGui::DragFloat("RefractionIndex", &RefractionIndex, 0.01f, 0.f, 100.f,"%.6f");
						object->SetFractionIndex(RefractionIndex);

						int EnvironmentMode = static_cast<int>(object->GetEnvironmentMode());
						ImGui::RadioButton("Only Reflection", &EnvironmentMode, 0);
						ImGui::SameLine();
						ImGui::RadioButton("Only Refraction", &EnvironmentMode, 1);
						ImGui::SameLine();
						ImGui::RadioButton("Fresnel", &EnvironmentMode, 2);
						bool EnvironmentPhong = object->GetIsEnvironmentPhongShading();
						ImGui::Checkbox("Phongshading", &EnvironmentPhong);
						object->SetIsEnvironmentPhongShading(EnvironmentPhong);
						if(object->GetIsEnvironmentPhongShading())
						{
							float value = object->GetPhongShadingValue();
							ImGui::DragFloat("PhongMixvalue", &value, 0.01f, 0, 1.f);
							object->SetPhongShadingValue(value);
						}
						
						object->SetEnvironmentMode(static_cast<E_Environment_Mapping>(EnvironmentMode));
						static const char* Setting[] = { "Air 1.000293","Hydrogen 1.000132","Water 1.333 ","Olive Oil 1.47","Ice (solidified water) 1.31","Quartz 1.46","Diamond 2.42","Acrylic / plexiglas / Lucite 1.49"};
						const float refractionidx[8] = { 1.000293f ,1.000132f,1.333f, 1.47f ,1.31f ,1.46f ,2.42f,1.49f };
						static int num;
						bool is_change = ImGui::Combo("RefractionSetting", &num, Setting,IM_ARRAYSIZE(Setting));
						if(is_change)
						object->SetFractionIndex(refractionidx[num]);

						
					}
					
					ImGui::NewLine();
					ImGui::TextColored({ 0.f,1.f,0.f,1.f }, "Mesh Settings");
					std::string MeshGuiTitle = "MeshName : ";
					MeshGuiTitle += object->GetMeshName();
					ImGui::Text(MeshGuiTitle.c_str());
					if (ImGui::BeginCombo("Mesh", current_mesh))
					{
						for (auto& mesh : mMeshManager.GetMeshes())
						{

							is_selected1 = (object->GetMeshName() == mesh.first.c_str());
							if (ImGui::Selectable(mesh.first.c_str(), is_selected1))
							{
								current_mesh = mesh.first.c_str();
							}
						}

						ImGui::EndCombo();
					}
					if (is_selected1)
					{
						if (object->GetMeshName().c_str() != current_mesh)
							if (current_mesh != nullptr)
							{
								object->SetMesh(mMeshManager.GetMesh(current_mesh));
								is_selected1 = false;
								current_mesh = nullptr;
							}
					}

					ImGui::NewLine();
					ImGui::TextColored({ 0.f,1.f,0.f,1.f }, "Object Settings");
					HS_Engine::Object::ObjectData& objectdata = object->GetObjData();

					glm::vec3 objectPosition = objectdata.m_Position;
					ImGui::DragFloat3("ObjectPositon", reinterpret_cast<float*>(&objectPosition), 0.01f, -1000.000f, 1000.000f, "%.2f", 1);
					object->SetPosition(objectPosition);

					glm::vec3 objectScale = objectdata.m_Scale;
					ImGui::DragFloat3("ObjectScale", reinterpret_cast<float*>(&objectScale), 0.01f, 0.000f, 1000.000f, "%.2f", 1);
					object->SetScale(objectScale);

					glm::vec3 objectRotateAxis = objectdata.m_RotateAxis;
					ImGui::DragFloat3("ObjectRotateAxis", reinterpret_cast<float*>(&objectRotateAxis), 1.f, -1.000f, 1.000f, "%.2f", 1);

					float objectAngle = objectdata.m_Angle;
					ImGui::DragFloat("Angle", &objectAngle, 1.f, 0, 360.f, "%.2f", 1);
					object->SetRotate(objectAngle, objectRotateAxis);







					ImGui::NewLine();
					ImGui::TextColored({ 0.f,1.f,0.f,1.f }, "Material Settings");
					std::string MaterialName = "MaterialName : " + object->GetMaterialName();
					ImGui::Text(MaterialName.c_str());
					if (ImGui::BeginCombo("Material", current_material))
					{
						for (auto& material : mMaterialManager.GetMaterials())
						{

							is_selected2 = (object->GetMaterialName() == material.first.c_str());
							if (ImGui::Selectable(material.first.c_str(), is_selected))
							{
								current_material = material.first.c_str();
							}
						}

						ImGui::EndCombo();
					}
					if (is_selected2)
					{
						if (object->GetMaterialName().c_str() != current_material)
							if (current_material != nullptr)
							{
								object->SetMaterial(mMaterialManager.GetMaterial(current_material));
								is_selected2 = false;
								current_material = nullptr;
							}
					}
					glm::vec3 objectMaterialDiffuse = objectdata.m_material.Getdiffuse();
					glm::vec3 objectMaterialAmbient = objectdata.m_material.GetAmbient();
					glm::vec3 objectMaterialSpecular = objectdata.m_material.GetSpecular();
					glm::vec3 objectMaterialEmissive = objectdata.m_material.GetEmissive();
					float objectMaterialShinness = objectdata.m_material.GetShinness();

					bool Is_change_material = false;
				
					if (!object->GetObjData().m_material.IsExistSpecularTexture())
					{
						Is_change_material |= ImGui::ColorEdit3("diffuse", reinterpret_cast<float*>(&objectMaterialDiffuse), 1);
					}
					if(!object->GetObjData().m_material.IsExistSpecularTexture())
					{
						Is_change_material |= ImGui::ColorEdit3("Specular", reinterpret_cast<float*>(&objectMaterialSpecular), 1);
					}
					
					Is_change_material |= ImGui::ColorEdit3("Ambient", reinterpret_cast<float*>(&objectMaterialAmbient), 1);
					Is_change_material |= ImGui::DragFloat("Shinness", &objectMaterialShinness, 0.001f, 0.000f, 3.000f, "%.4f", 1);
					Is_change_material |= ImGui::DragFloat3("Emissive", reinterpret_cast<float*>(&objectMaterialEmissive), 0.01f, 0.000f, 1.000f, "%.4f", 1);
					if (Is_change_material)
					{
						Material material{ objectMaterialAmbient ,objectMaterialDiffuse ,objectMaterialSpecular ,objectMaterialShinness,objectMaterialEmissive };
						object->SetMaterial(material);
					}
					bool Is_change_UV = false;
					int uv_types = static_cast<int>(object->GetMesh()->GetUVType());
					int uv_Entity = static_cast<int>(object->GetMesh()->GetUV_Entity_Types());
					int GPU_caluation = object->GetMesh()->GetGPUCalucation();
					if (object->GetMesh()->GetMeshType() == E_MeshTypes::BY_PATHMESH)
					{
						if (object->GetObjData().m_material.IsExistSpecularTexture() || object->GetObjData().m_material.IsExistSpecularTexture())
						{
							ImGui::NewLine();
							ImGui::Text("TextureUV Setting");

							Is_change_UV |= ImGui::RadioButton("CPU Calculation",&GPU_caluation, 0);
							ImGui::SameLine();
							Is_change_UV |= ImGui::RadioButton("GPU Calculation", &GPU_caluation, 1);
							
							object->GetMesh()->SetGPUCalculation(GPU_caluation);
							
							
							Is_change_UV |= ImGui::RadioButton("CYLINDRICAL_UV", &uv_types, 0);
							ImGui::SameLine();
							Is_change_UV |= ImGui::RadioButton("SPHERICAL_UV", &uv_types, 1);
							ImGui::SameLine();
							Is_change_UV |= ImGui::RadioButton("CUBEMAPPED", &uv_types, 2);
							ImGui::NewLine();


							ImGui::Text("UV_Entity");
							bool Is_change_UV_Entity = false;
							Is_change_UV_Entity |= ImGui::RadioButton("VertexPosition", &uv_Entity, 0);
							Is_change_UV_Entity |= ImGui::RadioButton("VertexNormal", &uv_Entity, 1);



							object->GetMesh()->ChangeUVType(static_cast<E_UV_Types>(uv_types), static_cast<E_UV_Entity_Types>(uv_Entity));

						}
					}
				
					//bool Is_change_materal = 




				}
			}
		}






	}

	void ObjectManager::GUILightViewer()
	{
		
	}

	bool ObjectManager::GetIsEnvironmentFunctionExist() const
	{
		return mIsEnvironmentmappingFunctionExist;
	}
}
