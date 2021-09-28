#include "PreceduralMesh.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace HS_Engine {
	
    void BuildIndexBuffer(int numDivisions, PreceduralMesh* preceduralmesh);
    //void BuildIndexBufferNew(int numDivisions, PreceduralMesh* preceduralmesh);
	
    void addVertex(PreceduralMesh* preceduralmesh, const Vertex& v);
    void addIndex(PreceduralMesh* preceduralmesh, int index);
    void addIndexNew(PreceduralMesh* preceduralmesh, int index);

    PreceduralMesh* CreatePlane(int stacks, int slices)
    {
        PreceduralMesh* newmesh = new PreceduralMesh();

        for (int stack = 0; stack <= stacks; ++stack)
        {
            float row = (float)stack / stacks;

            for (int slice = 0; slice <= slices; ++slice)
            {
                float col = (float)slice / slices;

                Vertex v;

                v.pos = glm::vec3(col - 0.5f, 0.5f - row, 0.0f);
                v.nrm = glm::vec3(0.0f, 0.0f, 1.0f);
                v.uv = glm::vec2(col, row);

                addVertex(newmesh, v);
            }
        }

        BuildIndexBuffer(stacks, newmesh);

        return newmesh;
    }



    PreceduralMesh* CreateCube(int stacks, int slices)
    {
        PreceduralMesh* planeMesh = CreatePlane(stacks, slices);
        PreceduralMesh* newmesh = new PreceduralMesh();

        Vec3 const translateArray[] =
        {
            Vec3(+0.0f, +0.0f, +0.5f), // Z+
            Vec3(+0.0f, +0.0f, -0.5f), // Z-
            Vec3(+0.5f, +0.0f, +0.0f), // X+
            Vec3(-0.5f, +0.0f, +0.0f), // X-
            Vec3(+0.0f, +0.5f, +0.0f), // Y+
            Vec3(+0.0f, -0.5f, +0.0f), // Y-
        };

        Vec2 const rotateArray[] =
        {
            Vec2(+0.0f, +0.0f),             // Z+
            Vec2(+0.0f, (float)+PI),        // Z-
            Vec2(+0.0f, (float)+HALF_PI),   // X+       
            Vec2(+0.0f, (float)-HALF_PI),   // X-
            Vec2((float)-HALF_PI, +0.0f),   // Y+
            Vec2((float)+HALF_PI, +0.0f)    // Y-
        };


        /*  Transform the plane to 6 positions to form the faces of the cube */
        for (int i = 0; i < 6; ++i)
        {
            glm::mat4 transformMat = Translate(translateArray[i]) *
                Rotate(rotateArray[i][YINDEX], { 0,1,0 }) *
                Rotate(rotateArray[i][XINDEX], { 1,0,0 });

            for (int j = 0; j < planeMesh->numVertices; ++j)
            {
                Vertex v;
                v.pos = Vec3(transformMat * Vec4(planeMesh->vertexBuffer[j].pos, 1.0));
                v.nrm = Vec3(transformMat * Vec4(planeMesh->vertexBuffer[j].nrm, 1.0));
                v.uv = planeMesh->vertexBuffer[j].uv;

                v.pos = RoundDecimal(v.pos);
                v.nrm = RoundDecimal(v.nrm);

                addVertex(newmesh, v);
            }

            for (unsigned int j = 0; j < planeMesh->numIndices; ++j)
                addIndex(newmesh, planeMesh->indexBuffer[j] + planeMesh->numVertices * i);
        }

        delete planeMesh;
    	
        return newmesh;
    }


    //PreceduralMesh* CreateSphere(int stacks, int slices)
    //{
    //    PreceduralMesh* newmesh = new PreceduralMesh();
    //	
    //    const float Radius{ 1.f };
    //    const float AlphaDelta = 2.f * PI / slices;
    //    const float BetaDelta = PI / stacks;

    //    for (int stack{}; stack <= stacks; ++stack)
    //    {
    //        float beta = 0.5f * PI - BetaDelta * stack;
    //        for (int slice{}; slice <= slices; ++slice)
    //        {
    //            float alpha = AlphaDelta * slice;
    //            Vertex v;

    //            v.pos.x = Radius * sin(alpha) * cos(beta);
    //            v.pos.y = Radius * sin(beta);
    //            v.pos.z = Radius * cos(alpha) * cos(beta);
    //            v.nrm = Normalize(v.pos);
    //            v.uv = Vec2((float)slice / slices, (float)stack / stacks);

    //            addVertex(newmesh, v);
    //        }
    //    }

    //    BuildIndexBuffer(stacks ,newmesh);

    //    return newmesh;
    //}

    PreceduralMesh* CreateSphere(const float Radius, int numbDivsions)
    {
        PreceduralMesh* newmesh = new PreceduralMesh();

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

               /* addVertex(newmesh, v);*/
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
    	for(int idx = 0; idx < facedata.size(); idx+=3)
    	{
            int p1_idx = facedata[idx];
            int p2_idx = facedata[idx+1];
            int p3_idx = facedata[idx+2];

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
	



    PreceduralMesh* CreateCylinder(int stacks, int slices)
    {
        PreceduralMesh* newmesh = new PreceduralMesh();
        for (int stack = 0; stack <= stacks; ++stack)
        {
            float row = float(stack) / stacks;
            for (int slice = 0; slice <= slices; ++slice)
            {
                float col = (float)slice / slices;
                float alpha = 2.0f * col * PI;
                Vertex v;
                float sinAlpha = sin(alpha);
                float cosAlpha = cos(alpha);
                v.pos = Vec3(0.5 * sinAlpha, row - 0.5, 0.5 * cosAlpha);
                v.nrm = Vec3(v.pos.x / 0.5f, 0.f, v.pos.z / 0.5f);
                v.uv = Vec2(row, col);

                addVertex(newmesh, v);
            }

        }
        BuildIndexBuffer(stacks,newmesh);


        return newmesh;
    }

    PreceduralMesh* CreateCircle(int numDivisions)
    {
        PreceduralMesh* newmesh = new PreceduralMesh();
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

    PreceduralMesh* CreateTorus(int stacks, int slices, float, float)
    {
        const float Rad = 0.9f;
        const float rad = 0.5f;
        PreceduralMesh* newmesh = new PreceduralMesh();

        for (int stack = 0; stack <= stacks; ++stack)
        {
            float row = (float)stack / stacks;
            float alpha = 2.0f * row * PI;
            float sinAlpha = sin(alpha);
            float cosAlpha = cos(alpha);


            for (int slice = 0; slice <= slices; ++slice)
            {
                float col = (float)slice / slices;
                float beta = 2.0f * col * PI;

                Vertex v;
                const float cosBeta = cos(beta);
                const float sinBeta = sin(beta);

                v.pos = Vec3((Rad + rad * cosBeta) * sinAlpha, rad * sinBeta, (Rad + rad * cosBeta) * cosAlpha);
                Vec3 c = { Rad * sinAlpha,0,Rad * cosAlpha };

                v.nrm = Vec3(glm::normalize(v.pos - c));
                v.uv = Vec2(col, row);

                addVertex(newmesh, v);
            }
        }

        BuildIndexBuffer(stacks ,newmesh);

        return newmesh;
    }

    PreceduralMesh* CreateCone(int stacks, int slices)
    {
        //@todo: IMPLEMENT ME
        //REMOVE THIS
        return CreateCube(stacks, slices);
    }


    void BuildIndexBuffer(int numDivisions, PreceduralMesh* preceduralmesh)
    {
        int i0 = 0, i1 = 0, i2 = 0;

        for (int i = 0; i < numDivisions; ++i)
        {
            for (int j = 0; j < numDivisions; ++j)
            {
                i0 = i * (numDivisions + 1) + j;
                i1 = i0 + 1;
                i2 = i1 + (1 + numDivisions);

				const auto& vertex =  preceduralmesh->m_MeshDataByVertexNormal.m_Vertexs;
            	
                glm::vec3 i0_convert = { vertex[3 * i0], vertex[3 * i0 + 1],vertex[3 * i0 + 2] };
                glm::vec3 i1_convert = { vertex[3 * i1], vertex[3 * i1 + 1],vertex[3 * i1 + 2] };
                glm::vec3 i2_convert = { vertex[3 * i2], vertex[3 * i2 + 1],vertex[3 * i2 + 2] };
            	
            	
                if (!DegenerateTri(i0_convert, i1_convert, i2_convert))
                {
                    addIndexNew(preceduralmesh, i0);
                    addIndexNew(preceduralmesh, i2);
                    addIndexNew(preceduralmesh, i1);
                }

                i1 = i2;
                i2 = i1 - 1;

           /*     i0_convert = { vertex[3 * i0], vertex[3 * i0 + 1],vertex[3 * i0 + 2] };
                i1_convert = { vertex[3 * i1], vertex[3 * i1 + 1],vertex[3 * i1 + 2] };
                i2_convert = { vertex[3 * i2], vertex[3 * i2 + 1],vertex[3 * i2 + 2] };*/
            	
                i0_convert = { vertex[3 * i1], vertex[3 * i1 + 1],vertex[3 * i1 + 2] };
                i1_convert = { vertex[3 * i2], vertex[3 * i2 + 1],vertex[3 * i2 + 2] };
                i2_convert = { vertex[3 * i0], vertex[3 * i0 + 1],vertex[3 * i0 + 2] };
            	
                if (!DegenerateTri(i0_convert, i1_convert, i2_convert))
                {
                    addIndexNew(preceduralmesh, i0);
                    addIndexNew(preceduralmesh, i2);
                    addIndexNew(preceduralmesh, i1);
                }
            }
        }
    }



    void addVertex(PreceduralMesh* preceduralmesh, const Vertex& v)
    {
        preceduralmesh->vertexBuffer.push_back(v);
        ++preceduralmesh->numVertices;
    }



    void addIndex(PreceduralMesh* preceduralmesh, int index)
    {
        preceduralmesh->indexBuffer.push_back(index);
        ++preceduralmesh->numIndices;

        if (preceduralmesh->numIndices % 3 == 0)
            ++preceduralmesh->numTris;
    }

    void addIndexNew(PreceduralMesh* preceduralmesh, int index)
    {
        preceduralmesh->m_MeshDataByVertexNormal.m_Faces.push_back(index);
        preceduralmesh->m_MeshDataByFaceNormal.m_Faces.push_back(index);
    	
    }

}