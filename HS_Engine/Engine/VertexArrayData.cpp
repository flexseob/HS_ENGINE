

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
			glEnableVertexAttribArray(element.m_LayoutLocation);
			glVertexAttribPointer
			(
				element.m_LayoutLocation,
				element.m_ElementCount,
				element.ShaderDataTypeToOpenGLBaseType(),
				element.m_Normalize ? GL_TRUE : GL_FALSE,
				described_Data.GetStride(),
				reinterpret_cast<const void*>(element.m_Offset)
			);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		m_VertexBuffers.push_back(vertex_buffer);
	}

	void VertexArray::AddIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer)
	{
		glBindVertexArray(m_VertexArrayID);
		index_buffer->Bind();

		m_IndexBuffers = index_buffer;
	}

	void VertexArray::ClearVertexBuffer()
	{
		for(auto& a : m_VertexBuffers)
		{
			a.reset();
		}
		m_VertexBuffers.clear();
	}

	void VertexArray::ClearIndexBuffer()
	{
		m_IndexBuffers.reset();
	}
}
