#include "Components/SpotLight.h"
#include "Helper/DebugDraw.h"

namespace EthernalEngine
{
	SpotLight::SpotLight(GameObject* gameobject)
	{
		spotAngle = 10;
		range = 2.0f;
		specularStrength = 0.5f;
		intensity = 1.0f;
		this->parentObj = gameobject;
	}

	void SpotLight::Draw()
	{
		if (parentObj->GetIsSelected())
		{
			glm::vec3 tipPos = parentObj->transform->position;
			glm::vec3 forwardDir = parentObj->transform->GetForward();

			DebugDraw::DrawCone(tipPos, forwardDir, spotAngle, range, glm::vec4(lightColor[0], lightColor[1], lightColor[2], lightColor[3]));
		}
	}

	json SpotLight::SerializeComponent() const
	{
		json componentJson = Light::SerializeComponent();
		componentJson["type"] = "SpotLight";
		componentJson["spotAngle"] = spotAngle;
		componentJson["range"] = range;
		componentJson["specularStrength"] = specularStrength;
		componentJson["intensity"] = intensity;
		return componentJson;
	}

	void SpotLight::DeserializeComponent(const json& componentJson)
	{
		Light::DeserializeComponent(componentJson);
		if (componentJson.contains("spotAngle"))
			spotAngle = componentJson["spotAngle"].get<float>();
		if (componentJson.contains("range"))
			range = componentJson["range"].get<float>();
		if (componentJson.contains("specularStrength"))
			specularStrength = componentJson["specularStrength"].get<float>();
		if (componentJson.contains("intensity"))
			intensity = componentJson["intensity"].get<float>();
	}
}