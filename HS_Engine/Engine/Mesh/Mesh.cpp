#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../Buffer.h"
#include "../Render.h"
#include "../Shader.h"

namespace HS_Engine
{
	void Mesh::ClearDataByVertexNormal()
	{
		m_MeshDataByVertexNormal.m_Vertexs.clear();
		m_MeshDataByVertexNormal.m_Normals.clear();
		m_MeshDataByVertexNormal.m_TexCoords.clear();
		m_MeshDataByVertexNormal.m_Faces.clear();

		
	}

	void Mesh::ClearDataByFaceNormal()
	{
		m_MeshDataByFaceNormal.m_Vertexs.clear();
		m_MeshDataByFaceNormal.m_Normals.clear();
		m_MeshDataByFaceNormal.m_TexCoords.clear();
		m_MeshDataByFaceNormal.m_Faces.clear();
		
	}

	void Mesh::SetIndexBufferCount(unsigned count)
	{
		m_Indices = count;
	}

	void Mesh::PreRender()
	{

		m_CheckFlag = true;
		
	}

	void Mesh::Render(E_RenderTypes rendertype)
	{
		m_VertexArray->Bind();
		switch (rendertype)
		{
		case E_RenderTypes::LINES:
			Render::DrawLine(m_VertexArray);
			break;
		case E_RenderTypes::TRIANGLES:
			Render::DrawIndexed(m_VertexArray);
			break;
		case E_RenderTypes::TRIANGELS_STRIP:

			break;
		}
		m_VertexArray->Unbind();
		

	}

	void Mesh::DebugRender()
	{
		if (m_VertexArray_Debug.get() != nullptr)
		{
			m_VertexArray_Debug->Bind();
			Render::DrawLine(m_VertexArray_Debug);
			m_VertexArray_Debug->Unbind();
		}
	}


	void Mesh::PostRender([[maybe_unused]]double dt)
	{

	}

	E_RenderTypes Mesh::GetRenderType()
	{
		return m_RenderType;
	}

	std::shared_ptr<VertexArray> Mesh::GetVertexArray()
	{
		return m_VertexArray;
	}

	void Mesh::SetSharedPtrVertexArray()
	{
		m_VertexArray = std::make_shared<VertexArray>();
	}

	void Mesh::ChangeRenderNormalType(bool IsFaceNormal)
	{
		if (m_CheckFlag == false)
		{
			if (IsFaceNormal)
			{
				m_VertexArray->ClearVertexBuffer();
				for (const auto& buffer : m_BufferForFaceNormal)
				{
					m_VertexArray->AddVertexBuffer(buffer);
				}
				//m_VertexArray->Unbind();
				m_VertexArray->AddIndexBuffer(m_IndexBufferForFaceNormal);
				SetNormalTypes(E_NormalTypes::FACE);
				m_CheckFlag = true;
			}
			else
			{
				m_VertexArray->ClearVertexBuffer();
				for (const auto& buffer : m_BufferForVertexNormal)
				{
					m_VertexArray->AddVertexBuffer(buffer);
				}
				//m_VertexArray->Unbind();
				m_VertexArray->AddIndexBuffer(m_IndexBufferForVertexNormal);
				SetNormalTypes(E_NormalTypes::VERTEX);
				m_CheckFlag = true;
			}
		}

	}

	void Mesh::ChangeDebugNormalType(bool IsFaceNormal)
	{
		if (m_CheckFlag == false)
		{
			if (IsFaceNormal)
			{
				m_VertexArray_Debug->ClearVertexBuffer();
			    m_VertexArray_Debug->AddVertexBuffer(m_BufferDisplayFaceNormalDebug);
				m_VertexArray_Debug->AddIndexBuffer(m_IndexBufferDisplayFaceNormalDebug);
				m_VertexArray_Debug->Unbind();
				m_CheckFlag = true;
			}
			else
			{
				m_VertexArray_Debug->ClearVertexBuffer();
				m_VertexArray_Debug->AddVertexBuffer(m_BufferDisplayVtxNormalDebug);
				m_VertexArray_Debug->AddIndexBuffer(m_IndexBufferDisplayVtxNormalDebug);
				m_VertexArray_Debug->Unbind();
				m_CheckFlag = true;
			}
		}

		
	}

	void Mesh::SetCheckFlag()
	{
		m_CheckFlag = false;
	}

	void Mesh::SetRenderType(E_RenderTypes types)
	{
		m_RenderType = types;
	}

	void Mesh::SetDebugMangitue(float magnitue)
	{
		m_DebugMagntite = magnitue;
	}

	const float Mesh::GetDebugMagnitue() const
	{
		return m_DebugMagntite;
	}

	E_NormalTypes Mesh::GetNormalType() const
	{
		return m_NormalType;
	}

	void Mesh::SetNormalTypes(E_NormalTypes normal_types)
	{
		m_NormalType = normal_types;
	}


