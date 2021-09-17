#include "PreceduralMesh.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


void BuildIndexBuffer(int stacks, int slices, PreceduralMesh& preceduralmesh);
void addVertex(PreceduralMesh& preceduralmesh, const Vertex& v);
void addIndex(PreceduralMesh& preceduralmesh, int index);


PreceduralMesh CreatePlane(int stacks, int slices)
{
    PreceduralMesh newmesh;

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

    BuildIndexBuffer(stacks, slices, newmesh);

    return newmesh;
}



PreceduralMesh CreateCube(int stacks, int slices)
{
    PreceduralMesh planeMesh = CreatePlane(stacks, slices);
    PreceduralMesh newmesh;

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
        Mat4 transformMat = Translate(translateArray[i]) *
            Rotate(rotateArray[i][YINDEX], { 0,1,0 }) *
            Rotate(rotateArray[i][XINDEX], { 1,0,0 });

        for (int j = 0; j < planeMesh.numVertices; ++j)
        {
            Vertex v;
            v.pos = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].pos, 1.0));
            v.nrm = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].nrm, 1.0));
            v.uv = planeMesh.vertexBuffer[j].uv;

            v.pos = RoundDecimal(v.pos);
            v.nrm = RoundDecimal(v.nrm);

            addVertex(newmesh, v);
        }

        for (unsigned int j = 0; j < planeMesh.numIndices; ++j)
            addIndex(newmesh, planeMesh.indexBuffer[j] + planeMesh.numVertices * i);
    }

    return newmesh;
}


PreceduralMesh CreateSphere(int stacks, int slices)
{
    PreceduralMesh newmesh;
    const float Radius{ 0.5f };
    const float AlphaDelta = 2.f * PI / slices;
    const float BetaDelta = PI/stacks;

    for (int stack{}; stack <= stacks; ++stack)
    {
        float beta = 0.5f * PI - BetaDelta * stack;
        for (int slice{}; slice <= slices; ++slice)
        {
            float alpha = AlphaDelta*slice;
            Vertex v;

            v.pos.x = Radius* sin(alpha)* cos(beta);
            v.pos.y = Radius * sin(beta);
            v.pos.z = Radius * cos(alpha)* cos(beta);
            v.nrm = Normalize(v.pos);
            v.uv = Vec2((float)slice/slices,(float)stack/stacks);

            addVertex(newmesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, newmesh);

    return newmesh;
}



PreceduralMesh CreateCylinder(int stacks, int slices)
{
    PreceduralMesh newmesh;
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
            v.nrm = Vec3(v.pos.x/0.5f,0.f,v.pos.z /0.5f);
            v.uv = Vec2(row, col);

            addVertex(newmesh, v);
        }
    	
    }
    BuildIndexBuffer(stacks, slices, newmesh);

	
    return newmesh;
}

PreceduralMesh CreateTorus(int stacks, int slices, float , float)
{
    const float Rad = 0.9f;
    const float rad = 0.5f;
    PreceduralMesh newmesh;

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

    BuildIndexBuffer(stacks, slices, newmesh);

    return newmesh;
}

PreceduralMesh CreateCone(int stacks, int slices)
{
    //@todo: IMPLEMENT ME
    //REMOVE THIS
    return CreateCube(stacks, slices);
}


void BuildIndexBuffer(int stacks, int slices, PreceduralMesh& preceduralmesh)
{
    int i0 = 0, i1 = 0, i2 = 0;

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            i0 = i * (slices + 1) + j;
            i1 = i0 + 1;
            i2 = i1 + (1+slices);

            if (!DegenerateTri(preceduralmesh.vertexBuffer[i0].pos, preceduralmesh.vertexBuffer[i1].pos,preceduralmesh.vertexBuffer[i2].pos))
            {
                addIndex(preceduralmesh, i0);
                addIndex(preceduralmesh, i2);
                addIndex(preceduralmesh, i1);
            }

            i1 = i2;
            i2 = i1 - 1;

            if (!DegenerateTri(preceduralmesh.vertexBuffer[i0].pos, preceduralmesh.vertexBuffer[i1].pos,preceduralmesh.vertexBuffer[i2].pos))
            {
                addIndex(preceduralmesh, i0);
                addIndex(preceduralmesh, i2);
                addIndex(preceduralmesh, i1);
            }
        }
    }
}



void addVertex(PreceduralMesh& preceduralmesh, const Vertex& v)
{
    preceduralmesh.vertexBuffer.push_back(v);
    ++preceduralmesh.numVertices;
}



void addIndex(PreceduralMesh& preceduralmesh, int index)
{
    preceduralmesh.indexBuffer.push_back(index);
    ++preceduralmesh.numIndices;

    if (preceduralmesh.numIndices % 3 == 0)
        ++preceduralmesh.numTris;
}