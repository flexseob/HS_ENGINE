/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
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
