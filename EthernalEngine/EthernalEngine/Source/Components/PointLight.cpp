#include "Components/PointLight.h"
#include "Helper/DebugDraw.h"

namespace EthernalEngine
{
	PointLight::PointLight(GameObject* gameobject)
	{
		this->gameobject = gameobject;
		specularStrength = 0.5f;
		intensity = 1.0f;
		radius = 10.0f;
	}

	void PointLight::Draw()
	{
		glm::vec3 centerPos = gameobject->transform.position;

		DebugDraw::DrawSphere(centerPos, radius, glm::vec4(lightColor[0], lightColor[1], lightColor[2], lightColor[3]));
	}
}