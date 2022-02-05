/* Start Header------------------------------------------------------ -
Hoseob Jeong
End Header--------------------------------------------------------*/

#include "Shader.h"
#include <fstream>
#include <iostream>
#include <GL/glew.h>

namespace HS_Engine
{
	Shader::Shader(const std::string& vert, const std::string& frag)
	{
		m_ShaderID = glCreateProgram();
		m_ShaderIDs.resize(2);
		GLint comp_result;


		std::ifstream vert_stream(vert);
		std::string vert_src;

		if (vert_stream.is_open())
		{
			vert_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(vert_stream.tellg());

			vert_src.resize(size);

			vert_stream.seekg(0, std::ios::beg);

			vert_stream.read(vert_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open vertex shader source file!");
		}
		vert_stream.close();
		m_ShaderPath.push_back({E_ShaderTypes::VERTEX,vert });
		const char* vert_cstring = vert_src.c_str();
		m_ShaderIDs[0] = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_ShaderIDs[0], 1, &vert_cstring, nullptr);
		glCompileShader(m_ShaderIDs[0]);

		glGetShaderiv(m_ShaderIDs[0], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "vertex shader compilation failed\n";
		}
		
		std::ifstream frag_stream(frag);
	
		std::string frag_src;

		if (frag_stream.is_open())
		{
			frag_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(frag_stream.tellg());

			frag_src.resize(size);

			frag_stream.seekg(0, std::ios::beg);

			frag_stream.read(frag_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open fragment shader source file!");
		}
		frag_stream.close();
		const char* frag_cstring = frag_src.c_str();
		m_ShaderIDs[1] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_ShaderIDs[1], 1, &frag_cstring, nullptr);
		glCompileShader(m_ShaderIDs[1]);
		
		glGetShaderiv(m_ShaderIDs[1], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "fragment shader compilation failed\n";
		}
		m_ShaderPath.push_back({E_ShaderTypes::FRAGMENT,frag });
	}


