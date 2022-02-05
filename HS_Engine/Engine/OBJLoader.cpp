/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
//#pragma comment(lib, "assimp.lib")
#include "OBJLoader.h"


#include <chrono>
#include <fstream>
#include <map>
#include <sstream>
#include "Shader.h"
#include "VertexArrayData.h"
#include <iostream>
#include <unordered_set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Engine/Mesh/ProceduralMesh.h"

namespace HS_Engine
{

	void ObjectLoader::LoadObjFile(std::string path)
	{
		//LoadOBJfileAssimp(path);
		ObjectLoader::LoadOBJfileAssimp(path);
		std::ifstream objfile(path, std::ios::in);

		if (objfile.good() == false)
		{
			throw std::runtime_error(path + " is not exist");
		}

		std::string line, token;

		getline(objfile, line);

		while (true)
		{
			size_t pos = line.find_first_of("#");
			if (pos != std::string::npos)
			{
				line = line.substr(0, pos);
			}

			if (line.length() > 0)
			{
				std::istringstream lineStream(line);

				lineStream >> token;

				if (token == "v")
				{
					glm::vec3 vertex;
					lineStream >> vertex.x;
					lineStream >> vertex.y;
					lineStream >> vertex.z;
					if (m_min.x > vertex.x)
						m_min.x = vertex.x;
					if (m_max.x <= vertex.x)
						m_max.x = vertex.x;
					if (m_min.y > vertex.y)
						m_min.y = vertex.y;
					if (m_max.y <= vertex.y)
						m_max.y = vertex.y;
					if (m_min.z > vertex.z)
						m_min.z = vertex.z;
					if (m_max.z <= vertex.z)
						m_max.z = vertex.z;
					m_VertexData.push_back(vertex);
				}
				else if (token == "vt")
				{
					glm::vec2 texturecoord;
					lineStream >> texturecoord.x >> texturecoord.y;
					m_TexData.push_back(texturecoord);
				}
				else if (token == "vn")
				{
					glm::vec3 normals;
					lineStream >> normals.x >> normals.y >> normals.z;
					m_NormalData.push_back(normals);
				}
				else if (token == "f")
				{
					std::vector<std::string> parts;
					while (lineStream.good())
					{
						std::string s;
						lineStream >> s;
						parts.push_back(s);
					}
					if (parts.size() > 2)
					{
						ObjectLoader::ObjectVertex firstVert(this, parts[0]);
						for (int i = 2; i < static_cast<int>(parts.size()); i++)
						{
							m_facesData.push_back(firstVert);
							int before = i - 1;
							m_facesData.push_back(ObjectLoader::ObjectVertex(this, parts[before]));
							m_facesData.push_back(ObjectLoader::ObjectVertex(this, parts[i]));
						}
					}
				}
			}
			if (objfile.eof())
			{
				break;
			}
			getline(objfile, line);
		}

		AdjustRange();


	}

	void ObjectLoader::GenerateVertexNormal()
	{
		if (m_NormalData.size() != 0) return;

		m_NormalData.resize(m_VertexData.size());

		std::unordered_map<unsigned int, std::vector<glm::vec3>> temp;
		for (size_t i = 0; i < m_facesData.size(); i += 3)
		{
			const glm::vec3& p1 = m_VertexData[m_facesData[i].m_pIdx];
			const glm::vec3& p2 = m_VertexData[m_facesData[1 + i].m_pIdx];
			const glm::vec3& p3 = m_VertexData[m_facesData[2 + i].m_pIdx];

			glm::vec3 a = p2 - p1;
			glm::vec3 b = p3 - p1;
			//use cross product
			glm::vec3 n = glm::normalize(glm::cross(a, b));

			//n *= -1.f;
			for (size_t j = i; j < i + 3; j++)
			{
				auto check = temp.find(m_facesData[j].m_pIdx);
				if (check == temp.end())
				{
					m_NormalData[m_facesData[j].m_pIdx] += n;
					temp[m_facesData[j].m_pIdx].push_back(n);
				}
				else
				{
					bool IsExist = false;
					for (auto normal : check->second)
					{
						if (abs(normal.x - n.x) < EPSILON && abs(normal.y - n.y) < EPSILON && abs(normal.z - n.z) < EPSILON)
						{
							IsExist = true;
						}
					}
					if (!IsExist)
					{
						m_NormalData[m_facesData[j].m_pIdx] += n;
						temp[m_facesData[j].m_pIdx].push_back(n);
					}
				}
			}

			m_facesData[i].m_nIdx = m_facesData[i].m_pIdx;
			m_facesData[1 + i].m_nIdx = m_facesData[1 + i].m_pIdx;
			m_facesData[2 + i].m_nIdx = m_facesData[2 + i].m_pIdx;
		}

		//average
		for (unsigned int i = 0; i < m_NormalData.size(); i++)
		{
			m_NormalData[i] = glm::normalize(m_NormalData[i]);
		}

	}


