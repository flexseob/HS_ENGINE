/* Start Header -------------------------------------------------------
Copyright (C) <2021> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: Mesh setup the buffers and mesh data
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_1>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <09/11/21>
End Header --------------------------------------------------------*/
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
		void SetUVCheckFlag();
		
		void SetRenderType(E_RenderTypes types);
		void SetDebugMangitue(float magnitue);
		void SetMeshName(std::string name);
		void SetMeshType(E_MeshTypes types);
		void SetGPUCalculation(bool isGPUCalculate);
		bool GetGPUCalucation() const;
		
		
		void ChangeUVType(E_UV_Types types, E_UV_Entity_Types entity_types);
		void ChangeUV_EntityType(E_UV_Entity_Types types);

		E_MeshTypes GetMeshType() const;
		E_UV_Types GetUVType() const;
		E_UV_Entity_Types GetUV_Entity_Types() const;
		
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
			
			std::vector<float> m_TexCoords_PLANAR;
			std::vector<float> m_TexCoords_CYLINDRICAL;
			std::vector<float> m_TexCoords_SPHERICAL;
			std::vector<float> m_TexCoords_CUBE_MAPPED;

			std::vector<float> m_TexCoords_PLANAR_NORMAL;
			std::vector<float> m_TexCoords_CYLINDRICAL_NORMAL;
			std::vector<float> m_TexCoords_SPHERICAL_NORMAL;
			std::vector<float> m_TexCoords_CUBE_MAPPED_NORMAL;
			
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
		std::vector<std::shared_ptr<VertexBuffer>> m_Buffers_TexCoords;
		std::vector<std::shared_ptr<VertexBuffer>> m_BufferForFaceNormal;
		
		std::shared_ptr<IndexBuffer> m_IndexBufferForFaceNormal;
		std::shared_ptr<IndexBuffer> m_IndexBufferForVertexNormal;

		std::shared_ptr<VertexBuffer> m_BufferDisplayVtxNormalDebug;
		std::shared_ptr<VertexBuffer> m_BufferDisplayFaceNormalDebug;

		std::shared_ptr<IndexBuffer> m_IndexBufferDisplayVtxNormalDebug;
		std::shared_ptr<IndexBuffer> m_IndexBufferDisplayFaceNormalDebug;

		bool m_CheckFlag = true;
		bool m_UV_CheckFlag = false;
		bool m_UV_GPU_calculation = false;

		
		bool m_IsFaceNormalRender = false;
		float m_DebugMagntite = 0.05f;

		std::string m_Meshname = "";
		
		E_RenderTypes m_RenderType = E_RenderTypes::NONE;
		E_NormalTypes m_NormalType = E_NormalTypes::VERTEX;
		E_UV_Types	  m_UV_Types = E_UV_Types::CYLINDRICAL_UV;
		E_UV_Entity_Types m_UV_Entity_Types = E_UV_Entity_Types::VERTEX_POS;
		E_MeshTypes m_MeshType = E_MeshTypes::BY_PATHMESH;
	};



	
}
