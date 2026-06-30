#include "Editor/PointLightEditorUI.h"

namespace EthernalEngine
{
	void PointLightEditorUI::ShowPointLightParameters(PointLight* pointLight)
	{
		ImGui::Text("Radius");
		ImGui::SameLine();
		ImGui::DragFloat("##Radius", &pointLight->radius, 0.0f, 100.0f);

		ImGui::Text("Specular Strength");
		ImGui::SameLine();
		ImGui::DragFloat("##SpecularStrength", &pointLight->specularStrength, 0.0f, 1.0f);

		ImGui::Text("Intensity");
		ImGui::SameLine();
		ImGui::DragFloat("##Intensity", &pointLight->intensity, 0.1f, 5.0f);

		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##Color", &pointLight->lightColor[0]);
	}
}