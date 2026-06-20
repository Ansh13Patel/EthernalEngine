#include "Scene/DirectionalLight.h"

namespace EthernalEngine
{
	DirectionalLight::DirectionalLight()
	{
		ambientStrength = 0.2f;
		specularStrength = 0.5f;
		intensity = 1.0f;
		useLight = true;
		direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	}
}