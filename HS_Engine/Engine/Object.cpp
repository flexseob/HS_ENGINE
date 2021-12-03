/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#include "Object.h"

#include "Engine.h"
#include "Light.h"
#include "Shader.h"

namespace HS_Engine
{
	Object::Object(std::string objectname, std::string meshname, std::string path, E_MeshTypes meshtype,
		E_RenderTypes rendertype) : m_Objname(objectname), m_path(path), m_MeshType(meshtype), m_Meshname(meshname),m_RenderType(rendertype)
	{
	}

	Object::Object(std::string objectname, std::string meshname, E_MeshTypes meshtype, E_Proceduralmesh preceduralmeshtype, E_RenderTypes rendertype)
		: m_Objname(objectname), m_MeshType(meshtype), m_Meshname(meshname), m_RenderType(rendertype), m_PreceduralmeshType(preceduralmeshtype)
	{
	}

	Object::Object(std::string objectname, Object* object)
	{
		*this = *object;
		m_Objname = objectname;

	}

	void Object::Init()
	{
		if (!m_shader) return;
		{
			m_shader->FindUniformLocation("model");
			m_shader->FindUniformLocation("material.diffuse");
			if (!dynamic_cast<Light*>(this))
			{
				m_shader->FindUniformLocation("material.ambient");
				m_shader->FindUniformLocation("material.specular");
				m_shader->FindUniformLocation("material.shininess");
				m_shader->FindUniformLocation("material.emissive");
			}

		
			if (GetObjData().m_material.IsExistDiffuseTexture())
			{
				m_shader->FindUniformLocation("diffuse_texture_isexist");
				m_shader->FindUniformLocation("diffuse_texture");
				m_shader->FindUniformLocation("gpu_calculation");
				m_shader->FindUniformLocation("UV_mapping_mode");
				m_shader->FindUniformLocation("UV_Entity_Normal");
			}
			if (GetObjData().m_material.IsExistSpecularTexture())
			{
				m_shader->FindUniformLocation("specular_texture_isexist");
				m_shader->FindUniformLocation("specular_texture");
				
			}
			if(GetIsEnvironmentMapping() == true)
			{
				m_shader->FindUniformLocation("TopFrame");
				m_shader->FindUniformLocation("BottomFrame");
				m_shader->FindUniformLocation("LeftFrame");
				m_shader->FindUniformLocation("RightFrame");
				m_shader->FindUniformLocation("FrontFrame");
				m_shader->FindUniformLocation("BackFrame");
				
				m_shader->FindUniformLocation("EnvironmentMode");
				m_shader->FindUniformLocation("isPhongshading");
				m_shader->FindUniformLocation("isEnvironmentMapping");
				m_shader->FindUniformLocation("isPhongshading");
				m_shader->FindUniformLocation("RefractionIdx");
				m_shader->FindUniformLocation("PhongShadingvalue");
			}
	
		}
		if (!m_shader_debug) return;
		m_shader_debug->FindUniformLocation("model");

	
	}
	

	Object::Object(const Object& object)
	{
		m_mesh = object.m_mesh;
		m_RenderType = object.m_RenderType;
		m_MeshType = object.m_MeshType;
		m_Meshname = object.m_Meshname;
		m_path = object.m_path;
		m_ObjData = object.m_ObjData;
	}

	Object& Object::operator=(const Object& object)
	{
		if(this == &object)
		{
			return *this;
		}
		
		//it will be managed by manager;
		m_mesh = object.m_mesh;
		m_RenderType = object.m_RenderType;
		m_MeshType = object.m_MeshType;
		m_Meshname = object.m_Meshname;
		m_path = object.m_path;
		m_ObjData = object.m_ObjData;

		return *this;
	}

	std::string Object::GetObjectName() const
	{
		return m_Objname;
	}

	std::string Object::GetMeshName() const
	{
		return m_Meshname;
	}

	std::string Object::GetObjPath() const
	{
		return m_path;
	}

	E_MeshTypes Object::GetMeshType() const
	{
		return m_MeshType;
	}

	E_Proceduralmesh Object::GetPreceduralmeshType() const
	{
		return m_PreceduralmeshType;
	}

	E_RenderTypes Object::GetRenderType() const
	{
		return m_RenderType;
	}

	E_Environment_Mapping Object::GetEnvironmentMode() const
	{
		return m_E_EnvironmentMappingMode;
	}

	void Object::SetFractionIndex(float index)
	{
		m_RefractionIdx = index;
	}

	void Object::SetShader(std::shared_ptr<Shader> shader)
	{
		m_shader = shader;
	}

