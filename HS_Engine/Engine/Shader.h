/* Start Header------------------------------------------------------ -
Copyright(C) < 2021 > DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name : Shader.h
Purpose : For Shader Compile and link and settings
Language : C++, Microsoft Visual C++
Platform : <Microsoft Visual C++ 19.29.30037, hardware requirements, Windows 10>
Project : <h.jeong_CS300_1>
Author : <Hoseob Jeong, h.jeong, 180002521>
Creation date : <09 / 11 / 21>
End Header-------------------------------------------------------- */

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace HS_Engine
{
	enum class E_ShaderTypes
	{
		VERTEX,
		FRAGMENT,
		GEOMERTY,
		TESC,
		TESE,
		NONE,
	};
	
	class Shader
	{
	public:
		Shader(const std::string& vert, const std::string& frag);
		Shader(const std::string& vert, const std::string& frag, const std::string& geom);
		Shader(const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tesc,const std::string& tese);
		~Shader();
		void LinkShader();
		void CompileShader(E_ShaderTypes shadertype, const std::string& path);
		void BindUniformVariable(const std::string& variable, float floating_value);
		void BindUniformVariable(const std::string& variable, glm::vec2 vec2);
		void BindUniformVariable(const std::string& variable, glm::vec3 vec3);
		void BindUniformVariable(const std::string& variable, const glm::mat4& matrix);
		void BindUniformVariable(const std::string& variable, const glm::mat3& matrix);
		void BindUniformVariable(const std::string& variable, int int_value);
		void BindUniformVariable(const std::string& variable, bool bool_value);
		void ReloadShader();
		
		void Bind() const;
		void Unbind() const;
		void ErrorCode(const std::string& variable) const;

		unsigned FindUniformLocation(std::string name);
		unsigned FindUniformBlockIndex(const std::string& name);
		int GetUnformSizeByLocation(unsigned index);
		unsigned int GetShaderID()const { return m_ShaderID; }
	private:
		unsigned int m_ShaderID;


		// 0 :vert 1:frag 2:geom
		std::vector<std::pair<E_ShaderTypes,std::string>> m_ShaderPath;
		std::vector<unsigned int> m_ShaderIDs;
		//std::unordered_map<unsigned int, std::string> m_UniformIDs;
		std::unordered_map<std::string, unsigned int> m_UniformIDs;
		std::unordered_map<unsigned int, std::string> m_UniformBlockID;
	};

}
