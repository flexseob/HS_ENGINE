/* Start Header -------------------------------------------------------
Copyright (C) <2021> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ProceduralMesh.h
Purpose: For ProceduralMesh create
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_1>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <09/11/21>
End Header --------------------------------------------------------*/
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