	void Object::SetShaderForDebug(std::shared_ptr<Shader> shader)
	{
		m_shader_debug = shader;
	}

	void Object::SetPosition(glm::vec3 position)
	{
		m_ObjData.m_Position = position;
	}

	void Object::SetScale(glm::vec3 scale)
	{
		m_ObjData.m_Scale = scale;
	}

	void Object::SetRotate(float angle, glm::vec3 axis)
	{
		m_ObjData.m_Angle = angle;
		m_ObjData.m_RotateAxis = axis;
	}

	void Object::SetObjectColor(glm::vec3 objectcolor)
	{
		m_ObjData.m_ObjectColor = objectcolor;
	}

	void Object::SetMaterial(Material material)
	{
		m_ObjData.m_material = material;
		m_Materialname = material.GetMaterialName();
	}

	void Object::SetMaterial(Material* material)
	{
		m_ObjData.m_material = *material;
		m_Materialname = material->GetMaterialName();
		if (GetIsEnvironmentMapping() == true)
		{
			auto& mTextureManager = Engine::GetObjectManager().GetTextureManager();
			std::string objname = GetObjectName();
			m_ObjData.m_material.AddCubeMappingTexture({ {E_CUBE_MAP::TOP,mTextureManager.GetTexture(objname + "TopFrame")},
										{E_CUBE_MAP::BOTTOM,mTextureManager.GetTexture(objname + "BottomFrame")},
										{E_CUBE_MAP::BACK,mTextureManager.GetTexture(objname + "BackFrame")},
										{E_CUBE_MAP::FRONT,mTextureManager.GetTexture(objname + "FrontFrame")},
										{E_CUBE_MAP::RIGHT,mTextureManager.GetTexture(objname + "RightFrame")},
										{E_CUBE_MAP::LEFT,mTextureManager.GetTexture(objname + "LeftFrame")} });
		
		}
	}

	void Object::SetMaterialDiffuseTexture(Texture* texture)
	{
		m_ObjData.m_material.SetDiffuseTexture(texture);
	}

	void Object::SetMaterialSpecularTexture(Texture* texture)
	{
		m_ObjData.m_material.SetSpecularTexture(texture);
	}


	void Object::PreRender()
	{
		m_ObjData.m_ModelMat = glm::mat4(1.0f);
		m_ObjData.m_ModelMat = glm::translate(m_ObjData.m_ModelMat, m_ObjData.m_Position);
		m_ObjData.m_ModelMat = glm::scale(m_ObjData.m_ModelMat, m_ObjData.m_Scale);
		m_ObjData.m_ModelMat = glm::rotate(m_ObjData.m_ModelMat, glm::radians(m_ObjData.m_Angle), m_ObjData.m_RotateAxis);

		m_mesh->PreRender();
	}

	void Object::Render()
	{
		if (!m_shader) return;
		
		m_shader->Bind();
		m_shader->BindUniformVariable("material.diffuse", m_ObjData.m_material.Getdiffuse());

		if (!dynamic_cast<Light*>(this))
		{
			m_shader->BindUniformVariable("material.ambient", m_ObjData.m_material.GetAmbient());
			m_shader->BindUniformVariable("material.specular", m_ObjData.m_material.GetSpecular());

			m_shader->BindUniformVariable("material.shininess", m_ObjData.m_material.GetShinness());
			m_shader->BindUniformVariable("material.emissive", m_ObjData.m_material.GetEmissive());

			if (GetObjData().m_material.IsExistDiffuseTexture())
			{
				GetObjData().m_material.GetDiffuseTexture()->Bind();
				m_shader->BindUniformVariable("diffuse_texture_isexist", true);
				m_shader->BindUniformVariable("diffuse_texture", static_cast<int>(GetObjData().m_material.GetDiffuseTexture()->GetTextureIdx()));
				m_shader->BindUniformVariable("gpu_calculation", GetMesh()->GetGPUCalucation());
				m_shader->BindUniformVariable("UV_mapping_mode", static_cast<int>(GetMesh()->GetUVType()));
				m_shader->BindUniformVariable("UV_Entity_Normal", static_cast<int>(GetMesh()->GetUV_Entity_Types()));
				
			}
			else
			{

				m_shader->BindUniformVariable("diffuse_texture_isexist", false);
			}
			
			if (GetObjData().m_material.IsExistSpecularTexture())
			{
				GetObjData().m_material.GetSpecularTexture()->Bind();
				m_shader->BindUniformVariable("specular_texture_isexist", true);
				m_shader->BindUniformVariable("specular_texture", static_cast<int>(GetObjData().m_material.GetSpecularTexture()->GetTextureIdx()));
			}
			else
			{
				m_shader->BindUniformVariable("specular_texture_isexist", false);
			}

			if(GetIsEnvironmentMapping() == true)
			{
				m_shader->BindUniformVariable("isEnvironmentMapping", GetIsEnvironmentMapping());
				for(auto& texture : GetObjData().m_material.GetMappingTexture())
				{
					texture.second->Bind();
					m_shader->FindUniformLocation(texture.first);
					const int texidx = static_cast<int>(texture.second->GetTextureIdx());
					m_shader->BindUniformVariable(texture.first, texidx);
					m_shader->BindUniformVariable("EnvironmentMode", static_cast<int>(GetEnvironmentMode()));
					m_shader->BindUniformVariable("RefractionIdx", GetRefractionIndex());
					m_shader->BindUniformVariable("isPhongshading", m_IsPhongShading_with);
					m_shader->BindUniformVariable("PhongShadingvalue", m_PhongShadingValue);
				}
				
			}else
			{
				m_shader->BindUniformVariable("isEnvironmentMapping", false);
			}

			
		}
		
		m_shader->BindUniformVariable("model", m_ObjData.m_ModelMat);
		
		m_mesh->Render(m_RenderType);
		m_shader->Unbind();

		if (GetIsDisplayDebug() == true)
		{
			m_shader_debug->FindUniformLocation("model");
			m_shader_debug->Bind();
			m_shader_debug->BindUniformVariable("model", m_ObjData.m_ModelMat);
			m_mesh->DebugRender();
			m_shader_debug->Unbind();
		}
	}

