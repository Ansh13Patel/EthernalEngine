#pragma once

#include "Components/Component.h"
#include "Editor/EditorUI.h"
#include "Core/ICamera.h"

namespace EthernalEngine
{
	enum Projection
	{
		Perspective,
		Orthographic
	};

	enum ClearFlag
	{
		SKYBOX,
		SOLIDCOLOR
	};

	class Camera : public Component, public ICamera
	{
	public:
		Camera(GameObject* gameobject, Window* window, Viewport& viewport);
		~Camera() = default;
		glm::mat4 GetViewMatrix() const override;
		glm::mat4 GetProjectionMatrix() const override;
		glm::vec3 GetPosition() const override;
		FrameBuffer* GetCameraBuffer() { return cameraBuffer; }
		json SerializeComponent() const override;
		void DeserializeComponent(const json& componentJson) override;

	public:
		ClearFlag flag;
		Projection projection;
		float fov;

	private:
		Viewport& m_viewport;
		FrameBuffer* cameraBuffer = nullptr;
	};
}