#pragma once

#include <glad/glad.h>

#include <string>
#include <glm/glm.hpp>

namespace EthernalEngine
{
	class Shader
	{
	public:

		Shader();
		~Shader();

		bool LoadFromFile(
			const char* vertexPath,
			const char* fragmentPath
		);

		void Use();

		void SetMat4(
			const std::string& name,
			const glm::mat4& matrix
		);

		void SetFloat(const std::string& name, float value);

		void SetFloat3(const std::string& name, float value[3]);
		
		void SetFloat4(const std::string& name, float value[4]);

		void SetInt(const std::string& name, int value);

	private:

		std::string ReadFile(
			const char* path
		);

		unsigned int shaderProgram = 0;
	};
}