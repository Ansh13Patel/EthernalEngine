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
		glm::mat4 GetLightSpaceMatrix(const glm::vec3& sceneCenter) const;
		json SerializeComponent() const override;
		void DeserializeComponent(const json& componentJson) override;

	public:
		float ambientStrength;
	};
}
