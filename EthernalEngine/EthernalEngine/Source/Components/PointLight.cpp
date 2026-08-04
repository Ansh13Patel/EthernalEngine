#include "Components/PointLight.h"
#include "Helper/DebugDraw.h"

namespace EthernalEngine
{
	PointLight::PointLight(GameObject* gameobject)
	{
		this->parentObj = gameobject;
		specularStrength = 0.5f;
		intensity = 1.0f;
		radius = 1.0f;
	}

	void PointLight::Draw()
	{
		if (parentObj->GetIsSelected())
		{
			glm::vec3 centerPos = parentObj->transform->position;
			DebugDraw::DrawSphere(centerPos, radius, glm::vec4(lightColor[0], lightColor[1], lightColor[2], lightColor[3]));
		}
	}

	json PointLight::SerializeComponent() const
	{
		json componentJson = Light::SerializeComponent();
		componentJson["type"] = "PointLight";
		componentJson["specularStrength"] = specularStrength;
		componentJson["intensity"] = intensity;
		componentJson["radius"] = radius;
		return componentJson;
	}

	void PointLight::DeserializeComponent(const json& componentJson)
	{
		Light::DeserializeComponent(componentJson);
		if (componentJson.contains("specularStrength"))
			specularStrength = componentJson["specularStrength"].get<float>();
		if (componentJson.contains("intensity"))
			intensity = componentJson["intensity"].get<float>();
		if (componentJson.contains("radius"))
			radius = componentJson["radius"].get<float>();
	}
}