	void ObjectLoader::ConvertDataFormatMeshByFaceNormal(Mesh* mesh)
	{
		mesh->ClearDataByFaceNormal();
		int idx = 0;
		HS_Engine::Mesh::MeshData& meshdata = mesh->m_MeshDataByFaceNormal;

		for (size_t i = 0; i < m_facesData.size(); i += 3)
		{
			const glm::vec3& p1 = m_VertexData[m_facesData[i].m_pIdx];
			const glm::vec3& p2 = m_VertexData[m_facesData[i + 1].m_pIdx];
			const glm::vec3& p3 = m_VertexData[m_facesData[i + 2].m_pIdx];

			glm::vec3 a = p2 - p1;
			glm::vec3 b = p3 - p1;
			//use cross product
			glm::vec3 n = glm::normalize(glm::cross(a, b));

			const glm::vec3 center_of_triangle = (p1 + p2 + p3) / 3.f;

			meshdata.m_DebugNormalData.m_Vertexs.push_back(center_of_triangle.x);
			meshdata.m_DebugNormalData.m_Vertexs.push_back(center_of_triangle.y);
			meshdata.m_DebugNormalData.m_Vertexs.push_back(center_of_triangle.z);


			meshdata.m_Vertexs.push_back(p1.x);
			meshdata.m_Vertexs.push_back(p1.y);
			meshdata.m_Vertexs.push_back(p1.z);

			meshdata.m_Vertexs.push_back(p2.x);
			meshdata.m_Vertexs.push_back(p2.y);
			meshdata.m_Vertexs.push_back(p2.z);

			meshdata.m_Vertexs.push_back(p3.x);
			meshdata.m_Vertexs.push_back(p3.y);
			meshdata.m_Vertexs.push_back(p3.z);

			glm::vec3 normal_other_point = center_of_triangle;
			normal_other_point += (n * mesh->GetDebugMagnitue());

			meshdata.m_DebugNormalData.m_Vertexs.push_back(normal_other_point.x);
			meshdata.m_DebugNormalData.m_Vertexs.push_back(normal_other_point.y);
			meshdata.m_DebugNormalData.m_Vertexs.push_back(normal_other_point.z);

			//uv
			glm::vec3 normalVertex = glm::vec3((p1.x - 1.0f));
			glm::vec2 uv{ 0.f };
			glm::vec3 centroidVec1 = glm::normalize(p1);
			glm::vec3 centroidVec2 = glm::normalize(p2);
			glm::vec3 centroidVec3 = glm::normalize(p3);


			std::vector<glm::vec3> center;
			center.push_back(centroidVec1);
			center.push_back(centroidVec2);
			center.push_back(centroidVec3);

			for (auto& cen : center)
			{
				uv = CreateSphericalUV(cen);
				meshdata.m_TexCoords_SPHERICAL.push_back(uv.x);
				meshdata.m_TexCoords_SPHERICAL.push_back(uv.y);
				uv = CreateCylindricalUV(cen);
				meshdata.m_TexCoords_CYLINDRICAL.push_back(uv.x);
				meshdata.m_TexCoords_CYLINDRICAL.push_back(uv.y);
				uv = CreateCubeMapUV(cen);
				meshdata.m_TexCoords_CUBE_MAPPED.push_back(uv.x);
				meshdata.m_TexCoords_CUBE_MAPPED.push_back(uv.y);
			}

			center.clear();

			for (int i = 0; i < 3; ++i)
			{
				uv = CreateSphericalUV(n);
				meshdata.m_TexCoords_SPHERICAL_NORMAL.push_back(uv.x);
				meshdata.m_TexCoords_SPHERICAL_NORMAL.push_back(uv.y);
				uv = CreateCylindricalUV(n);
				meshdata.m_TexCoords_CYLINDRICAL_NORMAL.push_back(uv.x);
				meshdata.m_TexCoords_CYLINDRICAL_NORMAL.push_back(uv.y);
				uv = CreateCubeMapUV(n);
				meshdata.m_TexCoords_CUBE_MAPPED_NORMAL.push_back(uv.x);
				meshdata.m_TexCoords_CUBE_MAPPED_NORMAL.push_back(uv.y);
			}

			for (int j = 0; j < 3; ++j)
			{
				meshdata.m_Normals.push_back(n.x);
				meshdata.m_Normals.push_back(n.y);
				meshdata.m_Normals.push_back(n.z);
			}

			for (int t = 0; t < 3; t++)
			{
				meshdata.m_Faces.push_back(idx + t);
			}

			idx += 3;
		}
	}

