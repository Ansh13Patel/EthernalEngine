#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

namespace EthernalEngine
{
	class Transform
	{
	public:
		Transform() = default;
		~Transform() = default;
		glm::vec3 GetForward();
		glm::vec3 GetRight();
		glm::mat4 GetModelMatrix();

	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation = glm::quat(glm::vec3(0.0f));
		glm::vec3 scale = glm::vec3(1.0f);
	};
}