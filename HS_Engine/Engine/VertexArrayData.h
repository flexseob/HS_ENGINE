
#pragma once
#include <memory>
#include <vector>
#include "Buffer.h"

namespace HS_Engine
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertex_buffer);
		void AddIndexBuffer(std::shared_ptr<IndexBuffer> index_buffer);

		void ClearVertexBuffer();
		void ClearIndexBuffer();
		std::shared_ptr<IndexBuffer> GetIndexBuffers() const { return m_IndexBuffers; }
	private:
		unsigned int m_VertexArrayID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffers;
	};
}

