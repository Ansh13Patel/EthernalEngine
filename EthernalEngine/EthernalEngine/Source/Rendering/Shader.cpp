#include "Rendering/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

namespace EthernalEngine
{
	Shader::Shader()
	{
	}

	Shader::~Shader()
	{
		if (shaderProgram != 0)
		{
			glDeleteProgram(shaderProgram);
		}
	}

	std::string Shader::ReadFile(const char* path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cout << "Failed to open shader file: " << path << std::endl;

			return "";
		}

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();

		return stream.str();
	}

	bool Shader::LoadFromFile(
		const char* vertexPath,
		const char* fragmentPath
	)
	{
		std::string vertexCode = ReadFile(vertexPath);

		std::string fragmentCode = ReadFile(fragmentPath);

		if (vertexCode.empty() || fragmentCode.empty())
		{
			return false;
		}

		const char* vertexShaderSource = vertexCode.c_str();

		const char* fragmentShaderSource = fragmentCode.c_str();

		// Vertex Shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

		glCompileShader(vertexShader);

		int success;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

			std::cout << "Vertex Shader Compilation Failed:\n" << infoLog << std::endl;

			return false;
		}

		// Fragment Shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

			std::cout << "Fragment Shader Compilation Failed:\n" << infoLog << std::endl;

			glDeleteShader(vertexShader);

			return false;
		}

		// Shader Program
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);

		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

			std::cout << "Shader Program Linking Failed:\n" << infoLog << std::endl;

			glDeleteShader(vertexShader);

			glDeleteShader(fragmentShader);

			return false;
		}

		// Cleanup
		glDeleteShader(vertexShader);

		glDeleteShader(fragmentShader);

		return true;
	}

	void Shader::Use()
	{
		glUseProgram(shaderProgram);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, 
			glm::value_ptr(matrix));
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}

	void Shader::SetFloat3(const std::string& name, float value[3]) 
	{
		glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, value);
	}

	void Shader::SetFloat4(const std::string& name, float value[4])
	{
		glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, value);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()),value);
	}
}