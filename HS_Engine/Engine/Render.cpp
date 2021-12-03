/* Start Header-------------------------------------------------------
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Render.cpp
Purpose : Render class source file
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

#include "Render.h"

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void HS_Engine::Render::Init()
{
	glViewport(0, 0, 1600, 900);
}

void HS_Engine::Render::Shutdown()
{

}

void HS_Engine::Render::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void HS_Engine::Render::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}

void HS_Engine::Render::ClearColorBufferScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void HS_Engine::Render::DrawSolidLine(std::shared_ptr<VertexArray> vertex_array)
{
	glDrawArrays(GL_LINE_STRIP, 0, vertex_array->GetIndexBuffers()->GetCount());
}

void HS_Engine::Render::DrawIndexed(std::shared_ptr<VertexArray> vertex_array)
{
	glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffers()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void HS_Engine::Render::DrawIndexedQuad(std::shared_ptr<VertexArray> vertex_array)
{
	glDrawElements(GL_QUADS, vertex_array->GetIndexBuffers()->GetCount(), GL_UNSIGNED_INT, nullptr);
}


void HS_Engine::Render::DrawSolid(std::shared_ptr<VertexArray> vertex_array)
{
	glDrawArrays(GL_TRIANGLES, 0, vertex_array->GetIndexBuffers()->GetCount());
}


void HS_Engine::Render::DrawPatches(std::shared_ptr<VertexArray> vertex_array)
{
	glDrawElements(GL_PATCHES,vertex_array->GetIndexBuffers()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void HS_Engine::Render::DrawLine(std::shared_ptr<VertexArray> vertex_array)
{
	glDrawArrays(GL_LINES, 0, vertex_array->GetIndexBuffers()->GetCount());
}

