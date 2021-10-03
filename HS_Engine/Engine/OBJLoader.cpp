
#include "OBJLoader.h"

#include <chrono>
#include <fstream>
#include <map>
#include <sstream>
#include "Shader.h"
#include "VertexArrayData.h"
#include <iostream>
#include <unordered_set>

#include "../Engine/Mesh/ProceduralMesh.h"

namespace HS_Engine
{

	void ObjectLoader::LoadObjFile(std::string path)
	{
		std::ifstream objfile(path, std::ios::in);

		if(objfile.good() == false)
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
					lineStream >> vertex.x >> vertex.y >> vertex.z;
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
							m_facesData.push_back(ObjectLoader::ObjectVertex( this, parts[before]));
							m_facesData.push_back(ObjectLoader::ObjectVertex( this, parts[i]));
						}
					}
				}
			}
			if(objfile.eof())
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
			for(size_t j = i; j<i+3; j++)
			{
				auto check = temp.find(m_facesData[j].m_pIdx);
				if(check == temp.end())
				{
					m_NormalData[m_facesData[j].m_pIdx] += n;
					temp[m_facesData[j].m_pIdx].push_back(n);
				}
				else
				{
					bool IsExist = false;
					for(auto normal : check->second)
					{
						if(normal == n)
						{
							IsExist = true;
						}
					}
					if(!IsExist)
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
		for(size_t i = 0; i < m_facesData.size(); i += 3)
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
			
			for(int j=0; j <3; ++j)
			{
				meshdata.m_Normals.push_back(n.x);
				meshdata.m_Normals.push_back(n.y);
				meshdata.m_Normals.push_back(n.z);
			}

			for(int t=0; t<3; t++)
			{
				meshdata.m_Faces.push_back(idx+t);
			}

			idx += 3;
		}
	}

	void ObjectLoader::ConvertDataFormatMeshByVertexNormal(std::shared_ptr<Mesh> mesh)
	{
		mesh->ClearDataByVertexNormal();

		std::map<std::tuple<int,int,int>, unsigned int> vertexMap;
		for (ObjectVertex& vert : m_facesData) 
		{
			std::tuple<int,int,int> vertStr = vert.tupledata();
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

		std::cout <<"\""<< path << "\"" << " OBJ file read in "
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
		m_min={ FLT_MAX, FLT_MAX, FLT_MAX };
		m_max={ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	}


	ObjectLoader::ObjectVertex::ObjectVertex( ObjectLoader* data,std::string& vertexStr) : m_pIdx(-1), m_nIdx(-1), m_tcIdx(-1)
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
		float CheckRange = GetHugeModelAxisRange()*0.5f;
		
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
}
	
