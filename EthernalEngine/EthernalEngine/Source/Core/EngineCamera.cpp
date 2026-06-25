#include "Core/EngineCamera.h"

#include<glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace EthernalEngine
{
	EngineCamera::EngineCamera(Window* window)
	{
		this->m_window = window;
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		yaw = -90.0f;
		pitch = 0.0f;
		fov = 45.0f;
	}

	void EngineCamera::UpdateCameraRotation(float xOffset, float yOffset)
	{
		yaw += xOffset;
		pitch += yOffset;
		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		glm::vec3 direction;

		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		
		cameraFront = glm::normalize(direction);
	}

	void EngineCamera::UpdateCameraFov(float yScrollOffset)
	{
		cameraPos += cameraFront * cameraScrollSpeed * yScrollOffset;
	}

	void EngineCamera::MoveCamera(bool forward, bool backward, bool right, bool left,bool up, bool down, float deltatime)
	{
		if (forward)
			cameraPos += cameraFront * cameraSpeed * deltatime;
		if (backward)
			cameraPos -= cameraFront * cameraSpeed * deltatime;
		if (right)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltatime;
		if (left)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltatime;
	}

	void EngineCamera::MoveCamera(float xOffset, float yOffset)
	{
		glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));

		cameraPos -= right * xOffset * cameraPanSpeed;
		cameraPos -= cameraUp * yOffset * cameraPanSpeed;
	}

	glm::mat4 EngineCamera::GetViewMatrix() const
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::mat4 EngineCamera::GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(fov), (float)m_window->GetWidth() / (float)m_window->GetHeight(), 0.1f, 100.0f);
	}
}