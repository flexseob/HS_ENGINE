#pragma once
#include <memory>
#include <vector>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include "../VertexArrayData.h"
namespace HS_Engine
{
	class Shader;
	class ObjectVertex;


	class Mesh
	{
	public:
		~Mesh() = default;
		void InitBuffer();
		void ClearData();
		void PreRender();
		void Render();
		void PostRender();
		void SetIndexBufferCount(unsigned int count);
		void SetPosition(glm::vec3 position);
		void SetScale(glm::vec3 scale);
		void SetRotation(float angle, glm::vec3 axis);
		void SetModelMatrixtoShader(std::shared_ptr<Shader> shader);
		void SetObjectColor(glm::vec3 color);
		
		std::vector<float> m_Vertexs;
		std::vector<float> m_Normals;
		std::vector<float> m_TexCoords;
		std::vector<unsigned int> m_Faces;

	private:

		unsigned int m_Indices;
		std::shared_ptr<VertexArray> m_VertexArray;

		glm::mat4 m_ModelMat;
		glm::vec3 m_Position;
		glm::vec3 m_Scale =glm::vec3{1.f};
		glm::vec3 m_RotateAxis =glm::vec3{0.f,0.f,1.f};
		glm::vec3 m_ObjectColor;
		float m_angle;
	};



	
}
