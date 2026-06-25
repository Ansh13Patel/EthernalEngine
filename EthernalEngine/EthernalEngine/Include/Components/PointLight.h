#pragma once

#include "Components/Light.h"
#include "Scene/GameObject.h"

namespace EthernalEngine
{
	class PointLight : public Light
	{
	public:
		PointLight(GameObject* gameobject);
		~PointLight() = default;

	public:
		float radius;
		GameObject* gameobject;
	};
}