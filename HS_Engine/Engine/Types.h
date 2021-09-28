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
	BY_PRECEDURALMESH,
};
enum class E_Preceduralmesh
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
