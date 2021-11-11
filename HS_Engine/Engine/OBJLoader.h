/* Start Header -------------------------------------------------------
Copyright (C) <2021> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OBJLoader.h
Purpose: Load the obj file to Opengl format
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_1>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <09/11/21>
End Header --------------------------------------------------------*/
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/detail/type_vec.hpp>
#include "Mesh/Mesh.h"
#include <cfloat>
namespace  HS_Engine
{

	class ObjectLoader
	{
	public:
		void LoadObjFile(std::string path);
		void GenerateVertexNormal();
		void ConvertDataFormatMeshByFaceNormal(Mesh* mesh);

		void ConvertDataFormatMeshByVertexNormal(Mesh* mesh);
		void ConvertDataFormatMeshByVertexNormal(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Mesh> Load(std::string path);
		Mesh* Load_raw_ptr(std::string path);

		void ClearObjectData();
		float GetHugeModelAxisRange();
		static glm::vec2 CreateSphericalUV(glm::vec3 vec3);
		static glm::vec2 CreateCylindricalUV(glm::vec3 vec3);
		static glm::vec2 CreateCubeMapUV(glm::vec3 vec3);
		
		// Adjust to model vertices to [-1 ,1]
		void AdjustRange();
		class ObjectVertex
		{
		public:
			int m_pIdx;
			int m_tcIdx;
			int m_nIdx;

			ObjectVertex() : m_pIdx(-1), m_nIdx(-1), m_tcIdx(-1) {}
			ObjectVertex(ObjectLoader* data ,std::string& vertexStr );
			std::tuple<int,int ,int> tupledata()
			{
				return std::make_tuple(m_pIdx, m_tcIdx, m_nIdx);
			}
	
		};
	
	private:
		std::vector<glm::vec3> m_VertexData;
		std::vector<glm::vec3> m_NormalData;
		std::vector<glm::vec2> m_TexData;
		std::vector<ObjectVertex> m_facesData;

		glm::vec3 m_min{ FLT_MAX, FLT_MAX, FLT_MAX };
		glm::vec3 m_max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

		
	};

}

