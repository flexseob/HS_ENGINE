/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : UniformBlock.h
Purpose : UniformBlock header file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <10 / 15 / 21>
End Header-------------------------------------------------------- */
#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>


namespace HS_Engine
{
	class UniformBlock
	{
	public:
		UniformBlock(unsigned ProgramID);
		~UniformBlock();
		void AddUniformName(const std::string& name);
		void AddUniformNameData(const std::vector<std::string>&& data);
		void InitBlockbuffer(int blocksize);
		void BindUniformBlock();
		void UnBindUniformBlock();
	
	private:
		unsigned int m_ProgramID;
		std::vector<std::string> m_blockNames;
		std::vector<unsigned> m_indices;
		std::vector<int> m_offset;
		GLubyte* m_blockbuffer = nullptr;
	};
	


}
