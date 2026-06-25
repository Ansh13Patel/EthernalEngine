#include "Components/DirectionalLight.h"

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
}