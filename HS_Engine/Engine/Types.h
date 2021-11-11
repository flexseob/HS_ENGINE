/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Type.h
Purpose : For Enum class type
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */
#pragma once

enum class E_RenderTypes
{
	NONE,
	TRIANGLES,
	TRIANGELS_STRIP,
	LINES,
};

enum class E_MeshTypes
{
	BY_PATHMESH,
	BY_PROCEDURALMESH,
};
enum class E_Proceduralmesh
{
	NONE,
	SPHERE,
	CUBE,
	PLANE,
	CYLINDER,
	CIRCLE,
};

enum class E_NormalTypes
{
	FACE,
	VERTEX,
};

enum E_LightType
{
	DIRLIGHT =0,
	POINTLIGHT,
	SPOTLIGHT,
};

enum class E_UV_Types
{
	CYLINDRICAL_UV =0,
	SPHERICAL_UV,
	CUBE_MAPPED_UV,
	DEFAULT,
};

enum class E_UV_Entity_Types
{
	VERTEX_POS =0,
	VERTEX_NORMAL,
};