#pragma once
#include <functional>
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
		struct ObjectData;
		Object(std::string objectname,std::string meshname ,std::string path,E_MeshTypes meshtype, E_RenderTypes rendertype);
		Object(std::string objectname, std::string meshname, E_MeshTypes meshtype,E_Preceduralmesh preceduralmeshtype, E_RenderTypes rendertype);
		Object(std::string objectname, Object* object);

		void Init();
		std::string GetObjectName() const;
		std::string GetMeshName() const;
		std::string GetObjPath() const;
		E_MeshTypes GetMeshType() const;
		E_Preceduralmesh GetPreceduralmeshType() const;
		E_RenderTypes GetRenderType() const;
		
		void SetShader(std::shared_ptr<Shader> shader);
		void SetShaderForDebug(std::shared_ptr<Shader> shader);
		void SetPosition(glm::vec3 position);
		void SetScale(glm::vec3 scale);
		void SetRotate(float angle, glm::vec3 axis);
		void SetObjectColor(glm::vec3 objectcolor);
		void SetMaterial(Material material);
		void SetMaterial(Material* material);
		void SetMesh(Mesh* mesh);

		glm::vec3 GetPosition() const;
		glm::vec3 GetScale() const;

		bool GetIsDisplayDebug() const;
		void SetIsDisplayDebug(bool Isdebug);

		Mesh* GetMesh() const;
		ObjectData& GetObjData();

		void PreRender();
		void Render();
		void PostRender(double dt);


		std::function<void(double)> pre_render_function = nullptr;
		float count = 0;
	private:
		Object(const Object& object);
		Object& operator=(const Object& object);
	public:	

		struct ObjectData
		{
			Material m_material;
			glm::vec3 m_Position{0.f,0.f,0.f};
			glm::vec3 m_Scale{1.f};
			glm::vec3 m_RotateAxis{0.f,0.f,1.f};
			glm::vec3 m_ObjectColor{1.f,0.f,0.f};
			glm::mat4 m_ModelMat{1.f};
			float m_Angle =0.f;
		};
	
		glm::vec3 m_DebugLineScale = { 1.f,1.f,1.f };
	private:
		
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Shader> m_shader_debug;
		E_MeshTypes m_MeshType;
		E_RenderTypes m_RenderType;
		E_Preceduralmesh m_PreceduralmeshType =E_Preceduralmesh::NONE;
		std::string m_Objname;
		std::string m_Meshname;
		std::string m_path;
		ObjectData m_ObjData;
		Mesh* m_mesh = nullptr;
		bool m_IsDisplayDebug = false;
		glm::mat4 m_DebugMat;
		//std::function<void> mPostRenderFunction = nullptr;
		
	};


}
