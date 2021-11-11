/* Start Header-------------------------------------------------------
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : ProceduralMesh.cpp
Purpose : ProceduralMesh class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

#include "ProceduralMesh.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../OBJLoader.h"
namespace HS_Engine {
	
    void BuildIndexBuffer(int numDivisions, ProceduralMesh* preceduralmesh);
    void addIndex(ProceduralMesh* preceduralmesh, int index);

    ProceduralMesh* CreateSphere(const float Radius, int numbDivsions)
    {
        ProceduralMesh* newmesh = new ProceduralMesh();

        const float AlphaDelta = 2.f * PI / numbDivsions;
        const float BetaDelta = PI / numbDivsions;

        auto& debugvertexnormal_data = newmesh->m_MeshDataByVertexNormal.m_DebugNormalData.m_Vertexs;
        for (int stack{}; stack <= numbDivsions; ++stack)
        {
            float phi = 0.5f * PI - BetaDelta * stack;

            for (int slice{}; slice <= numbDivsions; ++slice)
            {
                float alpha = AlphaDelta * slice;

                glm::vec3 point{ Radius * sin(alpha) * cos(phi) ,Radius * sin(phi),Radius * cos(alpha) * cos(phi) };
                newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.x);
                newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.y);
                newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.z);

                glm::vec3 normal = glm::normalize(point);
                newmesh->m_MeshDataByVertexNormal.m_Normals.push_back(normal.x);
                newmesh->m_MeshDataByVertexNormal.m_Normals.push_back(normal.y);
                newmesh->m_MeshDataByVertexNormal.m_Normals.push_back(normal.z);

                debugvertexnormal_data.push_back(point.x);
                debugvertexnormal_data.push_back(point.y);
                debugvertexnormal_data.push_back(point.z);
            	
                glm::vec3 normal_other_point = point;
                normal_other_point += normal * 0.1f;

                debugvertexnormal_data.push_back(normal_other_point.x);
                debugvertexnormal_data.push_back(normal_other_point.y);
                debugvertexnormal_data.push_back(normal_other_point.z);
            	
            	
            	
                glm::vec2 uv = glm::vec2((float)slice / numbDivsions, (float)stack / numbDivsions);
                newmesh->m_MeshDataByVertexNormal.m_TexCoords.push_back(uv.x);
                newmesh->m_MeshDataByVertexNormal.m_TexCoords.push_back(uv.y);


                uv = glm::vec2{ 0.f };
                glm::vec3 centroidVec = glm::normalize(point);



                std::vector<glm::vec3> center;
     
             
                    uv = ObjectLoader::CreateSphericalUV(centroidVec);
					newmesh->m_MeshDataByVertexNormal.m_TexCoords_SPHERICAL.push_back(uv.x);
					newmesh->m_MeshDataByVertexNormal.m_TexCoords_SPHERICAL.push_back(uv.y);
                    uv = ObjectLoader::CreateCylindricalUV(centroidVec);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CYLINDRICAL.push_back(uv.x);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CYLINDRICAL.push_back(uv.y);
                    uv = ObjectLoader::CreateCubeMapUV(centroidVec);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CUBE_MAPPED.push_back(uv.x);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CUBE_MAPPED.push_back(uv.y);
                

				  center.clear();

           
                    uv = ObjectLoader::CreateSphericalUV(normal);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_SPHERICAL_NORMAL.push_back(uv.x);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_SPHERICAL_NORMAL.push_back(uv.y);
                    uv = ObjectLoader::CreateCylindricalUV(normal);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CYLINDRICAL_NORMAL.push_back(uv.x);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CYLINDRICAL_NORMAL.push_back(uv.y);
                    uv = ObjectLoader::CreateCubeMapUV(normal);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CUBE_MAPPED_NORMAL.push_back(uv.x);
                    newmesh->m_MeshDataByVertexNormal.m_TexCoords_CUBE_MAPPED_NORMAL.push_back(uv.y);
                

            }
        }
 	
        BuildIndexBuffer(numbDivsions, newmesh);

    	//copy the data
        newmesh->m_MeshDataByFaceNormal.m_Vertexs = newmesh->m_MeshDataByVertexNormal.m_Vertexs;
        newmesh->m_MeshDataByFaceNormal.m_TexCoords = newmesh->m_MeshDataByVertexNormal.m_TexCoords;

    	const auto& facedata = newmesh->m_MeshDataByFaceNormal.m_Faces;
        const auto& vertexdata = newmesh->m_MeshDataByFaceNormal.m_Vertexs;
    	
        auto& normaldata = newmesh->m_MeshDataByFaceNormal.m_Normals;
        auto& debugfacenormaldata = newmesh->m_MeshDataByFaceNormal.m_DebugNormalData.m_Vertexs;
        normaldata.resize(vertexdata.size());


        //std::unordered_map<unsigned int, std::vector<glm::vec3>> temp;
    	for(int idx = 0; idx < static_cast<int>(facedata.size()); idx+=3)
    	{
            size_t p1_idx = facedata[idx];
            size_t p2_idx = facedata[static_cast<size_t>(idx)+1];
            size_t p3_idx = facedata[static_cast<size_t>(idx)+2];

            const auto& vertexdata = newmesh->m_MeshDataByFaceNormal.m_Vertexs;

    		
            glm::vec3 p1 = { vertexdata[p1_idx * 3], vertexdata[3 * p1_idx + 1], vertexdata[3 * p1_idx + 2] };
            glm::vec3 p2 = { vertexdata[p2_idx * 3], vertexdata[3 * p2_idx + 1], vertexdata[3 * p2_idx + 2] };
            glm::vec3 p3 = { vertexdata[p3_idx * 3], vertexdata[3 * p3_idx + 1], vertexdata[3 * p3_idx + 2] };

            glm::vec3 a = p2 - p1;
            glm::vec3 b = p3 - p1;
            //use cross product
            glm::vec3 n = glm::normalize(glm::cross(a, b));

            glm::vec3 center_of_triangle = (p1 + p2 + p3) / 3.f;

            debugfacenormaldata.push_back(center_of_triangle.x);
            debugfacenormaldata.push_back(center_of_triangle.y);
            debugfacenormaldata.push_back(center_of_triangle.z);

            glm::vec3 normal_other_point = center_of_triangle;
            normal_other_point += (n * newmesh->GetDebugMagnitue());

            debugfacenormaldata.push_back(normal_other_point.x);
            debugfacenormaldata.push_back(normal_other_point.y);
            debugfacenormaldata.push_back(normal_other_point.z);

            normaldata[p1_idx * 3] = n.x;
            normaldata[p1_idx * 3+1] = n.y;
            normaldata[p1_idx * 3+2] = n.z;

    		normaldata[p2_idx * 3] = n.x;
            normaldata[p2_idx * 3 + 1] = n.y;
            normaldata[p2_idx * 3 + 2] = n.z;

    		normaldata[p3_idx * 3] = n.x;
            normaldata[p3_idx * 3 + 1] = n.y;
            normaldata[p3_idx * 3 + 2] = n.z;
    		
    	}

        return newmesh;
    }
	

    ProceduralMesh* CreateCircle(int numDivisions)
    {
        ProceduralMesh* newmesh = new ProceduralMesh();
        const float angle = 2.f * PI / numDivisions;
    	
        for(int i = 0; i <= numDivisions; i++)
        {
            glm::vec3 point{cos(angle*i), sin(angle*i), 0.f};
            newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.x);
            newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.y);
            newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.z);
            point ={ cos(angle * (i+1)), sin(angle * (i+1)), 0.f };
            newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.x);
            newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.y);
            newmesh->m_MeshDataByVertexNormal.m_Vertexs.push_back(point.z);
        }

        return newmesh;
    }


    void BuildIndexBuffer(int numDivisions, ProceduralMesh* preceduralmesh)
    {
        size_t i0 = 0, i1 = 0, i2 = 0;
       const size_t numDiv = numDivisions;
        for (int i = 0; i < numDiv; ++i)
        {
            for (int j = 0; j < numDiv; ++j)
            {
                i0 = i * (numDiv + 1) + j;
                i1 = i0 + 1;
                i2 = i1 + (1 + numDiv);

				const auto& vertex =  preceduralmesh->m_MeshDataByVertexNormal.m_Vertexs;
            	
                glm::vec3 i0_convert = { vertex[3 * i0], vertex[3 * i0 + 1],vertex[3 * i0 + 2] };
                glm::vec3 i1_convert = { vertex[3 * i1], vertex[3 * i1 + 1],vertex[3 * i1 + 2] };
                glm::vec3 i2_convert = { vertex[3 * i2], vertex[3 * i2 + 1],vertex[3 * i2 + 2] };
            	
            	
                if (!MathLibrary::DegenerateTri(i0_convert, i1_convert, i2_convert))
                {
                    addIndex(preceduralmesh, static_cast<int>(i0));
                    addIndex(preceduralmesh, static_cast<int>(i2));
                    addIndex(preceduralmesh, static_cast<int>(i1));
                }

                i1 = i2;
                i2 = i1 - 1;
            	
                i0_convert = { vertex[3 * i1], vertex[3 * i1 + 1],vertex[3 * i1 + 2] };
                i1_convert = { vertex[3 * i2], vertex[3 * i2 + 1],vertex[3 * i2 + 2] };
                i2_convert = { vertex[3 * i0], vertex[3 * i0 + 1],vertex[3 * i0 + 2] };
            	
                if (!MathLibrary::DegenerateTri(i0_convert, i1_convert, i2_convert))
                {
                    addIndex(preceduralmesh, static_cast<int>(i0));
                    addIndex(preceduralmesh, static_cast<int>(i2));
                    addIndex(preceduralmesh, static_cast<int>(i1));
                }
            }
        }
    }


    void addIndex(ProceduralMesh* preceduralmesh, int index)
    {
        preceduralmesh->m_MeshDataByVertexNormal.m_Faces.push_back(index);
        preceduralmesh->m_MeshDataByFaceNormal.m_Faces.push_back(index);
    	
    }

}