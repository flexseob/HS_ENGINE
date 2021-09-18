#include "Object.h"

namespace HS_Engine
{
	Object::Object(std::string objectname, std::string meshname, std::string path, E_MeshTypes meshtype)
	: m_Objname(objectname),m_path(path), m_MeshType(meshtype), m_Meshname(meshname)
	{
	}

	Object::Object(std::string objectname, Object* object)
	{
		*this = *object;
		m_Objname = objectname;

	}

	Object::Object(const Object& object)
	{
		m_mesh = object.m_mesh;
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
		m_MeshType = object.m_MeshType;
		m_Meshname = object.m_Meshname;
		m_path = object.m_path;
		m_ObjData = object.m_ObjData;
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

	E_MeshTypes Object::GetMeshTypes() const
	{
		return m_MeshType;
	}

	void Object::SetShader(std::shared_ptr<Shader> shader)
	{
		m_shader = shader;
	}

	void Object::PreRender()
	{
		m_ObjData.m_ModelMat = glm::mat4(1.0f);
		m_ObjData.m_ModelMat = glm::translate(m_ObjData.m_ModelMat, m_ObjData.m_Position);
		m_ObjData.m_ModelMat = glm::scale(m_ObjData.m_ModelMat, m_ObjData.m_Scale);
		m_ObjData.m_ModelMat = glm::rotate(m_ObjData.m_ModelMat, glm::radians(m_ObjData.m_Angle), m_ObjData.m_RotateAxis);
	}

	void Object::Render()
	{

	
		
	}

	void Object::PostRender()
	{
		
	}

	void Object::SetMesh(Mesh* mesh)
	{
		m_mesh = mesh;
	}

	Mesh* Object::GetMesh() const
	{
		return m_mesh;
	}
}
