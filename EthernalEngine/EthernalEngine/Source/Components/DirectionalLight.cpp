#include "Components/DirectionalLight.h"
#include "Helper/DebugDraw.h"

namespace EthernalEngine
{
	DirectionalLight::DirectionalLight(GameObject* gameobject)
	{
		this->gameobject = gameobject;
		ambientStrength = 0.2f;
		specularStrength = 0.5f;
		intensity = 1.0f;
		enable = true;
	}

	void DirectionalLight::Draw()
	{
		if (gameobject->GetIsSelected())
		{
			glm::vec3 startPos = gameobject->transform->position;
			glm::vec3 endPos = gameobject->transform->position + (gameobject->transform->GetForward() * 0.25f);

			DebugDraw::DrawLine(startPos, endPos, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	json DirectionalLight::SerializeComponent() const
	{
		json componentJson = Light::SerializeComponent();
		componentJson["type"] = "DirectionalLight";
		componentJson["ambientStrength"] = ambientStrength;
		componentJson["specularStrength"] = specularStrength;
		componentJson["intensity"] = intensity;
		componentJson["enable"] = enable;
		return componentJson;
	}

	void DirectionalLight::DeserializeComponent(const json& componentJson)
	{
		Light::DeserializeComponent(componentJson);
		if (componentJson.contains("ambientStrength"))
			ambientStrength = componentJson["ambientStrength"].get<float>();
		if (componentJson.contains("specularStrength"))
			specularStrength = componentJson["specularStrength"].get<float>();
		if (componentJson.contains("intensity"))
			intensity = componentJson["intensity"].get<float>();
		if (componentJson.contains("enable"))
			enable = componentJson["enable"].get<bool>();
	}
}