	void Object::PostRender([[maybe_unused]]double dt)
	{
		if(m_PostRenderFunction != nullptr)
		{
			m_PostRenderFunction(dt);
		}
	}

	void Object::SetPostRenderFunction(std::function<void(double)> function)
	{
		m_PostRenderFunction = function;
	}

	void Object::SetMesh(Mesh* mesh)
	{
		m_mesh = mesh;
		m_Meshname = m_mesh->GetMeshName();
		m_RenderType = m_mesh->GetRenderType();
	}

	void Object::SetEnvironmentMapping(bool Isenvironment)
	{
		if (m_IsEnvironmentMapping != true && Isenvironment == true)
		{
			m_IsEnvironmentMapping = Isenvironment;
			auto& mTextureManager = HS_Engine::Engine::GetObjectManager().GetTextureManager();
			mTextureManager.AddTextureInternal(GetObjectName() + "FrontFrame", new HS_Engine::Texture(0x8, 1600, 900));
			mTextureManager.AddTextureInternal(GetObjectName() + "TopFrame", new HS_Engine::Texture(0x9, 1600, 900));
			mTextureManager.AddTextureInternal(GetObjectName() + "BottomFrame", new HS_Engine::Texture(0xA, 1600, 900));
			mTextureManager.AddTextureInternal(GetObjectName() + "LeftFrame", new HS_Engine::Texture(0xB, 1600, 900));
			mTextureManager.AddTextureInternal(GetObjectName() + "RightFrame", new HS_Engine::Texture(0xC, 1600, 900));
			mTextureManager.AddTextureInternal(GetObjectName() + "BackFrame", new HS_Engine::Texture(0xD, 1600, 900));
		}
		else if(m_IsEnvironmentMapping == true && Isenvironment == false)
		{
			m_IsEnvironmentMapping = false;
		}
	}

	void Object::SetEnvironmentMode(E_Environment_Mapping mode)
	{
		m_E_EnvironmentMappingMode = mode;
	}

	void Object::SetIsEnvironmentPhongShading(bool phongshading)
	{
		m_IsPhongShading_with = phongshading;
	}

	void Object::ReleaseShaderPointer()
	{
		m_shader.reset();
		m_shader_debug.reset();
	}

	glm::vec3 Object::GetPosition() const
	{
		return m_ObjData.m_Position;
	}

	glm::vec3 Object::GetScale() const
	{
		return m_ObjData.m_Scale;
	}

	float Object::GetRefractionIndex() const
	{
		return m_RefractionIdx;
	}

	bool Object::GetIsDisplayDebug() const
	{
		return m_IsDisplayDebug;
	}

	void Object::SetIsDisplayDebug(bool Isdebug)
	{
		m_IsDisplayDebug = Isdebug;
	}

	float Object::GetPhongShadingValue() const
	{
		return 	m_PhongShadingValue;
	}

	void Object::SetPhongShadingValue(float value)
	{
		m_PhongShadingValue = value;
	}

	Mesh* Object::GetMesh() const
	{
		return m_mesh;
	}

	Object::ObjectData& Object::GetObjData()
	{
		return m_ObjData;
	}
}
