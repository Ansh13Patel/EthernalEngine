#pragma once

#include <glm/glm.hpp>
#include "Core/Window.h"

namespace EthernalEngine
{
	class EngineCamera
	{
	public:
		EngineCamera(Window* window);
		~EngineCamera() = default;
		void Update(float deltaTime);
		void UpdateCameraRotation(float xOffset, float yOffset);
		void UpdateCameraFov(float yScrollOffset);
		void MoveCamera(bool forward, bool backward, bool right, bool left,bool up, bool down, float deltatime);
		void MoveCamera(float xOffset, float yOffset);
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

	private:
		Window* m_window = nullptr;
		float cameraSpeed = 3.0f;	
		float cameraPanSpeed = 0.05f;
		float cameraScrollSpeed = 0.25f;

	public:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		float yaw;
		float pitch;
		float fov;
	};

}