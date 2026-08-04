#pragma once
#include "Components/Light.h"

namespace EthernalEngine
{
	class SpotLight : public Light
	{
	public:
	public:
		SpotLight(GameObject* gameobject);
		~SpotLight() = default;
		void Draw() override;
		json SerializeComponent() const override;
		void DeserializeComponent(const json& componentJson) override;

	public:
		float spotAngle;
		float range;
	};
}