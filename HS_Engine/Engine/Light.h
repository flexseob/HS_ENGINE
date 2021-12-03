/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#pragma once
#include <glm/glm.hpp>

#include "Object.h"
#include "Types.h"

namespace HS_Engine
{
	struct LightRawData
	{
		glm::vec3 lightPosition{0.f,0.f,0.f};
		float cutoff = 0.520f;
		glm::vec3 lightDiffuse{ 1.f,1.f,1.f };
		float outcutoff = 0.884f;
		glm::vec3 lightAmbient{ 1.f,1.f,1.f };
		float falloff = 64.f;
		glm::vec3 lightSpecular{ 1.f,1.f,1.f }; 
		float Radius;
		glm::vec3 direction{ 0.f,-0.44f,0.f };
		int lightType = E_LightType::POINTLIGHT;
	};
	
	class Light : public Object
	{
	public:
		Light(std::string objectname, std::string meshname, std::string path, E_MeshTypes meshtype,E_RenderTypes rendertype);
		Light(std::string objectname, std::string meshname, E_MeshTypes meshtype, E_Proceduralmesh preceduralmeshtype, E_RenderTypes rendertype);
		void Init() override;
		void PreRender() override;
		void PostRender(double dt) override;

		LightRawData* GetLightData() { return &m_Lightdata; }
		LightRawData& GetLightDataRef() { return m_Lightdata; }
		LightRawData GetLightDataval() { return m_Lightdata; }
		bool IsCacheDirty = false;
		bool IsMoving = true;
	private:
		LightRawData m_Lightdata;
	};

}
