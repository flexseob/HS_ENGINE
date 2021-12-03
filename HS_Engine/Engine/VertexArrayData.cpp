/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#include "VertexArrayData.h"
#include "Buffer.h"

namespace  HS_Engine
{
	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	VertexArray::~VertexArray()
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_VertexArrayID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffer)
	{
		glBindVertexArray(m_VertexArrayID);
		vertex_buffer->Bind();

		const auto& described_Data = vertex_buffer->GetDescribedData();
		for (const auto& element : described_Data)
		{
			glVertexAttribPointer
			(
				element.m_LayoutLocation,
				element.m_ElementCount,
				element.ShaderDataTypeToOpenGLBaseType(),
				element.m_Normalize ? GL_TRUE : GL_FALSE,
				described_Data.GetStride(),
				reinterpret_cast<const void*>(element.m_Offset)
			);
			glEnableVertexAttribArray(element.m_LayoutLocation);
		}
		glBindVertexArray(0);
		vertex_buffer->UnBind();
		m_VertexBuffers.push_back(vertex_buffer);
	}

	



	void VertexArray::AddIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer)
	{
		if (index_buffer->GetIndexID() != std::numeric_limits<unsigned int>::max())
		{
			glBindVertexArray(m_VertexArrayID);
		index_buffer->Bind();

		m_IndexBuffers = index_buffer;
		glBindVertexArray(0);
		index_buffer->UnBind();
		}
		else
		{
			m_IndexBuffers = index_buffer;
		}
	}

	void VertexArray::AddUniformBuffer(std::shared_ptr<UniformBuffer> uniformbuffer)
	{
		m_uniform_buffers.push_back(uniformbuffer);
	}

	void VertexArray::ClearVertexBuffer()
	{
		glBindVertexArray(m_VertexArrayID);
		for(auto& vertex_buffer : m_VertexBuffers)
		{
			const auto& described_Data = vertex_buffer->GetDescribedData();
			for (const auto& element : described_Data)
			{
				glDisableVertexAttribArray(element.m_LayoutLocation);
			}
		}
		glBindVertexArray(0);
		m_VertexBuffers.clear();
	}

	void VertexArray::ClearIndexBuffer()
	{
		m_IndexBuffers.reset();
	}
}
