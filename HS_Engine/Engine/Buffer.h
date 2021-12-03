/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header-------------------------------------------------------- */
#pragma once
#include <initializer_list>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "Texture.h"

namespace HS_Engine
{
	enum class DataType
	{
		None = 0, Bool, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4,
	};

	struct DataAndLayoutLocation
	{
		DataType m_DataType;
		unsigned int m_LayoutLocation;
		unsigned int m_Size;
		unsigned int m_ElementCount;
		unsigned int m_Offset;
		bool m_Normalize;

		DataAndLayoutLocation() = delete;
		DataAndLayoutLocation(DataType data, unsigned int layout_location, bool normalize = false);
		GLenum ShaderDataTypeToOpenGLBaseType() const;
	};

	class DescribeData
	{
	public:
		DescribeData() {}
		DescribeData(const std::initializer_list<DataAndLayoutLocation>& described_data);

		unsigned int GetStride() const { return m_Stride; }

		std::vector<DataAndLayoutLocation>::iterator begin() { return m_DescribedData.begin(); }
		std::vector<DataAndLayoutLocation>::iterator end() { return m_DescribedData.end(); }
		std::vector<DataAndLayoutLocation>::const_iterator begin() const { return m_DescribedData.cbegin(); }
		std::vector<DataAndLayoutLocation>::const_iterator end() const { return m_DescribedData.cend(); }
	private:
		std::vector<DataAndLayoutLocation> m_DescribedData;
		unsigned int m_Stride = std::numeric_limits<unsigned int>::max();
	};
	
	class VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size);
		VertexBuffer(float* vertices, unsigned size);
		void Bind() const;
		void BufferData(const void* data, unsigned size);
		void UnBind() const;
		~VertexBuffer();

		void DescribeData(const HS_Engine::DescribeData& described_Data) { m_Describe_Data = described_Data; }
		const HS_Engine::DescribeData& GetDescribedData() const { return m_Describe_Data; }
		
	private:
		unsigned int m_Render_ID = std::numeric_limits<unsigned int>::max();
		HS_Engine::DescribeData m_Describe_Data;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned int* indices, unsigned int count);
		IndexBuffer(unsigned int count);
		~IndexBuffer();
		void Bind() const;
		void UnBind() const;
		unsigned int GetIndexID()const { return m_Render_ID; }
		unsigned int GetCount() const { return  m_Count; }

	private:
		unsigned int m_Render_ID = std::numeric_limits<unsigned int>::max();;
		unsigned int m_Count;
	};

	class UniformBuffer
	{
	public:
		UniformBuffer(const void* buffer, unsigned size,unsigned index);
		~UniformBuffer();
		void Bind() const;
		void BufferData(const void* buffer, unsigned size);
		void UnBind() const;
		void DescribeData(const HS_Engine::DescribeData& described_Data) { m_Describe_Data = described_Data; }
		const HS_Engine::DescribeData& GetDescribedData() const { return m_Describe_Data; }
	private:
		unsigned int m_UniformBuffer_ID = std::numeric_limits<unsigned int>::max();
		HS_Engine::DescribeData m_Describe_Data;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(int initTexturewidth, int initTextureheight);
		~FrameBuffer();
		void Bind() const;
		void UnBind() const;
		void Init();
		void CreateFrameTexture(Texture* texture);
		void DeleteBuffer()const;
		Texture* GetFrameTexture() const;
		
		
	private:
		unsigned int m_FrameBuffer_ID = std::numeric_limits<unsigned int>::max();
		unsigned int m_DepthRenderBuffer_ID = std::numeric_limits<unsigned int>::max();
		Texture* m_Texture = nullptr;
		HS_Engine::DescribeData m_Describe_Data;
	};


};


