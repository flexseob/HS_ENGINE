
#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include "../VertexArrayData.h"
#include "../Types.h"
namespace HS_Engine
{
	class Shader;
	class ObjectVertex;


	class Mesh
	{
	public:
		//~Mesh() = default;
		virtual void InitBuffer();
		void ClearDataByVertexNormal();
		void ClearDataByFaceNormal();
		virtual void SetIndexBufferCount(unsigned int count);
		void PreRender();
		void Render(E_RenderTypes rendertype);
		void DebugRender();
		void PostRender(double dt);
		
		E_RenderTypes GetRenderType();
		std::shared_ptr<VertexArray> GetVertexArray();
		void SetSharedPtrVertexArray();
		void ChangeRenderNormalType(bool IsFaceNormal);
		void ChangeDebugNormalType(bool IsFaceNormal);
		void SetCheckFlag();
		void SetRenderType(E_RenderTypes types);
		void SetDebugMangitue(float magnitue);
		void SetMeshName(std::string name);

		std::string GetMeshName() const
		{
			return m_Meshname;
		}

		const float GetDebugMagnitue() const;
		struct MeshData
		{
			std::vector<float> m_Vertexs;
			std::vector<float> m_Normals;
			std::vector<float> m_TexCoords;
			std::vector<unsigned int> m_Faces;
			struct DebugNormalData
			{
				std::vector<float> m_Vertexs;
			};
			DebugNormalData m_DebugNormalData;
		};

		MeshData m_MeshDataByFaceNormal;
		MeshData m_MeshDataByVertexNormal;

		
		E_NormalTypes GetNormalType() const;
		bool m_IsFaceNormalDebug = false;
	private:
		void SetNormalTypes(E_NormalTypes normal_types);
		
		unsigned int m_Indices =0;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexArray> m_VertexArray_Debug;

		std::vector<std::shared_ptr<VertexBuffer>> m_BufferForVertexNormal;
		std::vector<std::shared_ptr<VertexBuffer>> m_BufferForFaceNormal;
		std::shared_ptr<IndexBuffer> m_IndexBufferForFaceNormal;
		std::shared_ptr<IndexBuffer> m_IndexBufferForVertexNormal;

		std::shared_ptr<VertexBuffer> m_BufferDisplayVtxNormalDebug;
		std::shared_ptr<VertexBuffer> m_BufferDisplayFaceNormalDebug;

		std::shared_ptr<IndexBuffer> m_IndexBufferDisplayVtxNormalDebug;
		std::shared_ptr<IndexBuffer> m_IndexBufferDisplayFaceNormalDebug;
		bool m_CheckFlag = true;
		bool m_IsFaceNormalRender = false;
		float m_DebugMagntite = 0.05f;

		std::string m_Meshname = "";
		
		E_RenderTypes m_RenderType = E_RenderTypes::NONE;
		E_NormalTypes m_NormalType = E_NormalTypes::VERTEX;
	};



	
}
