/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#pragma once
#include <string>
#include <utility>
#include <vector>
#include "Light.h"
#include "ObjectManager.h"

namespace HS_Engine
{


	class LightManager
	{
	public:
		friend class ObjectManager;
		
		LightManager() = default;
		LightManager(int MAXLIGHT ,unsigned UniformBufferidx);
		~LightManager();
		void AddLight(Light* light);
		void SetInit(int MAXLIGHT, unsigned UniformBUfferidx);
		void LightUniformBufferUpdate() const;
		void DeleteLight(std::string lightname);
		void DeleteAllLight();
		void DeleteLightQueue();
		void RenderAll(double dt);
		void RenderAllForFrameBuffer(double dt);
		void UpdateLight();
		void SetShader(std::shared_ptr<Shader> shader);
		void ChangeAllLightShader(std::shared_ptr<Shader> shader);
		Light* GetLight(std::string name);
		void GUILightViewer();

		void GUILightScenario();
		int GetNumberOfLight() const {return  m_NumberOfLight; }
		std::vector<LightRawData>& GetLightRawDatas() { return m_LightRawDatas; }
	private:
		std::vector<std::pair<std::string, Light*>> m_Lights;
		std::vector<LightRawData> m_LightRawDatas;
		std::queue<Light*> mNeedtoDeleteLight;
		std::shared_ptr<Shader> m_LightShader;
		std::shared_ptr<UniformBuffer> m_LightUniformBuffer;
		int m_NumberOfLight = 0;
		bool m_IsAllRotating = false;
		int m_MaxLightCount;
		unsigned int m_LightUniformIndex;
	};


	
}
