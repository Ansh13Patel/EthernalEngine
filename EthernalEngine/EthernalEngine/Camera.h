#pragma once

#include <glm/glm.hpp>

namespace EthernalEngine
{
	class Camera
	{
	public:
		Camera(float screenWidth, float screenHeight);
		~Camera();
		void Update(float deltaTime);
		void UpdateCameraRotation(float xOffset, float yOffset);
		void UpdateCameraFov(float yScrollOffset);
		void MoveCamera(bool forward, bool backward, bool right, bool left, float deltatime);
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

	private:
		float screenWidth;
		float screenHeight;

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