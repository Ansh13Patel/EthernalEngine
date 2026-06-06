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
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

	private:
		float screenWidth;
		float screenHeight;

		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		float yaw;
		float pitch;
		float lastX;
		float lastY;
		bool firstMouse;
		float fov;

		float cameraSpeed = 2.5f;	
	};

}