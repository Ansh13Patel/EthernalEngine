#include "Components/PointLight.h"

namespace EthernalEngine
{
	PointLight::PointLight(GameObject* gameobject)
	{
		this->gameobject = gameobject;
		specularStrength = 0.5f;
		intensity = 1.0f;
		radius = 10.0f;
	}
}