	void ObjectLoader::ConvertDataFormatMeshByVertexNormal(std::shared_ptr<Mesh> mesh)
	{
		mesh->ClearDataByVertexNormal();

		std::map<std::tuple<int, int, int>, unsigned int> vertexMap;
		for (ObjectVertex& vert : m_facesData)
		{
			std::tuple<int, int, int> vertStr = vert.tupledata();
			auto iter = vertexMap.find(vertStr);

			if (iter == vertexMap.end())
			{
				unsigned vIdx = static_cast<unsigned>(mesh->m_MeshDataByVertexNormal.m_Vertexs.size() / 3);
				glm::vec3& vertex = m_VertexData[vert.m_pIdx];

				mesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(vertex.x);
				mesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(vertex.y);
				mesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(vertex.z);

				glm::vec3& normal = m_NormalData[vert.m_nIdx];
				mesh->m_MeshDataByVertexNormal.m_Normals.push_back(normal.x);
				mesh->m_MeshDataByVertexNormal.m_Normals.push_back(normal.y);
				mesh->m_MeshDataByVertexNormal.m_Normals.push_back(normal.z);






				if (m_TexData.empty() == false)
				{
					glm::vec2& texcoord = m_TexData[vert.m_tcIdx];
					mesh->m_MeshDataByVertexNormal.m_TexCoords.push_back(texcoord.x);
					mesh->m_MeshDataByVertexNormal.m_TexCoords.push_back(texcoord.y);
				}



				mesh->m_MeshDataByVertexNormal.m_Faces.push_back(static_cast<unsigned int>(vIdx));
				vertexMap[vertStr] = static_cast<unsigned int>(vIdx);
			}
			else
			{
				mesh->m_MeshDataByVertexNormal.m_Faces.push_back(iter->second);
			}
		}




	}

