/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Semester: Spring 2021
Project: CS250
Primary Author: Byeongkyu Park
-----------------------------------------------------------------*/

#pragma once
#define _USE_MATH_DEFINES 
#include <iostream> 
#include "Core/Affine.h"

namespace EndGame_Engine
{
    class PerlinNoise
    {
    public:
        PerlinNoise(const unsigned& seed = 2016);
        virtual ~PerlinNoise() {}
        float eval(const Vector& p, Vector& derivs) const;
        float eval(const Vector& p) const;

    private:
        uint8_t hash(const int& x, const int& y, const int& z) const;
        float gradientDotV(uint8_t perm, float x, float y, float z) const;
        static const unsigned tableSize = 256;
        static const unsigned tableSizeMask = tableSize - 1;
        Vector gradients[tableSize];
        unsigned permutationTable[tableSize * 2];
};
    class PolyMesh
    {
    public:
	    PolyMesh() : vertices(nullptr), st(nullptr), normals(nullptr) {}
	    ~PolyMesh()
	    {
            if (vertices!=nullptr) {
                delete[] vertices;
            }
            if (st!=nullptr) {
                delete[] st;
            }
            if (normals!=nullptr) {
                delete[] normals;
            }
	    }
	    Vector* vertices{ nullptr };
	    Vector* st{ nullptr };
        Vector* normals{nullptr};
        uint32_t* faceArray{nullptr};
        uint32_t* verticesArray{nullptr};
        uint32_t numVertices{};
        uint32_t numFaces{};
    };
    PolyMesh* createPolyMesh(uint32_t width = 1,uint32_t height = 1,uint32_t subdivisionWidth = 40,uint32_t subdivisionHeight = 40);
}