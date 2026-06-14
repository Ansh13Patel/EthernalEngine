#include "Camera.h"

#include<glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace EthernalEngine
{
	Camera::Camera(Window* window)
	{
		this->m_window = window;
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		yaw = -90.0f;
		pitch = 0.0f;
		fov = 45.0f;
	}

	void Camera::UpdateCameraRotation(float xOffset, float yOffset)
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

	void Camera::UpdateCameraFov(float yScrollOffset)
	{
		fov -= yScrollOffset;
		fov = glm::clamp(fov, 1.0f, 45.0f);
	}

	void Camera::MoveCamera(bool forward, bool backward, bool right, bool left, float deltatime)
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

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(fov), (float)m_window->GetWidth() / (float)m_window->GetHeight(), 0.1f, 100.0f);
	}

	Camera::~Camera()
	{
	}
}