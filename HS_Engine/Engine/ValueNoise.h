

#pragma once
#include <cmath> 
#include <cstdio> 
#include <random> 
#include <functional> 
#include <iostream> 
#include <fstream>
#include "MathLibrary.h"
#include "Core/Affine.h"

namespace HS_Engine
{
    class ValueNoise
    {
    public:
        ValueNoise(unsigned seed = 2016)
        {
            std::mt19937 gen(seed);
            std::uniform_real_distribution<float> distrFloat;
            auto randFloat = std::bind(distrFloat, gen);

            for (unsigned k = 0; k < kMaxTableSize; ++k) {
                r[k] = randFloat();
                permutationTable[k] = k;
            }

            std::uniform_int_distribution<unsigned> distrUInt;
            auto randUInt = std::bind(distrUInt, gen);
            for (unsigned k = 0; k < kMaxTableSize; ++k) {
                unsigned i = randUInt() & kMaxTableSizeMask;
                std::swap(permutationTable[k], permutationTable[i]);
                permutationTable[k + kMaxTableSize] = permutationTable[k];
            }
        }

        float eval(Vector& p) const
        {
            int xi = static_cast<int>(std::floor(p.x));
            int yi = static_cast<int>(std::floor(p.y));

            float tx = p.x - xi;
            float ty = p.y - yi;

            int rx0 = xi & kMaxTableSizeMask;
            int rx1 = (rx0 + 1) & kMaxTableSizeMask;
            int ry0 = yi & kMaxTableSizeMask;
            int ry1 = (ry0 + 1) & kMaxTableSizeMask;

            const float& c00 = r[permutationTable[permutationTable[rx0] + ry0]];
            const float& c10 = r[permutationTable[permutationTable[rx1] + ry0]];
            const float& c01 = r[permutationTable[permutationTable[rx0] + ry1]];
            const float& c11 = r[permutationTable[permutationTable[rx1] + ry1]];

            float sx = smoothstep(tx);
            float sy = smoothstep(ty);

            float nx0 = lerp(c00, c10, sx);
            float nx1 = lerp(c01, c11, sx);

            return lerp(nx0, nx1, sy);
        }
        static const unsigned kMaxTableSize = 256;
        static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
        float r[kMaxTableSize];
        unsigned permutationTable[kMaxTableSize * 2];
    };
}