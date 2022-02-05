/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
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
