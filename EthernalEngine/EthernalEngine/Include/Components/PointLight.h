#pragma once

#include "Components/Light.h"

namespace EthernalEngine
{
	class PointLight : public Light
	{
	public:
		PointLight(GameObject* gameobject);
		~PointLight() = default;
		void Draw() override;
		json SerializeComponent() const override;
		void DeserializeComponent(const json& componentJson) override;

	public:
		float radius;
	};
}