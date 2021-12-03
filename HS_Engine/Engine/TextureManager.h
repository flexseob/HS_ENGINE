/* Start Header------------------------------------------------------ -
Hoseob Jeong
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
