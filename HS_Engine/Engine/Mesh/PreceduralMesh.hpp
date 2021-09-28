
#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "../MathLibrary.h"
#include "Mesh.h"
namespace HS_Engine
{
    struct Vertex
    {
        Vertex(Vec3 pos, Vec3 nrm, Vec3 uv) : pos(pos), nrm(nrm), uv(uv) {}

        Vertex() { }

        Vec3 pos = Vec3(), nrm = Vec3();
        Vec2 uv = Vec2();
    };

    const int vertexSize = sizeof(Vertex);
    const int indexSize = sizeof(int);

    typedef std::vector<Vertex> VertexBufferType;
    typedef std::vector<unsigned> IndexBufferType;

    class PreceduralMesh : public Mesh
    {
    public:
        PreceduralMesh() : numVertices(0), numTris(0), numIndices(0) { }

        
        VertexBufferType vertexBuffer;
        IndexBufferType indexBuffer;
        


        int numVertices;
        int numTris;
        unsigned int numIndices;

    };

    PreceduralMesh* CreatePlane(int stacks, int slices);
    PreceduralMesh* CreateCube(int stacks, int slices);
    //PreceduralMesh* CreateSphere(int stacks, int slices);
    PreceduralMesh* CreateSphere(const float Radius, int slices);
    PreceduralMesh* CreateTorus(int stacks, int slices, float startAngle, float endAngle);
    PreceduralMesh* CreateCylinder(int stacks, int slices);
    PreceduralMesh* CreateCircle(int stacks);

}

#endif