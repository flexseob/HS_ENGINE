#pragma once
#include <limits>
#include <string>

namespace HS_Engine
{
	class Texture
	{
	public:
		Texture(std::string path,unsigned texture_idx);
		Texture(std::string path,unsigned texture_idx, bool is_skybox);
		Texture(unsigned texture_ID , unsigned int texture_idx);
		Texture(unsigned texture_idx, int width, int height);
		~Texture();
		void SetTextureIndex(unsigned index);
		void Bind();
		void UnBind();
		unsigned GetTextureIdx() const;
		unsigned int GetTextureID() const { return m_texture_ID; }
	private:
		unsigned int m_texture_ID = std::numeric_limits<unsigned int>::max();
		unsigned int m_texture_idx = 0;
	};

	


	
}
