/* Start Header-------------------------------------------------------
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : MathLibrary.cpp
Purpose : MathLibrary class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#include "../MathLibrary.h"
namespace HS_Engine
{
	float MathLibrary::RoundDecimal(float input)
    {
	    return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
    }

    MathLibrary::Vec3 MathLibrary::RoundDecimal(const Vec3& input)
    {
	    return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
    }

   MathLibrary::Vec4 MathLibrary::RoundDecimal(const Vec4& input)
    {
	    return Vec4(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]), RoundDecimal(input[3]));
    }

    bool MathLibrary::DegenerateTri(const Vec3& v0, const Vec3& v1, const Vec3& v2)
    {
	    return (glm::distance(v0, v1) < EPSILON ||
		    glm::distance(v1, v2) < EPSILON ||
		    glm::distance(v2, v0) < EPSILON);
    }

     MathLibrary::Mat4 MathLibrary::Scale(float sx, float sy, float sz)
    {
        return glm::scale(Mat4(1.0f), Vec3(sx, sy, sz));
    }

    MathLibrary::Mat4 MathLibrary::Scale(const Vec3& s)
    {
        return glm::scale(Mat4(1.0f), s);
    }

    MathLibrary::Mat4 MathLibrary::Rotate(float angle, const Vec3& axis)
    {
        return glm::rotate(Mat4(1.0f), angle, axis);
    }

    MathLibrary::Mat4 MathLibrary::Rotate(float angle, float ax, float ay, float az)
    {
        return glm::rotate(Mat4(1.0f), angle, Vec3(ax, ay, az));
    }

    MathLibrary::Mat4 MathLibrary::Translate(float tx, float ty, float tz)
    {
        return glm::translate(Mat4(1.0f), Vec3(tx, ty, tz));
    }

     MathLibrary::Mat4 MathLibrary::Translate(const Vec3& t)
    {
        return glm::translate(Mat4(1.0f), t);
    }

    MathLibrary::Mat4 MathLibrary::Frustum(float leftPlane, float rightPlane, float bottomPlane, float topPlane,
        float nearPlane, float farPlane)
    {
        return glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
    }

    MathLibrary::Mat4 MathLibrary::LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
        return glm::lookAt(eye, center, up);
    }

    MathLibrary::Mat4 MathLibrary::LookAtOrigin(float eyeRadius, float eyeAlpha, float eyeBeta)
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
    float MathLibrary::smoothstep(const float& t)
    {
        return t * t * (3 - 2 * t);
    }

    float MathLibrary::quintic(const float& t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float MathLibrary::quinticDeriv(const float& t)
    {
        return 30 * t * t * (t * (t - 2) + 1);
    }
}