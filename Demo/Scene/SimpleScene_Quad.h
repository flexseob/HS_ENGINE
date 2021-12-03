/* Start Header -------------------------------------------------------
Copyright (C) <current year in format YYYY> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SimpleScene_Quad.h
Purpose: SimpleScene_Quad Scene
Language: C++ , Microsoft Visual C++
Platform: <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project: <h.jeong_CS300_1>
class CS300 and this file is a part of assignment 1, then write: foo_CS300_1>
Author: <Hoseob Jeong, h.jeong, 180002521>
Creation date: <09/11/21>
End Header --------------------------------------------------------*/
#pragma once

#include <memory>

#include "OBJLoader.h"
#include "Shader.h"
#include "VertexArrayData.h"
#include "../../HS_Engine/Engine/Scene.h"


class SimpleScene_Quad : public HS_Engine::Scene
{
public:
	void Load() override;
	void Update(double dt) override;
	void UnLoad() override;
	void ImGuiUpdate() override;
	std::string GetSceneName() override { return "SimpleScene_Quad"; }


	private:
	HS_Engine::ObjectLoader m_objectloader;
		std::shared_ptr <HS_Engine::VertexArray> m_VertexArray;
		std::shared_ptr <HS_Engine::Shader> m_Shader;
		GLfloat angleOfRotation = 0.f;
};