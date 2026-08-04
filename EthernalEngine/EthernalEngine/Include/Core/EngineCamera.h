#pragma once

#include <glm/glm.hpp>
#include "Core/Window.h"
#include "Core/ICamera.h"
#include "Scene/Transform.h"

namespace EthernalEngine
{
	struct Viewport;
	class EngineCamera : public ICamera
	{
	public:
		EngineCamera(Viewport& viewport);
		~EngineCamera() = default;
		void Update(float deltaTime);
		void UpdateCameraRotation(float xOffset, float yOffset);
		void UpdateCameraFov(float yScrollOffset);
		void MoveCamera(bool forward, bool backward, bool right, bool left,bool up, bool down, float deltatime);
		void MoveCamera(float xOffset, float yOffset);
        glm::mat4 GetViewMatrix() const override;
		glm::mat4 GetProjectionMatrix() const override;
		glm::vec3 GetPosition() const override;

	private:
		Viewport& m_viewport;
		float cameraSpeed = 3.0f;	
		float cameraPanSpeed = 0.05f;
		float cameraScrollSpeed = 0.25f;

    public:
        Transform transform;
		float yaw;
		float pitch;
		float fov;
		bool isPerspective = true;
	};

}