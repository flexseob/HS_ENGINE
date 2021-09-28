

#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "VertexArrayData.h"

namespace HS_Engine
{
	class Render
	{
	public:
		static void Init();
		static void Shutdown();
		static void SetClearColor(const glm::vec4& color);
		static void ClearScreen();

		//static void DrawBegin(glm::mat4& mat);
		static void DrawSolidLine(std::shared_ptr<VertexArray> vertex_array);
		static void DrawIndexed(std::shared_ptr<VertexArray> vertex_array);
		static void DrawIndexedQuad(std::shared_ptr<VertexArray> vertex_array);
		static void DrawSolid(std::shared_ptr<VertexArray> vertex_array);
		static void DrawPatches(std::shared_ptr<VertexArray>vertex_array);
		static void DrawLine(std::shared_ptr<VertexArray> vertex_array);
		//static void DrawEnd();
	};
}
