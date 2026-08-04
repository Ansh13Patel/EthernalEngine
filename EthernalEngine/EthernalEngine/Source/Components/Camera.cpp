#include<Components/Camera.h>

namespace EthernalEngine
{
	Camera::Camera(GameObject* gameobject,Window* window, Viewport& viewport) : m_viewport(viewport)
	{
		parentObj = gameobject;
		fov = 60;
		flag = ClearFlag::SKYBOX;
		projection = Projection::Perspective;
		cameraBuffer = new FrameBuffer();
		cameraBuffer->Create(window->GetWidth(), window->GetHeight());
	}

	glm::vec3 Camera::GetPosition() const
	{
		return parentObj->transform->position;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		if (parentObj == nullptr || parentObj->transform == nullptr) return glm::mat4(1.0f);

		Transform* transform = parentObj->transform;

		glm::vec3 pos = transform->position;
		glm::vec3 front = transform->GetForward();
		glm::vec3 up = transform->GetUp();

		return glm::lookAt(pos, pos + front, up);
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		float width = m_viewport.viewportSize.x;
		float height = m_viewport.viewportSize.y;
		if (width <= 0 || height <= 0) return glm::mat4(1.0f);

		switch (projection)
		{		
		case EthernalEngine::Perspective:
			return glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
		case EthernalEngine::Orthographic:
			return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);
		default:
			break;
		}
	}

	json Camera::SerializeComponent() const
	{
		json cameraJson = json();
		return cameraJson;
	}

	void Camera::DeserializeComponent(const json& componentJson)
	{
		
	}
}