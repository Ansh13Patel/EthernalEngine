#pragma once
#include "Components/Light.h"
#include "Scene/GameObject.h"

namespace EthernalEngine
{
	class SpotLight : public Light
	{
	public:
	public:
		SpotLight(GameObject* gameobject);
		~SpotLight() = default;
		void Draw() override;

	public:
		float spotAngle;
		float range;
		GameObject* gameobject;
	};
}