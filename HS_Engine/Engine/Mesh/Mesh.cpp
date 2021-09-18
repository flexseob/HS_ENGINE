#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Buffer.h"
#include "../Render.h"
#include "../Shader.h"

namespace HS_Engine
{
	void Mesh::ClearData()
	{
		m_Vertexs.clear();
		m_Normals.clear();
		m_TexCoords.clear();
		m_Faces.clear();
	
	}

	void Mesh::SetIndexBufferCount(unsigned count)
	{
		m_Indices = count;
	}

	void Mesh::PreRender()
	{

		
	}

	void Mesh::Render()
	{
		m_VertexArray->Bind();


		m_VertexArray->Unbind();
	}

	void Mesh::PostRender()
	{
	}

	void Mesh::InitBuffer()
	{
		m_VertexArray = std::make_shared<VertexArray>();

		std::shared_ptr<VertexBuffer> vertex_buffer = std::make_shared<VertexBuffer>(m_Vertexs.data(), m_Vertexs.size() * sizeof(float));
		vertex_buffer->DescribeData({ { Float3,0 } });
		m_VertexArray->AddVertexBuffer(vertex_buffer);
		
		std::shared_ptr<VertexBuffer> normal_buffer = std::make_shared<VertexBuffer>(m_Normals.data(), m_Normals.size() * sizeof(float));
		normal_buffer->DescribeData({ {Float3, 1} });
		m_VertexArray->AddVertexBuffer(normal_buffer);

		if(!m_TexCoords.empty())
		{
			std::shared_ptr<VertexBuffer> texcoord_buffer = std::make_shared<VertexBuffer>(m_TexCoords.data(), m_TexCoords.size() * sizeof(float));
			texcoord_buffer->DescribeData({ { Float2, 2 } });
			m_VertexArray->AddVertexBuffer(texcoord_buffer);
		}

		std::shared_ptr<IndexBuffer> index_buffer = std::make_shared<IndexBuffer>(reinterpret_cast<unsigned*>(m_Faces.data()),m_Faces.size());
		m_VertexArray->AddIndexBuffer(index_buffer);
		
	}
}

