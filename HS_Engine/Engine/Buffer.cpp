/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */

#include "Buffer.h"

#include <iostream>
#include <stb_image.h>
namespace HS_Engine
{

	DataAndLayoutLocation::DataAndLayoutLocation(DataType data, unsigned layout_location, bool normalize)
		:m_DataType(data), m_LayoutLocation(layout_location), m_Size(0), m_ElementCount(0), m_Offset(0), m_Normalize(normalize)
	{
		switch (data)
		{
		case DataType::Bool:	m_Size = 4;			 break;
		case DataType::Int:		m_Size = 4;			 break;
		case DataType::Int2:	m_Size = 4 * 2;		 break;
		case DataType::Int3:	m_Size = 4 * 3;		 break;
		case DataType::Int4:	m_Size = 4 * 4;		 break;
		case DataType::Float:   m_Size = 4;			 break;
		case DataType::Float2:	m_Size = 4 * 2;		 break;
		case DataType::Float3:	m_Size = 4 * 3;		 break;
		case DataType::Float4:	m_Size = 4 * 4;		 break;
		case DataType::Mat3:	m_Size = 4 * 3 * 3;	 break;
		case DataType::Mat4:	m_Size = 4 * 4 * 4;	 break;
		}

		//get element count 
		switch (data)
		{
		case DataType::Bool:	m_ElementCount = 1; 	break;
		case DataType::Int:		m_ElementCount = 1;		break;
		case DataType::Int2:	m_ElementCount = 2;		break;
		case DataType::Int3:	m_ElementCount = 3;		break;
		case DataType::Int4:	m_ElementCount = 4;		break;
		case DataType::Float:	m_ElementCount = 1;		break;
		case DataType::Float2:	m_ElementCount = 2;		break;
		case DataType::Float3:	m_ElementCount = 3;		break;
		case DataType::Float4:	m_ElementCount = 4;		break;
		case DataType::Mat3:	m_ElementCount = 3 * 3;	break;
		case DataType::Mat4:	m_ElementCount = 4 * 4;	break;
		}
	}

	GLenum DataAndLayoutLocation::ShaderDataTypeToOpenGLBaseType() const
	{
		switch (m_DataType)
		{
		case DataType::Bool:		return GL_BOOL;
		case DataType::Int:			return GL_INT;
		case DataType::Int2:		return GL_INT;
		case DataType::Int3:		return GL_INT;
		case DataType::Int4:		return GL_INT;
		case DataType::Float:		return GL_FLOAT;
		case DataType::Float2:		return GL_FLOAT;
		case DataType::Float3:		return GL_FLOAT;
		case DataType::Float4:		return GL_FLOAT;
		case DataType::Mat3:		return GL_FLOAT;
		case DataType::Mat4:		return GL_FLOAT;
		}
		return 0;
	}


	DescribeData::DescribeData(const std::initializer_list<DataAndLayoutLocation>& described_data)
		:m_DescribedData(described_data)
	{
		//compute offset and stride
		unsigned int offset = 0;
		m_Stride = 0;
		for (auto& data : m_DescribedData)
		{
			data.m_Offset = offset;
			offset += data.m_Size;
			m_Stride += data.m_Size;
		}
	}

	VertexBuffer::VertexBuffer(unsigned size)
	{
		glGenBuffers(1, &m_Render_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_Render_ID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(float* vertices, unsigned size)
	{
		glGenBuffers(1, &m_Render_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_Render_ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_Render_ID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Render_ID);
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::BufferData(const void* data, unsigned size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Render_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	IndexBuffer::IndexBuffer(unsigned* indices, unsigned count)
		:m_Count(count)
	{
		glGenBuffers(1, &m_Render_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Render_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * count, indices, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(unsigned count) : m_Count(count)
	{
		
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Render_ID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Render_ID);
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	UniformBuffer::UniformBuffer(const void* buffer, unsigned size, unsigned index)
	{
		glGenBuffers(1, &m_UniformBuffer_ID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffer_ID);
		glBufferData(GL_UNIFORM_BUFFER, size, buffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, index,m_UniformBuffer_ID);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_UniformBuffer_ID);
	}

	void UniformBuffer::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffer_ID);
	}

	void UniformBuffer::BufferData(const void* data, unsigned size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffer_ID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	}


	void UniformBuffer::UnBind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	FrameBuffer::FrameBuffer(int initTexturewidth, int initTextureheight)
	{
		glGenFramebuffers(1,&m_FrameBuffer_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_ID);
		
		glGenRenderbuffers(1,&m_DepthRenderBuffer_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer_ID);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT, initTexturewidth, initTextureheight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteBuffers(1, &m_FrameBuffer_ID);
		delete m_Texture;
		m_Texture = nullptr;
	}

	void FrameBuffer::Bind() const
	{

		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer_ID);
	}

	void FrameBuffer::UnBind() const
	{
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Init()
	{

	}

	void FrameBuffer::CreateFrameTexture(Texture* texture)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureID(), 0);

	}


	Texture* FrameBuffer::GetFrameTexture() const
	{
		return m_Texture;
	}
}
