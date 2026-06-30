#include "Scene/Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
namespace EthernalEngine
{
	glm::mat4 Transform::GetModelMatrix()
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotationMatrix = glm::toMat4(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	glm::vec3 Transform::GetForward()
	{
		glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
		return glm::normalize(forward);
	}

	glm::vec3 Transform::GetRight()
	{
		glm::vec3 forward = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
		return glm::normalize(forward);
	}
}