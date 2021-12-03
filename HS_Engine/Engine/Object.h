/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
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
		Object(std::string objectname, std::string meshname, E_MeshTypes meshtype,E_Proceduralmesh preceduralmeshtype, E_RenderTypes rendertype);
		Object(std::string objectname, Object* object);

		virtual void Init();
		std::string GetObjectName() const;
		std::string GetMeshName() const;
		std::string GetObjPath() const;
		E_MeshTypes GetMeshType() const;
		E_Proceduralmesh GetPreceduralmeshType() const;
		E_RenderTypes GetRenderType() const;
		E_Environment_Mapping GetEnvironmentMode() const;
		
		void SetFractionIndex(float index);
		void SetShader(std::shared_ptr<Shader> shader);
		void SetShaderForDebug(std::shared_ptr<Shader> shader);
		void SetPosition(glm::vec3 position);
		void SetScale(glm::vec3 scale);
		void SetRotate(float angle, glm::vec3 axis);
		void SetObjectColor(glm::vec3 objectcolor);
		void SetMaterial(Material material);
		void SetMaterial(Material* material);
		void SetMaterialDiffuseTexture(Texture* texture);
		void SetMaterialSpecularTexture(Texture* texture);
		void SetMesh(Mesh* mesh);
		void SetEnvironmentMapping(bool Isenvironment);
		void SetEnvironmentMode(E_Environment_Mapping mode);
		bool GetIsEnvironmentMapping()const { return m_IsEnvironmentMapping; }
		void SetIsEnvironmentPhongShading(bool phongshading);
		bool GetIsEnvironmentPhongShading() const { return m_IsPhongShading_with; }
		void ReleaseShaderPointer();
		glm::vec3 GetPosition() const;
		glm::vec3 GetScale() const;
		float GetRefractionIndex() const;

		bool GetIsDisplayDebug() const;
		void SetIsDisplayDebug(bool Isdebug);
		float GetPhongShadingValue() const;
		void SetPhongShadingValue(float value);
		
		Mesh* GetMesh() const;
		ObjectData& GetObjData();
		std::string GetMaterialName() const
		{
			return m_Materialname;
		}
		virtual void PreRender();
		virtual void Render();
		virtual void PostRender(double dt);

		void SetPostRenderFunction(std::function<void(double)> function);
		std::function<void(double)> m_PostRenderFunction = nullptr;
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
	protected:
		
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<Shader> m_shader_debug;

		E_MeshTypes m_MeshType;
		E_RenderTypes m_RenderType;
		E_Proceduralmesh m_PreceduralmeshType =E_Proceduralmesh::NONE;

		std::string m_Objname;
		std::string m_Meshname;
		std::string m_Materialname;
		std::string m_path;
		ObjectData m_ObjData;
		Mesh* m_mesh = nullptr;
		bool m_IsDisplayDebug = false;
		glm::mat4 m_DebugMat;
		bool m_IsEnvironmentMapping = false;
		E_Environment_Mapping m_E_EnvironmentMappingMode = E_Environment_Mapping::REFLECTION;
		float m_RefractionIdx = 1.002f;
		bool m_IsPhongShading_with = false;
		float m_PhongShadingValue = 0.5f;
	};


}