	void ObjectLoader::ConvertDataFormatMeshByVertexNormal(Mesh* mesh)
	{
		mesh->ClearDataByVertexNormal();

		std::map<std::tuple<int, int, int>, unsigned int> vertexMap;

		for (ObjectVertex& vert : m_facesData)
		{
			std::tuple<int, int, int> vertStr = vert.tupledata();
			auto iter = vertexMap.find(vertStr);

			if (iter == vertexMap.end())
			{
				unsigned vIdx = static_cast<unsigned>(mesh->m_MeshDataByVertexNormal.m_Vertexs.size() / 3);
				auto& MeshData = mesh->m_MeshDataByVertexNormal;

				const glm::vec3& vertex = m_VertexData[vert.m_pIdx];

				MeshData.m_Vertexs.push_back(vertex.x);
				MeshData.m_Vertexs.push_back(vertex.y);
				MeshData.m_Vertexs.push_back(vertex.z);

				glm::vec3& normal = m_NormalData[vert.m_nIdx];

				MeshData.m_Normals.push_back(normal.x);
				MeshData.m_Normals.push_back(normal.y);
				MeshData.m_Normals.push_back(normal.z);


				MeshData.m_DebugNormalData.m_Vertexs.push_back(vertex.x);
				MeshData.m_DebugNormalData.m_Vertexs.push_back(vertex.y);
				MeshData.m_DebugNormalData.m_Vertexs.push_back(vertex.z);

				glm::vec3 normal_other_point = vertex;
				normal_other_point += (normal * mesh->GetDebugMagnitue());
				MeshData.m_DebugNormalData.m_Vertexs.push_back(normal_other_point.x);
				MeshData.m_DebugNormalData.m_Vertexs.push_back(normal_other_point.y);
				MeshData.m_DebugNormalData.m_Vertexs.push_back(normal_other_point.z);


				if (m_TexData.empty() == false)
				{
					if (vert.m_tcIdx != -1)
					{
						glm::vec2& texcoord = m_TexData[vert.m_tcIdx];
						MeshData.m_TexCoords.push_back(texcoord.x);
						MeshData.m_TexCoords.push_back(texcoord.y);
					}
				}
				glm::vec3 normVertex = glm::normalize(glm::vec3((vertex.x + 1.0f) / 2.f, (vertex.y + 1.0f) / 2.f, (vertex.z + 1.0f) / 2.f));
				float theta(0.0f);
				float z(0.0f);
				float phi(0.0f);

				//planar
				glm::vec2 uv(0.0f);
				uv.x = (normVertex.x - (-1.0f)) / (2.0f);
				uv.y = (normVertex.y - (-1.0f)) / (2.0f);

				MeshData.m_TexCoords_PLANAR.push_back(uv.x);
				MeshData.m_TexCoords_PLANAR.push_back(uv.x);

				glm::vec3 centroidVec = glm::normalize(vertex);

				////Cylindical
				uv = CreateCylindricalUV(centroidVec);


				MeshData.m_TexCoords_CYLINDRICAL.push_back(uv.x);
				MeshData.m_TexCoords_CYLINDRICAL.push_back(uv.y);

				//Spherical
				uv = CreateSphericalUV(centroidVec);

				MeshData.m_TexCoords_SPHERICAL.push_back(uv.x);
				MeshData.m_TexCoords_SPHERICAL.push_back(uv.y);


				// cubemap
				uv = CreateCubeMapUV(centroidVec);

				MeshData.m_TexCoords_CUBE_MAPPED.push_back(uv.s);
				MeshData.m_TexCoords_CUBE_MAPPED.push_back(uv.t);



				// Normal//
				uv.x = (normVertex.x - (-1.0f)) / (2.0f);
				uv.y = (normVertex.y - (-1.0f)) / (2.0f);

				MeshData.m_TexCoords_PLANAR_NORMAL.push_back(uv.x);
				MeshData.m_TexCoords_PLANAR_NORMAL.push_back(uv.x);

				centroidVec = glm::normalize(normal);

				//Cylindical
				uv = CreateCylindricalUV(centroidVec);

				MeshData.m_TexCoords_CYLINDRICAL_NORMAL.push_back(uv.x);
				MeshData.m_TexCoords_CYLINDRICAL_NORMAL.push_back(uv.y);

				//Spherical
				uv = CreateSphericalUV(centroidVec);

				MeshData.m_TexCoords_SPHERICAL_NORMAL.push_back(uv.x);
				MeshData.m_TexCoords_SPHERICAL_NORMAL.push_back(uv.y);

				// cubemap normal
				uv = CreateCubeMapUV(centroidVec);

				MeshData.m_TexCoords_CUBE_MAPPED_NORMAL.push_back(uv.s);
				MeshData.m_TexCoords_CUBE_MAPPED_NORMAL.push_back(uv.t);


				mesh->m_MeshDataByVertexNormal.m_Faces.push_back(static_cast<unsigned int>(vIdx));
				vertexMap[vertStr] = static_cast<unsigned int>(vIdx);
			}
			else
			{
				mesh->m_MeshDataByVertexNormal.m_Faces.push_back(iter->second);
			}

		}


	}


