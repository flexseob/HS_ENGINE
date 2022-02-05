/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#include "TextureManager.h"

#include <iostream>

#include "Texture.h"

namespace HS_Engine
{
	TextureManager::~TextureManager()
	{
		DeleteAllTexture();
	}

	void TextureManager::AddTexture(std::string texturename, std::string path, unsigned idx)
	{
		auto texture_iter = m_Textures.find(texturename);

		if(texture_iter == m_Textures.end())
		{
			Texture* new_texture = new Texture(path, idx);
			m_Textures.insert({ texturename, new_texture });
		}
		else
		{
			std::cout << "Texture name already exist! : " + texturename << std::endl;
		}
		
	}

	void TextureManager::AddTexture(std::string texturename, Texture* texture)
	{
		auto texture_iter = m_Textures.find(texturename);

		if(texture_iter == m_Textures.end())
		{
			m_Textures.insert({ texturename, texture });
		}
		else
		{
			std::cout << "Texture name already exist! : " + texturename << std::endl;
			std::cout << "This Texture deleted : " + texturename << std::endl;
			delete texture;
		}
	}

	void TextureManager::AddTextureInternal(std::string texturename, Texture* texture)
	{
		auto texture_iter = m_Textures.find(texturename);

		if (texture_iter == m_Textures.end())
		{
			m_Textures.insert({ texturename, texture });
		}
		else
		{
			delete texture;
		}
		
	}

	void TextureManager::AddTextureSkyBox(std::string texturename, std::string path, unsigned idx)
	{
		auto texture_iter = m_Textures.find(texturename);

		if (texture_iter == m_Textures.end())
		{
			Texture* new_texture = new Texture(path, idx, true);
			m_Textures.insert({ texturename, new_texture });
		}
		
	}


	void TextureManager::DeleteAllTexture()
	{
		for(auto& texture : m_Textures)
		{
			delete texture.second;
			texture.second = nullptr;
		}
		m_Textures.clear();
	}

	void TextureManager::DeleteTexture(std::string path)
	{
		auto texture_iter = m_Textures.find(path);
		if(texture_iter == m_Textures.end())
		{
			std::cout << "Cannot delete the texture" << std::endl;
		}
		else
		{
			delete texture_iter->second;
			texture_iter->second = nullptr;
			m_Textures.erase(texture_iter);
		}
	}

	Texture* TextureManager::GetTexture(std::string texturename)
	{
		auto texture_iter = m_Textures.find(texturename);

		if(texture_iter == m_Textures.end())
		{
			std::cout << " Cannot Find the Texture : " << texturename << std::endl;
			return nullptr;
		}

		return texture_iter->second;
	}
}
