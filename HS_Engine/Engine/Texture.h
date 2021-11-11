#pragma once
#include <limits>
#include <string>

namespace HS_Engine
{
	class Texture
	{
	public:
		Texture(std::string path,unsigned texture_idx);
		~Texture();
		void SetTextureIndex(unsigned index);
		void Bind();
		void UnBind();
		unsigned GetTextureIdx() const;
	private:
		unsigned int m_texture_ID = std::numeric_limits<unsigned int>::max();
		unsigned int m_texture_idx = 0;
	};


	
}
