#include "DirectionalLight.h"

namespace EthernalEngine
{
	DirectionalLight::DirectionalLight()
	{
		ambientStrength = 0.2f;
		specularStrength = 0.5f;
		intensity = 1.0f;
		useLight = true;
	}
}