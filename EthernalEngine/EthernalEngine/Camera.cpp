#include "Camera.h"

#include<glm/gtc/matrix_transform.hpp>

namespace EthernalEngine
{
	Camera::Camera(float screenWidth, float screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		yaw = -90.0f;
		pitch = 0.0f;
		lastX = 400.0f;
		lastY = 300.0f;
		firstMouse = true;
		fov = 45.0f;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	}

	Camera::~Camera()
	{
	}
}