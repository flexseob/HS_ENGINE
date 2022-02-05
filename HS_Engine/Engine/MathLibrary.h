/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
#pragma once
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace HS_Engine {

    const static float PI = 4.0f * atan(1.0f);
    const float  TWO_PI = 2.0f * PI;
    const float  HALF_PI = 0.5f * PI;
    const float  QUARTER_PI = 0.25f * PI;
    const float  EIGHTH_PI = 0.125f * PI;
    const float  SIXTEENTH_PI = 0.0625f * PI;

    const float  DEG_TO_RAD = PI / 180.0f;
    const float  RAD_TO_DEG = 180.0f / PI;

    const float  EPSILON = 0.00001f;

    const int XINDEX = 0;
    const int YINDEX = 1;
    const int ZINDEX = 2;

    const glm::vec3 XAXIS(1.0f, 0.0f, 0.0f);
    const glm::vec3 YAXIS(0.0f, 1.0f, 0.0f);
    const glm::vec3 ZAXIS(0.0f, 0.0f, 1.0f);

    const int    NUM_STEPS_PI = 36;
    const int    TWO_NUM_STEPS_PI = 2 * NUM_STEPS_PI;
    const float  ONE_STEP = PI / NUM_STEPS_PI;


    class MathLibrary
    {
    public:
        using Vec2 = glm::vec2;
        using Vec3 = glm::vec3;
        using Vec4 = glm::vec4;
        using Mat4 = glm::mat4;

        float smoothstep(const float& t);
        float quintic(const float& t);
        float quinticDeriv(const float& t);
        static Mat4 Scale(float sx, float sy, float sz);
        static Mat4 Scale(const Vec3& s);
        static Mat4 Rotate(float angle, const Vec3& axis);
        static Mat4 Rotate(float angle, float ax, float ay, float az);
        static Mat4 Translate(float tx, float ty, float tz);
        static Mat4 Translate(const Vec3& t);
        static Mat4 Frustum(float leftPlane, float rightPlane,
                            float bottomPlane, float topPlane,
                            float nearPlane, float farPlane);
        static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
        static Mat4 LookAtOrigin(float eyeRadius, float eyeAlpha, float eyeBeta);
        static float RoundDecimal(float input);
        static Vec3 RoundDecimal(const Vec3& input);
        static Vec4 RoundDecimal(const Vec4& input);
        static bool DegenerateTri(const Vec3& v0, const Vec3& v1, const Vec3& v2);

        template <typename T>
        inline static T Normalize(T vec)
        {
            return glm::normalize(vec);
        }

        template<typename T = float>
        inline T lerp(const T& lo, const T& hi, const T& t)
        {
            return lo * (1 - t) + hi * t;
        }

        template <typename T>
        inline static float const* ValuePtr(T const& value)
        {
            return glm::value_ptr(value);
        }
    };


    
}
