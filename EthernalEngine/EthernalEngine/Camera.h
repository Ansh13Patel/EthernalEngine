#pragma once

#include <glm/glm.hpp>
#include "Window.h"

namespace EthernalEngine
{
	class Camera
	{
	public:
		Camera(Window* window);
		~Camera();
		void Update(float deltaTime);
		void UpdateCameraRotation(float xOffset, float yOffset);
		void UpdateCameraFov(float yScrollOffset);
		void MoveCamera(bool forward, bool backward, bool right, bool left, float deltatime);
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

	private:
		Window* m_window = nullptr;
		float cameraSpeed = 3.0f;	

	public:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		float yaw;
		float pitch;
		float fov;
	};

}