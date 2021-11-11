/* Start Header-------------------------------------------------------
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Light.cpp
Purpose : Light Object class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#include "Light.h"

namespace HS_Engine
{
	Light::Light(std::string objectname, std::string meshname, std::string path, E_MeshTypes meshtype,E_RenderTypes rendertype) : Object( objectname,  meshname, path, meshtype, rendertype)
	{
		
	}

	Light::Light(std::string objectname, std::string meshname, E_MeshTypes meshtype,E_Proceduralmesh preceduralmeshtype, E_RenderTypes rendertype) : Object( objectname, meshname,  meshtype,  preceduralmeshtype, rendertype)
	{
		
	}

	void Light::Init()
	{
		Object::Init();
	}

	void Light::PreRender()
	{
		Object::PreRender();

		
		m_Lightdata.lightPosition = GetPosition();
		
		SetMaterial(Material(GetLightData()->lightAmbient, GetLightData()->lightDiffuse, GetLightData()->lightSpecular, GetObjData().m_material.GetShinness()));
	}

	void Light::PostRender(double dt)
	{
		if(IsMoving)
		{
			Object::PostRender(dt);
		}
		
	}
}
