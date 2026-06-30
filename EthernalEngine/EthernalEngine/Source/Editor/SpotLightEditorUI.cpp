#include "Editor/SpotLightEditorUI.h"

namespace EthernalEngine
{
	void SpotLightEditorUI::ShowSpotLightParameters(SpotLight* spotLight)
	{
		ImGui::Text("Spot Angle");
		ImGui::SameLine();
		ImGui::DragFloat("##SpotAngle", &spotLight->spotAngle, 0.0f, 100.0f);

		ImGui::Text("Range");
		ImGui::SameLine();
		ImGui::DragFloat("##Range", &spotLight->range, 0.0f, 100.0f);

		ImGui::Text("Specular Strength");
		ImGui::SameLine();
		ImGui::DragFloat("##SpecularStrength", &spotLight->specularStrength, 0.0f, 1.0f);

		ImGui::Text("Intensity");
		ImGui::SameLine();
		ImGui::DragFloat("##Intensity", &spotLight->intensity, 0.1f, 5.0f);

		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##Color", &spotLight->lightColor[0]);
	}
}