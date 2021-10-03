

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace HS_Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vert, const std::string& frag);
		Shader(const std::string& vert, const std::string& frag, const std::string& geom);
		Shader(const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tess,const std::string& tese);
		~Shader();
		void LinkShader();
		void BindUniformVariable(const std::string& variable, float floating_value);
		void BindUniformVariable(const std::string& variable, glm::vec2 vec2);
		void BindUniformVariable(const std::string& variable, glm::vec3 vec2);
		void BindUniformVariable(const std::string& variable, const glm::mat4& matrix);
		void BindUniformVariable(const std::string& variable, const glm::mat3& matrix);
		void BindUniformVariable(const std::string& variable, int int_value);
		void BindUniformVariable(const std::string& variable, bool bool_value);

		void Bind() const;
		void Unbind() const;
		void ErrorCode(const std::string& variable) const;

		unsigned FindUniformLocation(const std::string& name);
	private:
		unsigned int m_ShaderID;


		// 0 :vert 1:frag 2:geom
		std::vector<unsigned int> m_ShaderIDs;
		std::unordered_map<unsigned int, std::string> m_UniformIDs;
	};

}
