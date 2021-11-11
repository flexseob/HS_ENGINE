/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : UniformBlock.cpp
Purpose : UniformBlock Source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <10 / 15 / 21>
End Header-------------------------------------------------------- */

#include "UniformBlock.h"

namespace HS_Engine
{
	UniformBlock::UniformBlock(unsigned ProgramID) : m_ProgramID(ProgramID)
	{

	}

	UniformBlock::~UniformBlock()
	{
		delete m_blockbuffer;
	}

	void UniformBlock::AddUniformName(const std::string& name)
	{
		m_blockNames.push_back(name);
	}

	void UniformBlock::AddUniformNameData(const std::vector<std::string>&& data)
	{
		m_blockNames = data;
	}

	void UniformBlock::InitBlockbuffer(int blocksize)
	{
		m_blockbuffer = new GLubyte[blocksize];
	}

	void UniformBlock::BindUniformBlock()
	{
		const GLsizei size = static_cast<int>(m_blockNames.size());
		std::vector<const char*> c_styleString(size);
		for(std::string item : m_blockNames)
		{
			c_styleString.push_back(item.c_str());
		}
		const char* const* c_stylePtr = c_styleString.data();

		m_indices.resize(size);
		m_offset.resize(size);
		glGetUniformIndices(m_ProgramID, size, c_stylePtr, m_indices.data());
		glGetActiveUniformsiv(m_ProgramID, size, m_indices.data(), GL_UNIFORM_OFFSET, m_offset.data());
	}

	void UniformBlock::UnBindUniformBlock()
	{
		
	}
}
