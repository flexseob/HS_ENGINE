/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : VertexArrayData.h
Purpose : For setting VertexArray class
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Buffer.h"
#include <unordered_map>

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
		void AddUniformBuffer(std::shared_ptr<UniformBuffer> uniformbuffer);
		
		
		void ClearVertexBuffer();
		void ClearIndexBuffer();
        std::shared_ptr<IndexBuffer> GetIndexBuffers() const { return m_IndexBuffers; }
	private:
		unsigned int m_VertexArrayID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffers;
		std::vector<std::shared_ptr<UniformBuffer>> m_uniform_buffers;
	};
}

