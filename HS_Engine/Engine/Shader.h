/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/

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
		void SetUniformBlockBinding(const std::string& variable, unsigned int binding_num) const;
		int GetUnformSizeByLocation(unsigned index);
		unsigned int GetShaderID()const { return m_ShaderID; }
		void SetIsDebugShader(bool IsDebug);
		bool GetIsDebugShader() const;
	private:
		unsigned int m_ShaderID;
		bool mIsDebugShader = false;

		// 0 :vert 1:frag 2:geom
		std::vector<std::pair<E_ShaderTypes,std::string>> m_ShaderPath;
		std::vector<unsigned int> m_ShaderIDs;
		//std::unordered_map<unsigned int, std::string> m_UniformIDs;
		std::unordered_map<std::string, unsigned int> m_UniformIDs;
		std::unordered_map< std::string, unsigned int > m_UniformBlockID;
	};

}
