#include "ObjectManager.h"

#include <imgui.h>

#include "Types.h"
#include <iostream>
#include "MeshManager.h"
#include "Object.h"


namespace HS_Engine
{
	ObjectManager::~ObjectManager()
	{
		mMeshManager.DeleteAllMesh();
	}

	void ObjectManager::AddObject(Object* object)
	{
		auto findobject = mObjects.find(object->GetObjectName());

		if(findobject == mObjects.end())
		{
			std::string objectname = object->GetObjectName();
			if (object->GetMesh() == nullptr)
			{
				Mesh* mesh = nullptr;
				switch (object->GetMeshType())
				{
					case E_MeshTypes::BY_PATHMESH:
						mesh = mMeshManager.AddMesh(object->GetMeshName(), object->GetObjPath(),object->GetRenderType());
						mesh->SetRenderType(object->GetRenderType());
						object->SetMesh(mesh);
						mObjects.insert({ objectname,object });
					break;

					case E_MeshTypes::BY_PRECEDURALMESH:
						switch(object->GetPreceduralmeshType())
						{
							case E_Preceduralmesh::SPHERE:
								mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Preceduralmesh::SPHERE,object->GetRenderType());
								object->SetMesh(mesh);
								mObjects.insert({ objectname,object });
							break;
							case E_Preceduralmesh::CUBE:
								mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Preceduralmesh::CUBE, object->GetRenderType());
								object->SetMesh(mesh);
							break;
							case E_Preceduralmesh::CYLINDER:
								mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Preceduralmesh::CYLINDER, object->GetRenderType());
								object->SetMesh(mesh);
							break;
							case E_Preceduralmesh::PLANE:
								mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Preceduralmesh::PLANE, object->GetRenderType());
								object->SetMesh(mesh);
							break;
							case E_Preceduralmesh::CIRCLE:
								mesh = mMeshManager.AddMesh(object->GetMeshName(), E_Preceduralmesh::CIRCLE, object->GetRenderType());
								object->SetMesh(mesh);
							break;
						}
						mObjects.insert({ objectname,object });
					break;
				}
			}
			else
			{
				mObjects.insert({ objectname,object });
			}
		}
		else
		{
			std::cout << "Object name is Exist! : " << object->GetObjectName() << std::endl;
			delete object;
		}

		
	}

	void ObjectManager::DeleteObject(std::string objname)
	{
		auto findobject = mObjects.find(objname);

		if (findobject != mObjects.end())
		{
			Object* object = findobject->second;
			delete object;
			object = nullptr;
			mObjects.erase(findobject);
			std::cout << "Object is deleted " << objname << std::endl;
		}
		else
		{
			std::cout << "Object name is not exist : " << objname << std::endl;
		}
	}

	void ObjectManager::DeleteObjectAll()
	{
		for(auto& object : mObjects)
		{
			Object* objptr = object.second;
			delete objptr;
			object.second = nullptr;
		}
		
		mObjects.clear();
	}



	void ObjectManager::RenderAll([[maybe_unused]]double dt)
	{
		for(auto& object : mObjects)
		{
			Object* obj = object.second;
			obj->PreRender();
			obj->Render();
			obj->PostRender(dt);
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
			std::cout << "Cannot find the object! : " << objname << std::endl;
			return nullptr;
		}
		
	}

	void ObjectManager::HelperPrecedualMesh(Object* object)
	{
		if (object->GetMeshType() != E_MeshTypes::BY_PATHMESH)
			return;
		std::string precedualmeshname = object->GetMeshName();
		std::string objectname = object->GetObjectName();
		
		if(precedualmeshname == "Sphere")
		{
			Mesh* mesh = mMeshManager.AddMesh(precedualmeshname, E_Preceduralmesh::SPHERE,object->GetRenderType());
			object->SetMesh(mesh);
			mObjects.insert({ objectname,object });
		}
		
		
	}

	void ObjectManager::GUIViewer()
	{
		static const char* current_object = nullptr;
		static bool DebugDisplay = false;
		bool is_selected = true;
		if (ImGui::BeginCombo("object", current_object))
		{
			for(auto& obj : mObjects)
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
				if (object != nullptr)
				{
					std::string MeshGuiTitle = "MeshName : ";
					MeshGuiTitle += object->GetMeshName();
					ImGui::Text(MeshGuiTitle.c_str());
					Mesh* mesh = object->GetMesh();
					if (dynamic_cast<PreceduralMesh*>(mesh) == nullptr)
					{
						if (mesh->GetNormalType() == E_NormalTypes::VERTEX)
						{
							ImGui::Text("VertexNormalRender");
						}
						else
						{
							ImGui::Text("FaceNormalRender");
						}
						
						if (ImGui::Button("FaceNormalRender"))
						{
							mesh->SetCheckFlag();
							if(mesh->GetNormalType() == E_NormalTypes::VERTEX)
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
					ImGui::Checkbox("DisplayNormalDebug", &DebugDisplay);
					if (DebugDisplay == true)
					{
						
						object->SetIsDisplayDebug(true);
						if (ImGui::Button("FaceNormalDebug"))
						{
							mesh->SetCheckFlag();
							mesh->ChangeDebugNormalType(true);

						}
						if (ImGui::Button("VertexNormalDebug"))
						{
							mesh->SetCheckFlag();
							mesh->ChangeDebugNormalType(false);
						}
					}
					else
					{
						object->SetIsDisplayDebug(false);
					}


					ImGui::NewLine();
					ImGui::Text("Object Settings");
					HS_Engine::Object::ObjectData& objectdata = object->GetObjData();
					
					glm::vec3 objectColor = objectdata.m_ObjectColor;
					ImGui::ColorEdit3("objectcolor", reinterpret_cast<float*>(&objectColor));
					object->SetObjectColor(objectColor);

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

					

					
					glm::vec3 objectMaterialDiffuse = objectdata.m_material.Getdiffuse();
					glm::vec3 objectMaterialAmbient = objectdata.m_material.GetAmbient();
					glm::vec3 objectMaterialSpecular = objectdata.m_material.GetSpecular();
					float objectMaterialShininess = objectdata.m_material.GetShinness();

					ImGui::NewLine();
					ImGui::Text("Material");
					ImGui::DragFloat3("diffuse", reinterpret_cast<float*>(&objectMaterialDiffuse),0.01f, 0.000f, 1.000f, "%.4f", 1);
					ImGui::DragFloat3("Ambient", reinterpret_cast<float*>(&objectMaterialAmbient), 0.01f, 0.000f, 1.000f, "%.4f", 1);
					ImGui::DragFloat3("Specular", reinterpret_cast<float*>(&objectMaterialSpecular), 0.01f, -1000.000f, 1.000f, "%.4f", 1);
					ImGui::DragFloat("Shininess", &objectMaterialShininess, 0.01f, 0, 1.f, "%.2f", 1);
					Material material{ objectMaterialAmbient ,objectMaterialDiffuse ,objectMaterialSpecular ,objectMaterialShininess };
					object->SetMaterial(material);
					
				}
			}
		}

			

			
		
		
	}
}
