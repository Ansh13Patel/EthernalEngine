#include "Components/DirectionalLight.h"
#include "Helper/DebugDraw.h"

namespace EthernalEngine
{
	DirectionalLight::DirectionalLight(GameObject* gameobject)
	{
		this->gameobject = gameobject;
		ambientStrength = 0.2f;
		specularStrength = 0.5f;
		intensity = 1.0f;
		enable = true;
	}

	void DirectionalLight::Draw()
	{
		if (gameobject->GetIsSelected())
		{
			glm::vec3 startPos = gameobject->transform.position;
			glm::vec3 endPos = gameobject->transform.position + (gameobject->transform.GetForward() * 0.25f);

			DebugDraw::DrawLine(startPos, endPos, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}
}