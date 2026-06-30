#pragma once
#include "Components/Light.h"
#include "Scene/GameObject.h"

namespace EthernalEngine
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(GameObject* gameobject);
		~DirectionalLight() = default;
		void Draw() override;

	public:
		GameObject* gameobject;
		float ambientStrength;

	};
}
