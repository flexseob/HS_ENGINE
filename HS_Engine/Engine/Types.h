/* S/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/
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