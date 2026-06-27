#include "Editor/SpotLightEditorUI.h"

namespace EthernalEngine
{
	void SpotLightEditorUI::ShowSpotLightParameters(SpotLight* spotLight)
	{
		ImGui::Text("Spot Angle");
		ImGui::SameLine();
		ImGui::SliderFloat("##SpotAngle", &spotLight->spotAngle, 0.0f, 179.0f);

		ImGui::Text("Range");
		ImGui::SameLine();
		ImGui::SliderFloat("##Range", &spotLight->range, 0.0f, 100.0f);

		ImGui::Text("Specular Strength");
		ImGui::SameLine();
		ImGui::SliderFloat("##SpecularStrength", &spotLight->specularStrength, 0.0f, 1.0f);

		ImGui::Text("Intensity");
		ImGui::SameLine();
		ImGui::SliderFloat("##Intensity", &spotLight->intensity, 0.1f, 5.0f);

		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##Color", &spotLight->lightColor[0]);
	}
}