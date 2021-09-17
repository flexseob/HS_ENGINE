

#pragma once
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

template<typename T = float>
inline T lerp(const T& lo, const T& hi, const T& t)
{
    return lo * (1 - t) + hi * t;
}

inline float smoothstep(const float& t)
{
    return t * t * (3 - 2 * t);
}

inline float quintic(const float& t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline float quinticDeriv(const float& t)
{
    return 30 * t * t * (t * (t - 2) + 1);
}

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;


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

const Vec3 XAXIS(1.0f, 0.0f, 0.0f);
const Vec3 YAXIS(0.0f, 1.0f, 0.0f);
const Vec3 ZAXIS(0.0f, 0.0f, 1.0f);

const int    NUM_STEPS_PI = 36;
const int    TWO_NUM_STEPS_PI = 2 * NUM_STEPS_PI;
const float  ONE_STEP = PI / NUM_STEPS_PI;



inline static Mat4 Scale(float sx, float sy, float sz)
{
    return glm::scale(Mat4(1.0f), Vec3(sx, sy, sz));
}


inline static Mat4 Scale(const Vec3& s)
{
    return glm::scale(Mat4(1.0f), s);
}


inline static Mat4 Rotate(float angle, const Vec3& axis)
{
    return glm::rotate(Mat4(1.0f), angle, axis);
}


inline static Mat4 Rotate(float angle, float ax, float ay, float az)
{
    return glm::rotate(Mat4(1.0f), angle, Vec3(ax, ay, az));
}



inline static Mat4 Translate(float tx, float ty, float tz)
{
    return glm::translate(Mat4(1.0f), Vec3(tx, ty, tz));
}


inline static Mat4 Translate(const Vec3& t)
{
    return glm::translate(Mat4(1.0f), t);
}


inline static Mat4 Frustum(float leftPlane, float rightPlane,
    float bottomPlane, float topPlane,
    float nearPlane, float farPlane)
{
    return glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
}

inline static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
    return glm::lookAt(eye, center, up);
}


inline static Mat4 LookAtOrigin(float eyeRadius, float eyeAlpha, float eyeBeta)
{
    glm::vec4 position{ eyeRadius, 0.f, 0.f, 1.f };
    glm::vec3 up{ 0.f, 1.f, 0.f };

    //Setup rotation matrix to rotate the position around the target
    glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, eyeAlpha, glm::vec3{ 0.f, 0.f, -1.f });
    rotation = rotation * glm::rotate(glm::mat4{ 1 }, eyeBeta, glm::vec3{ 0.f, 1.f, 0.f });
    position = position * rotation;

    //Use cross product to calculate correct up vector
    glm::vec3 left = glm::cross(glm::vec3{ position }, up);
    up = glm::cross(left, glm::vec3{ position });

    return LookAt(glm::vec3{ position }, Vec3(0, 0, 0), up);
}


template <typename T>
inline static T Normalize(T vec)
{
    return glm::normalize(vec);
}


template <typename T>
inline static float const* ValuePtr(T const& value)
{
    return glm::value_ptr(value);
}

inline static float RoundDecimal(float input)
{
    return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
}


inline static Vec3 RoundDecimal(const Vec3& input)
{
    return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
}

inline static Vec4 RoundDecimal(const Vec4& input)
{
    return Vec4(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]), RoundDecimal(input[3]));
}


inline static bool DegenerateTri(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}


