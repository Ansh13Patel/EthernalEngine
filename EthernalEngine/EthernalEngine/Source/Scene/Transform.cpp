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
		float pitch = rotation.x;
		float yaw = rotation.y;

		glm::vec3 forward;

		forward.x = cos(yaw) * cos(pitch);
		forward.y = sin(pitch);
		forward.z = sin(yaw) * cos(pitch);

		return glm::normalize(forward);
	}
}