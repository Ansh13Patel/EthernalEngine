#pragma once
#include "Scene/Transform.h"

namespace EthernalEngine
{
	class DirectionalLight
	{
	public:
		DirectionalLight();
		~DirectionalLight() = default;

		glm::vec3 direction;
		float lightColor[3]{ 1.0f,1.0f,1.0f };
		float ambientStrength;
		float specularStrength;
		float intensity;
		bool useLight;
	};
}