	Shader::Shader(const std::string& vert, const std::string& frag, const std::string& geom)
	{
		m_ShaderID = glCreateProgram();
		m_ShaderIDs.resize(3);
		GLint comp_result;
		
		std::ifstream vert_stream(vert);
		std::string vert_src;
		if (vert_stream.is_open())
		{
			vert_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(vert_stream.tellg());

			vert_src.resize(size);

			vert_stream.seekg(0, std::ios::beg);

			vert_stream.read(vert_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open vertex shader source file!");
		}
		vert_stream.close();

		const char* vert_cstring = vert_src.c_str();
		m_ShaderIDs[0] = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_ShaderIDs[0], 1, &vert_cstring, 0);
		glCompileShader(m_ShaderIDs[0]);

		
		glGetShaderiv(m_ShaderIDs[0], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "vertex shader compilation failed\n";
		}
		m_ShaderPath.push_back({ E_ShaderTypes::VERTEX,vert });
		
		std::ifstream frag_stream(frag);
		std::string frag_src;
		if (frag_stream.is_open())
		{
			frag_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(frag_stream.tellg());

			frag_src.resize(size);

			frag_stream.seekg(0, std::ios::beg);

			frag_stream.read(frag_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open fragment shader source file!");
		}
		frag_stream.close();

		const char* frag_cstring = frag_src.c_str();
		m_ShaderIDs[1] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_ShaderIDs[1], 1, &frag_cstring, 0);
		glCompileShader(m_ShaderIDs[1]);

		
		glGetShaderiv(m_ShaderIDs[1], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "fragment shader compilation failed\n";
		}

		std::ifstream geom_stream(geom);
		std::string geom_src;
		if (geom_stream.is_open())
		{
			geom_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(geom_stream.tellg());

			geom_src.resize(size);

			geom_stream.seekg(0, std::ios::beg);

			geom_stream.read(geom_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open geom shader source file!");
		}
	
		const char* geom_cstring = geom_src.c_str();
		m_ShaderIDs[2] = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(m_ShaderIDs[2], 1, &geom_cstring, 0);
		glCompileShader(m_ShaderIDs[2]);


		glGetShaderiv(m_ShaderIDs[2], GL_COMPILE_STATUS, &comp_result);
		if (comp_result == GL_FALSE)
		{
			GLint log_length = 0;
			glGetShaderiv(m_ShaderIDs[2], GL_INFO_LOG_LENGTH, &log_length);
			std::vector<char> error(log_length);
			glGetShaderInfoLog(m_ShaderIDs[2], log_length, nullptr, error.data());
			glDeleteShader(m_ShaderIDs[2]);
			m_ShaderIDs[2] = 0;
			std::string error_log = std::string(error.begin(), error.end());
			std::cout << "vertex shader compilation failed\n";
		}

		m_ShaderPath.push_back({ E_ShaderTypes::GEOMERTY,geom });

	}

	Shader::Shader(const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tesc, const std::string& tese)
	{
		m_ShaderID = glCreateProgram();
		m_ShaderIDs.resize(5);
		GLint comp_result;

		std::ifstream vert_stream(vert);
		std::string vert_src;
		if (vert_stream.is_open())
		{
			vert_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(vert_stream.tellg());

			vert_src.resize(size);

			vert_stream.seekg(0, std::ios::beg);

			vert_stream.read(vert_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open vertex shader source file!");
		}
		m_ShaderPath.push_back({ E_ShaderTypes::VERTEX , vert });

		const char* vert_cstring = vert_src.c_str();
		m_ShaderIDs[0] = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_ShaderIDs[0], 1, &vert_cstring, 0);
		glCompileShader(m_ShaderIDs[0]);

		glGetShaderiv(m_ShaderIDs[0], GL_COMPILE_STATUS, &comp_result);
		if (comp_result == GL_FALSE)
		{
			GLint log_length = 0;
			glGetShaderiv(m_ShaderIDs[0], GL_INFO_LOG_LENGTH, &log_length);
			std::vector<char> error(log_length);
			glGetShaderInfoLog(m_ShaderIDs[0], log_length, nullptr, error.data());
			glDeleteShader(m_ShaderIDs[0]);
			m_ShaderIDs[0] = 0;
			std::string error_log = std::string(error.begin(), error.end());
			std::cout << "geometry shader compilation failed\n";
		}

		std::ifstream tesc_stream(tesc);
		std::string tesc_src;
		if (tesc_stream.is_open())
		{
			tesc_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(tesc_stream.tellg());

			tesc_src.resize(size);

			tesc_stream.seekg(0, std::ios::beg);

			tesc_stream.read(tesc_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open tess_control shader source file!");
		}

		
		const char* tesc_cstring = tesc_src.c_str();
		m_ShaderIDs[1] = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(m_ShaderIDs[1], 1, &tesc_cstring, 0);
		glCompileShader(m_ShaderIDs[1]);

		glGetShaderiv(m_ShaderIDs[1], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "tess_control shader compilation failed\n";
		}
		m_ShaderPath.push_back({ E_ShaderTypes::TESC , tesc });


		std::ifstream tese_stream(tese);
		std::string tese_src;
		if (tese_stream.is_open())
		{
			tese_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(tese_stream.tellg());

			tese_src.resize(size);

			tese_stream.seekg(0, std::ios::beg);

			tese_stream.read(tese_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open tess_evaluation shader source file!");
		}

		const char* tese_cstring = tese_src.c_str();
		m_ShaderIDs[2] = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(m_ShaderIDs[2], 1, &tese_cstring, 0);
		glCompileShader(m_ShaderIDs[2]);

		glGetShaderiv(m_ShaderIDs[2], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "tess_evaluation shader compilation failed\n";
		}
		m_ShaderPath.push_back({ E_ShaderTypes::TESE , tese });

		std::ifstream geom_stream(geom);
		std::string geom_src;
		if (geom_stream.is_open())
		{
			geom_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(geom_stream.tellg());

			geom_src.resize(size);

			geom_stream.seekg(0, std::ios::beg);

			geom_stream.read(geom_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open geom shader source file!");
		}

		const char* geom_cstring = geom_src.c_str();
		m_ShaderIDs[3] = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(m_ShaderIDs[3], 1, &geom_cstring, 0);
		glCompileShader(m_ShaderIDs[3]);

		glGetShaderiv(m_ShaderIDs[3], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "geometry shader compilation failed\n";
		}
		m_ShaderPath.push_back({ E_ShaderTypes::GEOMERTY , geom });

		std::ifstream frag_stream(frag);
		std::string frag_src;
		if (frag_stream.is_open())
		{
			frag_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(frag_stream.tellg());

			frag_src.resize(size);

			frag_stream.seekg(0, std::ios::beg);

			frag_stream.read(frag_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open fragment shader source file!");
		}

		const char* frag_cstring = frag_src.c_str();
		m_ShaderIDs[4] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_ShaderIDs[4], 1, &frag_cstring, 0);
		glCompileShader(m_ShaderIDs[4]);


		glGetShaderiv(m_ShaderIDs[4], GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << "fragment shader compilation failed\n";
		}
		m_ShaderPath.push_back({ E_ShaderTypes::FRAGMENT , frag });
	




		
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderID);
		glDeleteShader(m_ShaderID);
	}

	

	
	void Shader::LinkShader()
	{


		for(auto& shader : m_ShaderIDs)
		{
			glAttachShader(m_ShaderID, shader);
		}
		

		glLinkProgram(m_ShaderID);
		GLint lnk_log;
		glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &lnk_log);
		char* error_log = new char[lnk_log];
		error_log[0] = '\0';
		for (auto& shader : m_ShaderIDs)
		{
			glDetachShader(m_ShaderID, shader);
			glDeleteShader(shader);
		}
		GLint lnk_status;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &lnk_status);
		if (GL_FALSE == lnk_status) 
		{
			std::cerr << "Failed to link shader program\n";
			std::cerr << m_ShaderPath.at(0).second <<", " << m_ShaderPath.at(1).second << std::endl;
			glGetProgramInfoLog(m_ShaderID, lnk_log, NULL, error_log);
			std::cout << error_log << std::endl;
		}
		if (lnk_log > 0)
			delete[] error_log;
	
		glValidateProgram(m_ShaderID);
		GLint is_validate = 0;
		glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, &is_validate);
		if (is_validate == GL_FALSE)
		{
			GLint log_length = 0;
			glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &log_length);
			std::vector<char> error(log_length);
			glGetProgramInfoLog(m_ShaderID, log_length, nullptr, error.data());
			throw std::runtime_error(std::string(error.begin(), error.end()));
		}


	}

	void Shader::CompileShader(E_ShaderTypes shadertype, const std::string& path)
	{
		int shaderMacro = 0;
		std::string type;
		switch(shadertype)
		{
		case E_ShaderTypes::VERTEX:
			shaderMacro = GL_VERTEX_SHADER;
			type = "Vertex";
			break;
		case E_ShaderTypes::FRAGMENT:
			shaderMacro = GL_FRAGMENT_SHADER;
			type = "Fragment";
			break;
		case E_ShaderTypes::GEOMERTY:
			shaderMacro = GL_GEOMETRY_SHADER;
			type = "Geometry";
			break;
		case E_ShaderTypes::TESE:
			shaderMacro = GL_TESS_EVALUATION_SHADER;
			type = "TessEvaluation";
			break;
		case E_ShaderTypes::TESC:
			shaderMacro = GL_TESS_CONTROL_SHADER;
			type = "TessControl";
			break;
		}
		
		GLint comp_result;
		std::ifstream shader_stream(path);
		std::string shader_src;

		if (shader_stream.is_open())
		{
			shader_stream.seekg(0, std::ios::end);

			int size = static_cast<int>(shader_stream.tellg());

			shader_src.resize(size);

			shader_stream.seekg(0, std::ios::beg);

			shader_stream.read(shader_src.data(), size);
		}
		else
		{
			throw std::runtime_error("Cannot open "+ type+" shader source file!");
		}
		shader_stream.close();

		const char* vert_cstring = shader_src.c_str();
		unsigned shader_id = glCreateShader(shaderMacro);
		glShaderSource(shader_id, 1, &vert_cstring, nullptr);
		glCompileShader(shader_id);

		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &comp_result);
		if (GL_FALSE == comp_result)
		{
			std::cout << type + " shader compilation failed\n";
		}
		m_ShaderIDs.push_back(shader_id);

		
	}

	void Shader::BindUniformVariable(const std::string& variable, float floating_value)
	{
		if(auto find = m_UniformIDs.find(variable); find != m_UniformIDs.end() && find->second != std::numeric_limits<unsigned int>::max())
		{
			glUniform1f(find->second, floating_value);
			return;
		}
	}

	void Shader::BindUniformVariable(const std::string& variable, glm::vec2 vec2)
	{
		if (auto find = m_UniformIDs.find(variable); find != m_UniformIDs.end() && find->second != std::numeric_limits<unsigned int>::max())
		{
			glUniform2f(find->second, vec2.x,vec2.y);
			return;
		}
	}
	void Shader::BindUniformVariable(const std::string& variable, glm::vec3 vec3)
	{
		if (auto find = m_UniformIDs.find(variable); find != m_UniformIDs.end() && find->second != std::numeric_limits<unsigned int>::max())
		{
			glUniform3f(find->second, vec3.x, vec3.y, vec3.z);
			return;
		}
	}
	


	void Shader::BindUniformVariable(const std::string& variable, const glm::mat4& matrix)
	{
		if (auto find = m_UniformIDs.find(variable); find != m_UniformIDs.end() && find->second != std::numeric_limits<unsigned int>::max())
		{
			glUniformMatrix4fv(find->second, 1, GL_FALSE, &matrix[0][0]);
			return;
		}
	}

	void Shader::BindUniformVariable(const std::string& variable, const glm::mat3& matrix)
	{
		if (auto find = m_UniformIDs.find(variable); find != m_UniformIDs.end() && find->second != std::numeric_limits<unsigned int>::max())
		{
			glUniformMatrix3fv(find->second, 1, GL_FALSE, &matrix[0][0]);
			return;
		}
	}
	
	void Shader::BindUniformVariable(const std::string& variable, int int_value)
	{
		if (auto find = m_UniformIDs.find(variable); find != m_UniformIDs.end() && find->second != std::numeric_limits<unsigned int>::max())
		{
			glUniform1i(find->second, int_value);
			return;
		}
	}

	void Shader::BindUniformVariable(const std::string& variable, bool bool_value)
	{
		auto find = m_UniformIDs.find(variable);
		if (auto find = m_UniformIDs.find(variable); find != m_UniformIDs.end() && find->second != std::numeric_limits<unsigned int>::max())
		{
			glUniform1i(find->second, bool_value);
			return;
		}	
	}

	void Shader::ReloadShader()
	{
		glDeleteProgram(m_ShaderID);
		m_ShaderIDs.clear();
		m_ShaderID = glCreateProgram();
		
		for(auto& shader : m_ShaderPath)
		{
			CompileShader(shader.first,shader.second);
		}
		LinkShader();

		//change the uniform variable
		std::unordered_map<std::string, unsigned> temp_uniform_ids;
		for (auto& uniformid : m_UniformIDs)
		{
			unsigned uniformlocate = glGetUniformLocation(m_ShaderID, uniformid.first.c_str());
			if (uniformlocate == std::numeric_limits<unsigned>::max())
			{
				std::cout << m_ShaderPath[0].second << "/.frag : " << uniformid.first << " doesn't exist!" << std::endl;
			}
			temp_uniform_ids.insert({ uniformid.first,uniformlocate });
		}
		
		m_UniformIDs.clear();
		m_UniformIDs = temp_uniform_ids;
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ShaderID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::ErrorCode(const std::string& variable) const
	{
		GLenum errorCode = glGetError();

		if (errorCode == GL_NO_ERROR)
			return;

		std::string error = "Unknown error";
		std::string description = "No description";

		switch (errorCode)
		{
		case GL_INVALID_ENUM:
		{
			error = "GL_INVALID_ENUM";
			description = "An unacceptable value has been specified for an enumerated argument.";
			break;
		}

		case GL_INVALID_VALUE:
		{
			error = "GL_INVALID_VALUE";
			description = "A numeric argument is out of range.";
			break;
		}

		case GL_INVALID_OPERATION:
		{
			error = "GL_INVALID_OPERATION";
			description = "The specified operation is not allowed in the current state.";
			break;
		}

		case GL_STACK_OVERFLOW:
		{
			error = "GL_STACK_OVERFLOW";
			description = "This command would cause a stack overflow.";
			break;
		}

		case GL_STACK_UNDERFLOW:
		{
			error = "GL_STACK_UNDERFLOW";
			description = "This command would cause a stack underflow.";
			break;
		}

		case GL_OUT_OF_MEMORY:
		{
			error = "GL_OUT_OF_MEMORY";
			description = "There is not enough memory left to execute the command.";
			break;
		}

		default:
		{
			error += " " + std::to_string(errorCode);
			break;
		}
		}

		std::cerr << variable <<" : "<< error << "\n   " << description << "\n\n";
	}


	
	//Find UniformLocation if it didn't add uniform ID, it will add the uniform ID
	unsigned Shader::FindUniformLocation(std::string name)
	{
		auto find = m_UniformIDs.find(name);
		if(find != m_UniformIDs.end())
		{
			return find->second;
		}
		else
		{
			unsigned uniformlocate = glGetUniformLocation(m_ShaderID, name.c_str());
			m_UniformIDs.insert({ name,uniformlocate });
			if(uniformlocate == std::numeric_limits<unsigned>::max())
			{
				std::cout << m_ShaderPath[0].second << ".vert : " << name << " doesn't exist!" << std::endl;
			}
			return uniformlocate;
		}
	}

	unsigned Shader::FindUniformBlockIndex(const std::string& name)
	{
		auto find = m_UniformBlockID.find(name);
		if(find != m_UniformBlockID.end())
		{
			return find->second;
		}
		else
		{
			unsigned uniformindex = glGetUniformBlockIndex(m_ShaderID, name.c_str());
			m_UniformBlockID.insert({name, uniformindex});
			if (uniformindex == std::numeric_limits<unsigned>::max())
			{
				std::cout << m_ShaderPath[0].second << ".vert : " << name << " doesn't exist!" << std::endl;
			}
			return uniformindex;
		}		
	}

	void Shader::SetUniformBlockBinding(const std::string& variable, unsigned binding_num) const
	{
		auto find = m_UniformBlockID.find(variable);
		if (find != m_UniformBlockID.end())
		{
			glUniformBlockBinding(m_ShaderID, find->second, binding_num);
		}
		else
		{
			std::cout << m_ShaderPath[0].second << ".vert or .frag : " << variable << " doesn't exist! : SetUniformBlockBinding" << std::endl;
		}
	}

	int Shader::GetUnformSizeByLocation(unsigned index)
	{
		GLint blocksize;
		glGetActiveUniformBlockiv(m_ShaderID, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blocksize);
		return blocksize;
	}

	void Shader::SetIsDebugShader(bool IsDebug)
	{
		mIsDebugShader = IsDebug;
	}

	bool Shader::GetIsDebugShader() const
	{
		return mIsDebugShader;
	}
}
