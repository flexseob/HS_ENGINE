/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#ifndef MESH_H
#define MESH_H

#include <vector>
#include "../MathLibrary.h"
#include "Mesh.h"
namespace HS_Engine
{

    class ProceduralMesh : public Mesh
    {
    public:
        ProceduralMesh() {}
    };

    ProceduralMesh* CreateSphere(const float Radius, int slices);
    ProceduralMesh* CreateCircle(int stacks);

}

#endif