	std::shared_ptr<Mesh> ObjectLoader::Load(std::string path)
	{
		std::shared_ptr<Mesh> newmesh = std::make_shared<Mesh>();
		auto startTime = std::chrono::high_resolution_clock::now();
		LoadObjFile(path);
		GenerateVertexNormal();
		ConvertDataFormatMeshByVertexNormal(newmesh);
		newmesh->SetIndexBufferCount(static_cast<unsigned>(m_facesData.size()));

		auto endTime = std::chrono::high_resolution_clock::now();

		double timeDuration = std::chrono::duration< double, std::milli >(endTime - startTime).count();

		std::cout << "\"" << path << "\"" << " OBJ file read in "
			<< timeDuration
			<< "  milli seconds." << std::endl;

		return newmesh;
	}

	Mesh* ObjectLoader::Load_raw_ptr(std::string path)
	{
		Mesh* newmesh = new Mesh();
		auto startTime = std::chrono::high_resolution_clock::now();
		LoadObjFile(path);
		GenerateVertexNormal();
		ConvertDataFormatMeshByFaceNormal(newmesh);
		ConvertDataFormatMeshByVertexNormal(newmesh);
		newmesh->SetIndexBufferCount(static_cast<unsigned>(m_facesData.size()));
		ClearObjectData();

		auto endTime = std::chrono::high_resolution_clock::now();

		double timeDuration = std::chrono::duration< double, std::milli >(endTime - startTime).count();

		std::cout << "\"" << path << "\"" << " OBJ file read in "
			<< timeDuration
			<< "  milli seconds." << std::endl;

		return newmesh;
	}

	


	void ObjectLoader::ClearObjectData()
	{
		m_VertexData.clear();
		m_NormalData.clear();
		m_TexData.clear();
		m_facesData.clear();
		m_min = { FLT_MAX, FLT_MAX, FLT_MAX };
		m_max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	}


	ObjectLoader::ObjectVertex::ObjectVertex(ObjectLoader* data, std::string& vertexStr) : m_pIdx(-1), m_nIdx(-1), m_tcIdx(-1)
	{

		// vertex/tex/nor
		// num/num/num
		size_t slash1;
		size_t slash2;
		slash1 = vertexStr.find("/");
		m_pIdx = std::stoi(vertexStr.substr(0, slash1));
		if (m_pIdx < 0)
		{
			m_pIdx += static_cast<int>(data->m_VertexData.size());
		}
		else
		{
			// array buffer begin at 0 but obj file start 1
			m_pIdx--;
		}
		if (slash1 != std::string::npos)
		{
			slash2 = vertexStr.find("/", slash1 + 1);
			if (slash2 > slash1 + 1)
			{
				//check texture coord index;
				m_tcIdx = std::stoi(vertexStr.substr(slash1 + 1, slash2 - slash1 - 1));
				if (m_tcIdx < 0)
				{
					m_tcIdx += static_cast<int>(data->m_TexData.size());
				}
				else
				{
					m_tcIdx--;
				}
			}
			//check normal coord index
			m_nIdx = std::stoi(vertexStr.substr(slash2 + 1));
			if (m_nIdx < 0)
			{
				m_nIdx += static_cast<int>(data->m_NormalData.size());
			}
			else
			{
				m_nIdx--;
			}

		}

	}