	void Mesh::InitBuffer()
	{
		m_VertexArray = std::make_shared<VertexArray>();

		//MeshByVertexNormal
		std::shared_ptr<VertexBuffer> vertex_buffer = std::make_shared<VertexBuffer>(m_MeshDataByVertexNormal.m_Vertexs.data(), m_MeshDataByVertexNormal.m_Vertexs.size() * sizeof(float));
		vertex_buffer->DescribeData({ { DataType::Float3,0 } });
		m_VertexArray->AddVertexBuffer(vertex_buffer);
		m_BufferForVertexNormal.push_back(vertex_buffer);
		
		if (!m_MeshDataByVertexNormal.m_Normals.empty())
		{
			std::shared_ptr<VertexBuffer> normal_buffer = std::make_shared<VertexBuffer>(m_MeshDataByVertexNormal.m_Normals.data(), m_MeshDataByVertexNormal.m_Normals.size() * sizeof(float));
			normal_buffer->DescribeData({ {Float3, 1} });
			m_VertexArray->AddVertexBuffer(normal_buffer);
			m_BufferForVertexNormal.push_back(normal_buffer);	
		}
		
		if (GetRenderType() != E_RenderTypes::LINES)
		{
			std::shared_ptr<IndexBuffer> index_buffer = std::make_shared<IndexBuffer>(reinterpret_cast<unsigned*>(m_MeshDataByVertexNormal.m_Faces.data()), m_MeshDataByVertexNormal.m_Faces.size());
			m_VertexArray->AddIndexBuffer(index_buffer);
			m_IndexBufferForVertexNormal = index_buffer;

			std::shared_ptr<IndexBuffer> index_buffer1 = std::make_shared<IndexBuffer>(reinterpret_cast<unsigned*>(m_MeshDataByFaceNormal.m_Faces.data()), m_MeshDataByFaceNormal.m_Faces.size());
			m_IndexBufferForFaceNormal = index_buffer1;
		}
		else
		{
			std::shared_ptr<IndexBuffer> index_buffer = std::make_shared<IndexBuffer>(m_MeshDataByVertexNormal.m_Vertexs.size());
			m_VertexArray->AddIndexBuffer(index_buffer);
			m_IndexBufferForVertexNormal = index_buffer;
		}

		
		//MeshByFaceNormal
		if (!m_MeshDataByFaceNormal.m_Vertexs.empty())
		{
			std::shared_ptr<VertexBuffer> vertex_buffer1 = std::make_shared<VertexBuffer>(m_MeshDataByFaceNormal.m_Vertexs.data(), m_MeshDataByFaceNormal.m_Vertexs.size() * sizeof(float));
			vertex_buffer1->DescribeData({ { Float3,0 } });
			m_BufferForFaceNormal.push_back(vertex_buffer1);
		}

		if (!m_MeshDataByFaceNormal.m_Normals.empty())
		{
			std::shared_ptr<VertexBuffer> normal_buffer1 = std::make_shared<VertexBuffer>(m_MeshDataByFaceNormal.m_Normals.data(), m_MeshDataByFaceNormal.m_Normals.size() * sizeof(float));
			normal_buffer1->DescribeData({ {Float3, 1} });
			m_BufferForFaceNormal.push_back(normal_buffer1);
		}
		
		if(!m_MeshDataByVertexNormal.m_TexCoords.empty())
		{
			std::shared_ptr<VertexBuffer> texcoord_buffer = std::make_shared<VertexBuffer>(m_MeshDataByVertexNormal.m_TexCoords.data(), m_MeshDataByVertexNormal.m_TexCoords.size() * sizeof(float));
			texcoord_buffer->DescribeData({ { Float2, 2 } });
			m_VertexArray->AddVertexBuffer(texcoord_buffer);
			m_BufferForVertexNormal.push_back(texcoord_buffer);
		}
		if(!m_MeshDataByFaceNormal.m_TexCoords.empty())
		{
			std::shared_ptr<VertexBuffer> texcoord_buffer1 = std::make_shared<VertexBuffer>(m_MeshDataByFaceNormal.m_TexCoords.data(), m_MeshDataByFaceNormal.m_TexCoords.size() * sizeof(float));
			texcoord_buffer1->DescribeData({ { Float2, 2 } });
			m_BufferForFaceNormal.push_back(texcoord_buffer1);
		}
		


		//Debug Vertex normal data
		
		if (!m_MeshDataByVertexNormal.m_DebugNormalData.m_Vertexs.empty())
		{
			m_VertexArray_Debug = std::make_shared<VertexArray>();
			std::shared_ptr<VertexBuffer> vertex_buffer_debug_vertexnormal = std::make_shared<VertexBuffer>(m_MeshDataByVertexNormal.m_DebugNormalData.m_Vertexs.data(),
				m_MeshDataByVertexNormal.m_DebugNormalData.m_Vertexs.size() * sizeof(float));
			vertex_buffer_debug_vertexnormal->DescribeData({ { Float3,0 } });
			m_VertexArray_Debug->AddVertexBuffer(vertex_buffer_debug_vertexnormal);
			m_BufferDisplayVtxNormalDebug = vertex_buffer_debug_vertexnormal;

			std::shared_ptr<IndexBuffer> index_buffer_debug_vtx = std::make_shared<IndexBuffer>(m_MeshDataByVertexNormal.m_DebugNormalData.m_Vertexs.size());
			m_VertexArray_Debug->AddIndexBuffer(index_buffer_debug_vtx);
			m_IndexBufferDisplayVtxNormalDebug = index_buffer_debug_vtx;
		}

		//Debug face normal data
		if (!m_MeshDataByFaceNormal.m_DebugNormalData.m_Vertexs.empty())
		{
			std::shared_ptr<VertexBuffer> vertex_buffer_debug_facenormal = std::make_shared<VertexBuffer>(m_MeshDataByFaceNormal.m_DebugNormalData.m_Vertexs.data(),
				m_MeshDataByFaceNormal.m_DebugNormalData.m_Vertexs.size() * sizeof(float));
			vertex_buffer_debug_facenormal->DescribeData({ {Float3, 0} });
			m_BufferDisplayFaceNormalDebug = vertex_buffer_debug_facenormal;

			std::shared_ptr<IndexBuffer> index_buffer_debug_face = std::make_shared<IndexBuffer>(m_MeshDataByFaceNormal.m_DebugNormalData.m_Vertexs.size());
			m_IndexBufferDisplayFaceNormalDebug = index_buffer_debug_face;
		}

	/*	ClearDataByFaceNormal();
		ClearDataByVertexNormal();*/
	}
}

