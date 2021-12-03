/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#pragma once
#include "Object.h"
#include "Types.h"
namespace HS_Engine
{

	
	class SkyBox : public Object
	{
	public:
		SkyBox(std::string objectname, std::string meshname, std::string path, E_MeshTypes meshtype, E_RenderTypes rendertype, const std::initializer_list<CUBEMAP_INFO>& textures);
		void AddTexture(Texture* texture);
		void Render() override;
		~SkyBox();

	private:
		std::vector<CUBEMAP_INFO> m_Textures;

		std::vector<CUBEMAP_INFO>::iterator begin() { return m_Textures.begin(); }
		std::vector<CUBEMAP_INFO>::iterator end() { return m_Textures.end(); } 
		std::vector<CUBEMAP_INFO>::const_iterator begin() const { return m_Textures.cbegin(); }
		std::vector<CUBEMAP_INFO>::const_iterator end() const { return m_Textures.cend(); }
	};


	
}
