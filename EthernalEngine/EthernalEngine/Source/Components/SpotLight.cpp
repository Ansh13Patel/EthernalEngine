#include "Components/SpotLight.h"
#include "Helper/DebugDraw.h"

namespace EthernalEngine
{
	SpotLight::SpotLight(GameObject* gameobject)
	{
		spotAngle = 10;
		range = 2.0f;
		specularStrength = 0.5f;
		intensity = 1.0f;
		this->gameobject = gameobject;
	}

	void SpotLight::Draw()
	{
		if (gameobject->GetIsSelected())
		{
			glm::vec3 tipPos = gameobject->transform.position;
			glm::vec3 forwardDir = gameobject->transform.GetForward();

			DebugDraw::DrawCone(tipPos, forwardDir, spotAngle, range, glm::vec4(lightColor[0], lightColor[1], lightColor[2], lightColor[3]));
		}
	}
}