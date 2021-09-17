/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Semester: Spring 2021
Project: CS250
Primary Author: Byeongkyu Park
-----------------------------------------------------------------*/

#define _USE_MATH_DEFINES 
#include "PerlinNoise.h"

#include <cmath> 
#include <cstdio> 
#include <random> 
#include <functional> 
#include <fstream> 
#include <algorithm>
#include "MathLibrary.h"
#include "PerlinNoise.h"


EndGame_Engine::PolyMesh* EndGame_Engine::createPolyMesh(uint32_t width, uint32_t height, uint32_t subdivisionWidth, uint32_t subdivisionHeight)
{
	PolyMesh* poly = new PolyMesh;
	poly->numVertices = (subdivisionWidth + 1) * (subdivisionHeight + 1);
	std::cerr << poly->numVertices << std::endl;
	poly->vertices = new Vector[poly->numVertices];
	poly->normals = new Vector[poly->numVertices];
	poly->st = new Vector[poly->numVertices];
	float invSubdivisionWidth = 1.f / subdivisionWidth;
	float invSubdivisionHeight = 1.f / subdivisionHeight;
	for (uint32_t j = 0; j <= subdivisionHeight; ++j) {
		for (uint32_t i = 0; i <= subdivisionWidth; ++i) {
            size_t vertIndex = j * (static_cast<size_t>(subdivisionWidth) + 1) + i;
            size_t stIndex = j * (static_cast<size_t>(subdivisionWidth) + 1) + i;
            Vector vert(static_cast<float>(width) * ((float)i * invSubdivisionWidth - 0.5f), 0.f, static_cast<float>(height) * ((float)j * invSubdivisionHeight - 0.5f));
            Vector st(static_cast<float>(i) * invSubdivisionWidth, static_cast<float>(j) * invSubdivisionHeight);
            poly->vertices[vertIndex] = vert;
            poly->st[stIndex] = st;
		}
		std::cerr << std::endl;
	}

	poly->numFaces = subdivisionWidth * subdivisionHeight;
	poly->faceArray = new uint32_t[poly->numFaces];
	for (uint32_t i = 0; i < poly->numFaces; ++i)
		poly->faceArray[i] = 4;

	poly->verticesArray = new uint32_t[4 * static_cast<size_t>(poly->numFaces)];
	for (uint32_t j = 0, k = 0; j < subdivisionHeight; ++j) {
		for (uint32_t i = 0; i < subdivisionWidth; ++i) {
            uint32_t v1= static_cast<uint32_t>((int)j * ((int)subdivisionWidth + 1) + i);
            uint32_t v2= static_cast<uint32_t>(j * (subdivisionWidth + 1) + i + 1);
            uint32_t v3= static_cast<uint32_t>((j + 1) * (subdivisionWidth + 1) + i + 1);
            uint32_t v4= static_cast<uint32_t>((j + 1) * (subdivisionWidth + 1) + i);
            poly->verticesArray[static_cast<size_t>(k)] = v1;
            poly->verticesArray[k + 1] = v2;
            poly->verticesArray[k + 2] = v3;
            poly->verticesArray[k + 3] = v4;
			k += 4;
		}
	}

	return poly;
}
EndGame_Engine::PerlinNoise::PerlinNoise(const unsigned& seed)
{
    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> distribution;
    auto dice = std::bind(distribution, generator);
    for (unsigned i = 0; i < tableSize; ++i) {
        float theta = acos(2 * dice() - 1);
        float phi = 2 * dice() * PI;

        float x = cos(phi) * sin(theta);
        float y = sin(phi) * sin(theta);
        float z = cos(theta);
        gradients[i] = Vector(x, y, z);
        permutationTable[i] = i;
    }

    std::uniform_int_distribution<unsigned> distributionInt;
    auto diceInt = std::bind(distributionInt, generator);
    for (unsigned i = 0; i < tableSize; ++i) {
        std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]);
    }
    for (unsigned i = 0; i < tableSize; ++i) {
        permutationTable[tableSize + i] = permutationTable[i];
    }
}

