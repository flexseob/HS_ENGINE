/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#include "Texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION    
#include <stb_image.h>
#include <GL/glew.h>

namespace HS_Engine
{
	Texture::Texture(std::string path, unsigned texture_idx)
	{
		if (texture_idx <= 31)
			m_texture_idx = texture_idx;
		else
		{
			std::cout << "Texture index could not be larger than 31!!" << std::endl;
		}
		glGenTextures(1, &m_texture_ID);
		int width;
		int height;
		int nrComponents;
		glActiveTexture(GL_TEXTURE0 + m_texture_idx);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		
		if (data)
		{
			GLenum format =0;
			if (nrComponents == 1)
			{
				format = GL_RED;
			}
			else if (nrComponents == 3)
			{
				format = GL_RGB;
			}
			else if (nrComponents == 4)
			{
				format = GL_RGBA;
			}
			glBindTexture(GL_TEXTURE_2D, m_texture_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Texture failed to load : " << path << std::endl;
		}
		stbi_image_free(data);
	}

	Texture::Texture(std::string path, unsigned texture_idx, bool is_skybox)
	{
		if (texture_idx <= 31)
			m_texture_idx = texture_idx;
		else
		{
			std::cout << "Texture index could not be larger than 31!!" << std::endl;
		}
		glGenTextures(1, &m_texture_ID);
		int width;
		int height;
		int nrComponents;
		glActiveTexture(GL_TEXTURE0 + m_texture_idx);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format = 0;
			if (nrComponents == 1)
			{
				format = GL_RED;
			}
			else if (nrComponents == 3)
			{
				format = GL_RGB;
			}
			else if (nrComponents == 4)
			{
				format = GL_RGBA;
			}
			glBindTexture(GL_TEXTURE_2D, m_texture_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Texture failed to load : " << path << std::endl;
		}
		stbi_image_free(data);

		
	}

	Texture::Texture(unsigned texture_ID, unsigned texture_idx)
	{
		if (texture_idx <= 31)
			m_texture_idx = texture_idx;
		else
		{
			std::cout << "Texture index could not be larger than 31!!" << std::endl;
		}
		m_texture_ID = texture_ID;
	}

	Texture::Texture(unsigned texture_idx,int width, int height)
	{
		if (texture_idx <= 31)
			m_texture_idx = texture_idx;
		else
		{
			std::cout << "Texture index could not be larger than 31!!" << std::endl;
		}
		glGenTextures(1, &m_texture_ID);
		glBindTexture(GL_TEXTURE_2D, m_texture_ID);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	Texture::Texture(unsigned texture_idx, int width, int height, unsigned internalformat)
	{
		if (texture_idx <= 31)
			m_texture_idx = texture_idx;
		else
		{
			std::cout << "Texture index could not be larger than 31!!" << std::endl;
		}
		glGenTextures(1, &m_texture_ID);
		glBindTexture(GL_TEXTURE_2D, m_texture_ID);
		glTexImage2D(
			GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		
	}

	Texture::Texture(unsigned texture_idx, int width, int height, unsigned internalformat, int depth)
	{
		if (texture_idx <= 31)
			m_texture_idx = texture_idx;
		else
		{
			std::cout << "Texture index could not be larger than 31!!" << std::endl;
		}
		glGenTextures(1, &m_texture_ID);
		glBindTexture(GL_TEXTURE_2D, m_texture_ID);
		glTexImage2D(
			GL_TEXTURE_2D, 0, internalformat, width, height, 0, depth, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_texture_ID);
	}

	void Texture::SetTextureIndex(unsigned index)
	{
		m_texture_idx = index;
	}

	void Texture::Bind()
	{
		glActiveTexture(GL_TEXTURE0 + m_texture_idx);
		glBindTexture(GL_TEXTURE_2D, m_texture_ID);
	}

	void Texture::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned Texture::GetTextureIdx() const
	{
		return m_texture_idx;
	}
}
