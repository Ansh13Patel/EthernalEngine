#pragma once
#include "Components/Light.h"

namespace EthernalEngine
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(GameObject* gameobject);
		~DirectionalLight() override = default;
		void Draw() override;
		json SerializeComponent() const override;
		void DeserializeComponent(const json& componentJson) override;

	public:
		float ambientStrength;
	};
}
