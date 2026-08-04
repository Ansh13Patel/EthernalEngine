
#define GLM_ENABLE_EXPERIMENTAL
#include "Core/EngineCamera.h"
#include <Editor/EditorUI.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

namespace EthernalEngine
{
    EthernalEngine::EngineCamera::EngineCamera(Viewport& viewport)
		: m_viewport(viewport)
	{
        transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
        transform.rotation = glm::quat(glm::vec3(0.0f));

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
		direction = glm::normalize(direction);

		// Build a view matrix looking in the new direction, then extract rotation
		glm::mat4 view = glm::lookAt(transform.position, transform.position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::inverse(view);
		transform.rotation = glm::quat_cast(model);
	}

	glm::vec3 EngineCamera::GetPosition() const
	{
		return transform.position;
	}


    void EngineCamera::UpdateCameraFov(float yScrollOffset)
	{
		// Use scroll to move camera along its forward direction (zoom)
		transform.position += transform.GetForward() * cameraScrollSpeed * yScrollOffset;
	}

    void EngineCamera::MoveCamera(bool forward, bool backward, bool right, bool left,bool up, bool down, float deltatime)
	{
		if (forward)
			transform.position += transform.GetForward() * cameraSpeed * deltatime;
		if (backward)
			transform.position -= transform.GetForward() * cameraSpeed * deltatime;
		if (right)
			transform.position += transform.GetRight() * cameraSpeed * deltatime;
		if (left)
			transform.position -= transform.GetRight() * cameraSpeed * deltatime;
		if (up)
			transform.position += transform.GetUp() * cameraSpeed * deltatime;
		if (down)
			transform.position -= transform.GetUp() * cameraSpeed * deltatime;
	}

    void EngineCamera::MoveCamera(float xOffset, float yOffset)
	{
		glm::vec3 right = transform.GetRight();

		transform.position -= right * xOffset * cameraPanSpeed;
		transform.position -= transform.GetUp() * yOffset * cameraPanSpeed;
	}

    glm::mat4 EngineCamera::GetViewMatrix() const
	{
        return glm::lookAt(transform.position, transform.position + transform.GetForward(), transform.GetUp());
	}

	glm::mat4 EngineCamera::GetProjectionMatrix() const
	{
		float width = m_viewport.viewportSize.x;
		float height = m_viewport.viewportSize.y;
		if (width <= 0 || height <= 0) return glm::mat4(1.0f);

		if (isPerspective)
		{
			return glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
		}
		else
		{
			return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);
		}
	}
}