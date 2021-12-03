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

namespace HS_Engine
{
	class Texture;
}
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

enum class E_CUBE_MAP
{
	TOP = 0,
	LEFT,
	FRONT,
	RIGHT,
	BACK,
	BOTTOM,
};
enum class E_Current_Shader
{
	PHONG_SHADING = 0,
	PHONG_LIGHTING,
	BLINN_SHADING,
};

enum class E_Environment_Mapping
{
	REFLECTION = 0,
	REFRACTION,
	MIX,
};

struct CUBEMAP_INFO
{
	E_CUBE_MAP Position;
	HS_Engine::Texture* texture;
};

enum E_MeshType
{
	PATH, Precedural,
};