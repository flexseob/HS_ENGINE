#pragma once
#include <memory>
#include <vector>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include "../VertexArrayData.h"
namespace HS_Engine
{
	class Shader;
	class ObjectVertex;


	class Mesh
	{
	public:
		~Mesh() = default;
		void InitBuffer();
		void ClearData();
		void SetIndexBufferCount(unsigned int count);
		void PreRender();
		void Render();
		void PostRender();
		
		std::vector<float> m_Vertexs;
		std::vector<float> m_Normals;
		std::vector<float> m_TexCoords;
		std::vector<unsigned int> m_Faces;

	private:

		unsigned int m_Indices;
		std::shared_ptr<VertexArray> m_VertexArray;

	};



	
}
