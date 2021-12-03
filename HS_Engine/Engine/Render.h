/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Render.h
Purpose : For Render the Mesh
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

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
		static void ClearColorBufferScreen();

		static void DrawSolidLine(std::shared_ptr<VertexArray> vertex_array);
		static void DrawIndexed(std::shared_ptr<VertexArray> vertex_array);
		static void DrawIndexedQuad(std::shared_ptr<VertexArray> vertex_array);
		static void DrawSolid(std::shared_ptr<VertexArray> vertex_array);
		static void DrawPatches(std::shared_ptr<VertexArray>vertex_array);
		static void DrawLine(std::shared_ptr<VertexArray> vertex_array);

	};
}
