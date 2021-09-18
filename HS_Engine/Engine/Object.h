#pragma once
#include <string>
#include <memory>

#include "Types.h"
#include "Material.h"
#include "Mesh/Mesh.h"

namespace HS_Engine
{
	class Object
	{
	public:
		Object(std::string objectname,std::string meshname ,std::string path,E_MeshTypes meshtype);
		Object(std::string objectname, Object* object);
		std::string GetObjectName() const;
		std::string GetMeshName() const;
		std::string GetObjPath() const;
		E_MeshTypes GetMeshTypes() const;

		void SetShader(std::shared_ptr<Shader> shader);
		void SetPosition(glm::vec3 position);
		void SetScale(glm::vec3 scale);
		void SetRotate(float angle, glm::vec3 Axis);
		void SetObjectColor(glm::vec3 objectcolor);
		
		void SetMesh(Mesh* mesh);
		Mesh* GetMesh() const;
		void PreRender();
		void Render();
		void PostRender();
	private:
		Object(const Object& object);
		Object& operator=(const Object& object);
	public:	

		struct ObjectData
		{
			Material m_material;
			glm::vec3 m_Position;
			glm::vec3 m_Scale;
			glm::vec3 m_RotateAxis;
			glm::vec3 m_ObjectColor;
			glm::mat4 m_ModelMat;
			float m_Angle;
		};
	
	private:
		
		std::shared_ptr<Shader> m_shader;
		E_MeshTypes m_MeshType;
		std::string m_Objname;
		std::string m_Meshname;
		std::string m_path;
		ObjectData m_ObjData;
		Mesh* m_mesh = nullptr;

		
	};


}
