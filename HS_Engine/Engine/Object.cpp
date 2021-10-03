
#include "Object.h"
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
		m_shader->FindUniformLocation("model");

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
		m_shader_debug->FindUniformLocation("model");
		m_shader->BindUniformVariable("material.diffuse", m_ObjData.m_material.Getdiffuse());
		m_shader->BindUniformVariable("material.ambient", m_ObjData.m_material.GetAmbient());
		m_shader->BindUniformVariable("material.specular", m_ObjData.m_material.GetSpecular());
		m_shader->BindUniformVariable("model", m_ObjData.m_ModelMat);
		m_mesh->Render(m_RenderType);
		m_shader->Unbind();

		if (GetIsDisplayDebug() == true)
		{
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

	glm::vec3 Object::GetPosition() const
	{
		return m_ObjData.m_Position;
	}

	glm::vec3 Object::GetScale() const
	{
		return m_ObjData.m_Scale;
	}

	bool Object::GetIsDisplayDebug() const
	{
		return m_IsDisplayDebug;
	}

	void Object::SetIsDisplayDebug(bool Isdebug)
	{
		m_IsDisplayDebug = Isdebug;
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
