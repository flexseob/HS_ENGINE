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

	void Mesh::PreRender()
	{
		m_ModelMat = glm::mat4(1.0f);
		m_ModelMat = glm::translate(m_ModelMat, m_Position);
		m_ModelMat = glm::scale(m_ModelMat, m_Scale);
		m_ModelMat = glm::rotate(m_ModelMat,glm::radians(m_angle), m_RotateAxis);
	}

	void Mesh::Render()
	{
		m_VertexArray->Bind();
		Render::DrawIndexed(m_VertexArray);
		m_VertexArray->Unbind();
	}

	void Mesh::PostRender()
	{
		
	}

	void Mesh::SetIndexBufferCount(unsigned count)
	{
		m_Indices = count;
	}

	void Mesh::SetPosition(glm::vec3 position)
	{
		m_Position = position;
		
	}

	void Mesh::SetScale(glm::vec3 scale)
	{
		m_Scale = scale;
		
	}

	void Mesh::SetRotation(float angle, glm::vec3 axis)
	{
		m_angle = angle;
		m_RotateAxis = axis;
		
	}

	void Mesh::SetModelMatrixtoShader(std::shared_ptr<Shader> shader)
	{
		shader->BindUniformVariable("model", m_ModelMat);

		
	}

	void Mesh::SetObjectColor(glm::vec3 color)
	{
		m_ObjectColor = color;
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