	void ObjectLoader::AdjustRange()
	{
		glm::vec3 point = (m_max + m_min) * 0.5f;
		float CheckRange = GetHugeModelAxisRange() * 0.5f;

		for (auto& vertex : m_VertexData)
		{
			vertex -= point;
			vertex /= CheckRange;
		}


	}

	float ObjectLoader::GetHugeModelAxisRange()
	{
		float x_Range = m_max.x - m_min.x;
		float y_Range = m_max.y - m_min.y;
		float z_Range = m_max.z - m_min.z;

		float result = x_Range < y_Range ? y_Range : x_Range;
		result = result < z_Range ? z_Range : result;

		return result;
	}

	glm::vec2 ObjectLoader::CreateSphericalUV(glm::vec3 vec3)
	{
		glm::vec2 uv;
		float theta = glm::degrees(static_cast<float>(atan2(vec3.z, vec3.x)));
		theta += 180.0f;

		float z = vec3.y;
		float phi = glm::degrees(glm::acos(z / vec3.length()));

		uv.x = theta / 360.0f;
		uv.y = 1.0f - (phi / 180.0f);

		return uv;
	}

	glm::vec2 ObjectLoader::CreateCylindricalUV(glm::vec3 vec3)
	{
		glm::vec2 uv;
		float theta = 0.f;
		theta = glm::degrees(static_cast<float>(atan2(vec3.z, vec3.x)));
		theta += 180.0f;

		float z = (vec3.y + 1.0f) * 0.5f;

		uv.x = theta / 360.0f;
		uv.y = z;

		return uv;
	}

	glm::vec2 ObjectLoader::CreateCubeMapUV(glm::vec3 vec3)
	{
		float x = vec3.x;
		float y = vec3.y;
		float z = vec3.z;

		float absX = abs(x);
		float absY = abs(y);
		float absZ = abs(z);

		bool isXPositive = x > 0 ? 1 : 0;
		bool isYPositive = y > 0 ? 1 : 0;
		bool isZPositive = z > 0 ? 1 : 0;

		float maxAxis = 0;
		float uc = 0;
		float vc = 0;

		// POSITIVE X right
		if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
		{
			maxAxis = absX;
			uc = -z;
			vc = y;
		}

		// NEGATIVE X left
		else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
		{

			maxAxis = absX;
			uc = z;
			vc = y;
		}

		// POSITIVE Y top
		else if (bool(isYPositive) && absY >= absX && absY >= absZ)
		{
			maxAxis = absY;
			uc = x;
			vc = -z;
		}

		// NEGATIVE Y bottom
		else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
		{
			maxAxis = absY;
			uc = x;
			vc = z;
		}

		// POSITIVE Z front
		else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
		{
			maxAxis = absZ;
			uc = x;
			vc = y;
		}

		// NEGATIVE Z back
		else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
		{
			maxAxis = absZ;
			uc = -x;
			vc = y;
		}
		glm::vec2 uv;

		uv.s = 0.5f * (uc / maxAxis + 1.0f);
		uv.t = 0.5f * (vc / maxAxis + 1.0f);

		return uv;
	}

	Meshes* ObjectLoader::Load_raw_ptr_meshes(std::string path)
	{

		return nullptr;
		
	}

	void ObjectLoader::LoadOBJfileAssimp(const std::string& path)
	{
		bool Ret = false;
		//TODO : implemented the assimp model loader
		Assimp::Importer Importer;
		const aiScene* pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode )
		{
			std::cout << "Load failed obj : " << path << std::endl;
			std::cout << Importer.GetErrorString() << std::endl;
			
		}
	}

	void ObjectLoader::ProcessTheNode(aiNode* node, const aiScene* scene)
	{
		for(unsigned idx = 0; idx < node->mNumMeshes; ++idx)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[idx]];
			
			
		}
			
	}
}
	
