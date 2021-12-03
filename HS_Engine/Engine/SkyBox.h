/* Start Header -------------------------------------------------------
Copyright (C) <current year in format YYYY> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SkyBox.h
Purpose: SkyBox object
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_3>
class CS300 and this file is a part of assignment 3, then write: foo_CS300_3>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <11/11/21>
End Header --------------------------------------------------------*/
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
