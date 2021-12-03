/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : TextureManager.h
Purpose : TextureManager Header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <10 / 15 / 21>
End Header-------------------------------------------------------- */

#pragma once
#include <unordered_map>

namespace HS_Engine
{
	class Texture;

	class TextureManager
	{
	public:
		TextureManager() = default;
		~TextureManager();

		void AddTexture(std::string texturename, std::string path, unsigned idx);
		void AddTexture(std::string texturename, Texture* texture);
		void AddTextureInternal(std::string texturename, Texture* texture);
		void AddTextureSkyBox(std::string texturename, std::string path, unsigned idx);
		void DeleteAllTexture();
		void DeleteTexture(std::string path);
		Texture* GetTexture(std::string texturename);
		

	
	private:
		std::unordered_map<std::string, Texture*> m_Textures;		
	};

	
}
