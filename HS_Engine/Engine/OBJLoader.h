#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/detail/type_vec.hpp>
#include "Mesh/Mesh.h"
#include <cfloat>
namespace  HS_Engine
{
	class Utils {
	public:
		static void trimString(std::string& str)
		{
			const char* whiteSpace = " \t\n\r";
			size_t location;
			location = str.find_first_not_of(whiteSpace);
			str.erase(0, location);
			location = str.find_last_not_of(whiteSpace);
			str.erase(location + 1);
		}
	};
	class ObjectLoader
	{
	public:
		void LoadObjFile(std::string path);
		void GenerateVertexNormalIfnotexist();
		
		void ConvertToGLFormatMeshByFaceNormal(Mesh* mesh);

		
		void ConvertToGLFormatMeshByVertexNormal(Mesh* mesh);
		void ConvertToGLFormatMeshByVertexNormal(std::shared_ptr<Mesh> mesh);
		std::shared_ptr<Mesh> Load(std::string path, bool center);
		Mesh* Load_raw_ptr(std::string path, bool center);
		Mesh* Load_raw_ptr_new(std::string path, bool center);
		void ClearObjectData();
		float GetHugeModelAxisRange();
		bool CheckOutofNDCFormat();
		// Adjust to model vertices to [-1 ,1]
		void AdjustRange();
		class ObjectVertex
		{
		public:
			int m_pIdx;
			int m_tcIdx;
			int m_nIdx;

			ObjectVertex() : m_pIdx(-1), m_nIdx(-1), m_tcIdx(-1) {}
			ObjectVertex(std::string& vertexStr, ObjectLoader* data);
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