float EndGame_Engine::PerlinNoise::eval(const Vector& p, Vector& derivs) const
{
    int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x - ((int)std::floor(p.x));
    float ty = p.y - ((int)std::floor(p.y));
    float tz = p.z - ((int)std::floor(p.z));

    float u = quintic(tx);
    float v = quintic(ty);
    float w = quintic(tz);

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
    float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
    float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
    float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
    float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
    float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
    float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
    float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

    float du = quinticDeriv(tx);
    float dv = quinticDeriv(ty);
    float dw = quinticDeriv(tz);

    float k0 = a;
    float k1 = (b - a);
    float k2 = (c - a);
    float k3 = (e - a);
    float k4 = (a + d - b - c);
    float k5 = (a + f - b - e);
    float k6 = (a + g - c - e);
    float k7 = (b + c + e + h - a - d - f - g);

    derivs.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
    derivs.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
    derivs.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

    return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
}

float EndGame_Engine::PerlinNoise::eval(const Vector& p) const
{
    int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x - ((int)std::floor(p.x));
    float ty = p.y - ((int)std::floor(p.y));
    float tz = p.z - ((int)std::floor(p.z));

    float u = smoothstep(tx);
    float v = smoothstep(ty);
    float w = smoothstep(tz);

    // gradients at the corner of the cell
    const Vector& c000 = gradients[hash(xi0, yi0, zi0)];
    const Vector& c100 = gradients[hash(xi1, yi0, zi0)];
    const Vector& c010 = gradients[hash(xi0, yi1, zi0)];
    const Vector& c110 = gradients[hash(xi1, yi1, zi0)];

    const Vector& c001 = gradients[hash(xi0, yi0, zi1)];
    const Vector& c101 = gradients[hash(xi1, yi0, zi1)];
    const Vector& c011 = gradients[hash(xi0, yi1, zi1)];
    const Vector& c111 = gradients[hash(xi1, yi1, zi1)];

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    Vector p000 = Vector(x0, y0, z0);
    Vector p100 = Vector(x1, y0, z0);
    Vector p010 = Vector(x0, y1, z0);
    Vector p110 = Vector(x1, y1, z0);

    Vector p001 = Vector(x0, y0, z1);
    Vector p101 = Vector(x1, y0, z1);
    Vector p011 = Vector(x0, y1, z1);
    Vector p111 = Vector(x1, y1, z1);

    // linear interpolation
    float a = lerp(dot(c000, p000), dot(c100, p100), u);
    float b = lerp(dot(c010, p010), dot(c110, p110), u);
    float c = lerp(dot(c001, p001), dot(c101, p101), u);
    float d = lerp(dot(c011, p011), dot(c111, p111), u);

    float e = lerp(a, b, v);
    float f = lerp(c, d, v);

    return lerp(e, f, w); // g 
}

uint8_t EndGame_Engine::PerlinNoise::hash(const int& x, const int& y, const int& z) const {
    return static_cast<uint8_t>(permutationTable[permutationTable[permutationTable[x] + y] + z]);
}

float EndGame_Engine::PerlinNoise::gradientDotV(uint8_t perm, float x, float y, float z) const
{
    switch (perm & 15) {
    case  0: return  x + y; // (1,1,0) 
    case  1: return -x + y; // (-1,1,0) 
    case  2: return  x - y; // (1,-1,0) 
    case  3: return -x - y; // (-1,-1,0) 
    case  4: return  x + z; // (1,0,1) 
    case  5: return -x + z; // (-1,0,1) 
    case  6: return  x - z; // (1,0,-1) 
    case  7: return -x - z; // (-1,0,-1) 
    case  8: return  y + z; // (0,1,1), 
    case  9: return -y + z; // (0,-1,1), 
    case 10: return  y - z; // (0,1,-1), 
    case 11: return -y - z; // (0,-1,-1) 
    case 12: return  y + x; // (1,1,0) 
    case 13: return -x + y; // (-1,1,0) 
    case 14: return -y + z; // (0,-1,1) 
    case 15: return -y - z; // (0,-1,-1) 
    default:    return 0.f;
    }
}

