#include "SimpleScene_Quad.h"
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine.h"
#include "Render.h"

void SimpleScene_Quad::Load()
{
	m_Shader = std::make_shared<HS_Engine::Shader>("Common/shaders/QuadVertexShader.vert", "Common/shaders/QuadFragmentShader.frag");
	m_Shader->LinkShader();

	
	m_VertexArray = std::make_shared<HS_Engine::VertexArray>();

	
	
	std::vector <float>vertices = { 0.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f
};

	std::shared_ptr<HS_Engine::VertexBuffer> vertexBuffer = std::make_shared<HS_Engine::VertexBuffer>(reinterpret_cast<float*>(vertices.data()), vertices.size() *sizeof(float));
	vertexBuffer->DescribeData({{HS_Engine::DataType::Float3,0}});


	m_VertexArray->AddVertexBuffer(vertexBuffer);
	
	std::shared_ptr<HS_Engine::IndexBuffer> indexBuffer = std::make_shared<HS_Engine::IndexBuffer>(6);


	m_VertexArray->AddIndexBuffer(indexBuffer);
	m_Shader->FindUniformLocation("vertexTransform");
	HS_Engine::Render::SetClearColor({0.5f, 0.5f, 0.5f, 1.0f});
	HS_Engine::Engine::Instance().SetFPS(120.f);
}

void SimpleScene_Quad::Update(double dt)
{
	HS_Engine::Render::ClearScreen();
	glm::mat4 modelMat = glm::mat4(1.0f);
	glm::vec3 scaleVector = glm::vec3(1.0f);
	glm::vec3 centroid = glm::vec3(-0.5f, -0.5f, 0.0f);

	modelMat = glm::rotate(angleOfRotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(scaleVector) *
		glm::translate(centroid);
	m_Shader->BindUniformVariable("vertexTransform", modelMat);
	m_Shader->Bind();
	m_VertexArray->Bind();
	HS_Engine::Render::DrawSolid(m_VertexArray);
	
	angleOfRotation += 0.01f;
	
}

void SimpleScene_Quad::UnLoad()
{
	m_VertexArray->ClearIndexBuffer();
	m_VertexArray->ClearVertexBuffer();
	
}

void SimpleScene_Quad::ImGuiUpdate()
{

	
}
