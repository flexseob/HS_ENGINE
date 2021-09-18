#include "OBJLoader.h"

#include <chrono>
#include <fstream>
#include <map>
#include <sstream>
#include "Shader.h"
#include "VertexArrayData.h"
#include <iostream>
#include <unordered_set>

#include "../Engine/Mesh/PreceduralMesh.hpp"

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
			Utils::trimString(line);
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
						ObjectLoader::ObjectVertex firstVert(parts[0], this);
						for (int i = 2; i < parts.size(); i++)
						{
							m_facesData.push_back(firstVert);
							m_facesData.push_back(ObjectLoader::ObjectVertex(parts[i - 1], this));
							m_facesData.push_back(ObjectLoader::ObjectVertex(parts[i], this));
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

	void ObjectLoader::GenerateNormalIfnotexist()
	{
		if (m_NormalData.size() != 0) return;

		m_NormalData.resize(m_VertexData.size());

		std::unordered_map<unsigned int, std::vector<glm::vec3>> temp;
		for (unsigned int i = 0; i < m_facesData.size(); i += 3)
		{
			const glm::vec3& p1 = m_VertexData[m_facesData[i].m_pIdx];
			const glm::vec3& p2 = m_VertexData[m_facesData[i + 1].m_pIdx];
			const glm::vec3& p3 = m_VertexData[m_facesData[i + 2].m_pIdx];

			glm::vec3 a = p2 - p1;
			glm::vec3 b = p3 - p1;
			//use cross product
			glm::vec3 n = glm::normalize(glm::cross(a, b));

			for(int j = i; j<i+3; j++)
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
			m_facesData[i + 1].m_nIdx = m_facesData[i + 1].m_pIdx;
			m_facesData[i + 2].m_nIdx = m_facesData[i + 2].m_pIdx;
		}
		
		//average
		for (unsigned int i = 0; i < m_NormalData.size(); i++) 
		{
			m_NormalData[i] = glm::normalize(m_NormalData[i]);
		}
		
	}

	void ObjectLoader::ConvertToGLFormatMesh(std::shared_ptr<Mesh> mesh)
	{
		mesh->ClearData();

		std::map<std::tuple<int,int,int>, unsigned int> vertexMap;
		for (ObjectVertex& vert : m_facesData) 
		{
			std::tuple<int,int,int> vertStr = vert.tupledata();
			auto iter = vertexMap.find(vertStr);

			if (iter == vertexMap.end()) 
			{
				unsigned vIdx = mesh->m_Vertexs.size() / 3;
				glm::vec3& vertex = m_VertexData[vert.m_pIdx];

				mesh->m_Vertexs.push_back(vertex.x);
				mesh->m_Vertexs.push_back(vertex.y);
				mesh->m_Vertexs.push_back(vertex.z);

				glm::vec3& normal = m_NormalData[vert.m_nIdx];
				mesh->m_Normals.push_back(normal.x);
				mesh->m_Normals.push_back(normal.y);
				mesh->m_Normals.push_back(normal.z);

				if (m_TexData.empty() == false)
				{
					glm::vec2& texcoord = m_TexData[vert.m_tcIdx];
					mesh->m_TexCoords.push_back(texcoord.x);
					mesh->m_TexCoords.push_back(texcoord.y);
				}


				mesh->m_Faces.push_back(static_cast<unsigned int>(vIdx));
				vertexMap[vertStr] = static_cast<unsigned int>(vIdx);
			}
			else 
			{
				mesh->m_Faces.push_back(iter->second);
			}
		}

		

		
	}

	void ObjectLoader::ConvertToGLFormatMesh(Mesh* mesh)
	{
		mesh->ClearData();

		std::map<std::tuple<int, int, int>, unsigned int> vertexMap;
		for (ObjectVertex& vert : m_facesData)
		{
			std::tuple<int, int, int> vertStr = vert.tupledata();
			auto iter = vertexMap.find(vertStr);

			if (iter == vertexMap.end())
			{
				unsigned vIdx = mesh->m_Vertexs.size() / 3;
				glm::vec3& vertex = m_VertexData[vert.m_pIdx];

				mesh->m_Vertexs.push_back(vertex.x);
				mesh->m_Vertexs.push_back(vertex.y);
				mesh->m_Vertexs.push_back(vertex.z);

				glm::vec3& normal = m_NormalData[vert.m_nIdx];
				mesh->m_Normals.push_back(normal.x);
				mesh->m_Normals.push_back(normal.y);
				mesh->m_Normals.push_back(normal.z);

				if (m_TexData.empty() == false)
				{
					glm::vec2& texcoord = m_TexData[vert.m_tcIdx];
					mesh->m_TexCoords.push_back(texcoord.x);
					mesh->m_TexCoords.push_back(texcoord.y);
				}


				mesh->m_Faces.push_back(static_cast<unsigned int>(vIdx));
				vertexMap[vertStr] = static_cast<unsigned int>(vIdx);
			}
			else
			{
				mesh->m_Faces.push_back(iter->second);
			}
		}
		
	}


	std::shared_ptr<Mesh> ObjectLoader::Load(std::string path, bool center)
	{
		std::shared_ptr<Mesh> newmesh = std::make_shared<Mesh>();
		auto startTime = std::chrono::high_resolution_clock::now();
		LoadObjFile(path);
		GenerateNormalIfnotexist();
		ConvertToGLFormatMesh(newmesh);
		newmesh->SetIndexBufferCount(m_facesData.size());
		
		auto endTime = std::chrono::high_resolution_clock::now();

		double timeDuration = std::chrono::duration< double, std::milli >(endTime - startTime).count();

		std::cout <<"\""<< path << "\"" << " OBJ file read in "
			<< timeDuration
			<< "  milli seconds." << std::endl;
		
		return newmesh;
	}

	Mesh* ObjectLoader::Load_raw_ptr(std::string path, bool center)
	{
		Mesh* newmesh = new Mesh();
		auto startTime = std::chrono::high_resolution_clock::now();
		LoadObjFile(path);
		GenerateNormalIfnotexist();
		ConvertToGLFormatMesh(newmesh);
		newmesh->SetIndexBufferCount(m_facesData.size());

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
	}


	ObjectLoader::ObjectVertex::ObjectVertex(std::string& vertexStr, ObjectLoader* data) : m_pIdx(-1), m_nIdx(-1), m_tcIdx(-1)
	{
		
		// vertex/tex/nor
		// num/num/num
		unsigned slash1;
		unsigned slash2;
		slash1 = vertexStr.find("/");
		m_pIdx = std::stoi(vertexStr.substr(0, slash1));
		if (m_pIdx < 0)
		{
			m_pIdx += data->m_VertexData.size();
		}
		else
		{
			// array buffer begin at 0
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
					m_tcIdx += data->m_TexData.size();
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
				m_nIdx += data->m_NormalData.size();
			}
			else
			{
				m_nIdx--;
			}

		}

	}

	bool ObjectLoader::CheckOutofNDCFormat()
	{
		if (m_min.x < -1.f || m_max.x > 1.f)
		{
			return false;
		}
		else if (m_min.y < -1.f || m_max.y > 1.f)
		{
			return false;
		}
		else if (m_min.z < -1.f || m_max.z > 1.f)
		{
			return false;
		}
		return true;
	}
	
	void ObjectLoader::AdjustRange()
	{
		if (CheckOutofNDCFormat() == false)
		{
			float CheckRange = GetHugeModelAxisRange();
			for (auto& vertex : m_VertexData)
			{
				vertex.x -= m_min.x;
				vertex.y -= m_min.y;
				vertex.z -= m_min.z;

				vertex = (vertex * 2.f / CheckRange);

				vertex -= 1.f;
			}
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
	
