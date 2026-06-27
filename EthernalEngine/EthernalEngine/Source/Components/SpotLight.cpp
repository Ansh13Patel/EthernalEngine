#include "Components/SpotLight.h"

namespace EthernalEngine
{
	SpotLight::SpotLight(GameObject* gameobject)
	{
		spotAngle = 10;
		range = 10;
		specularStrength = 0.5f;
		intensity = 1.0f;
		this->gameobject = gameobject;
	}
}