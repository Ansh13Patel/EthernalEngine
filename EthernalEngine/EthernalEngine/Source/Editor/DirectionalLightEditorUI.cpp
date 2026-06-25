#include"Editor/DirectionalLightEditorUI.h"

namespace EthernalEngine
{
	void DirectionalLightEditorUI::ShowDirectionalLightParameters(DirectionalLight* dirLight)
	{
		ImGui::Text("Ambient Strength");
		ImGui::SameLine();
		ImGui::SliderFloat("##AmbientStrength", &dirLight->ambientStrength, 0.0f, 1.0f);

		ImGui::Text("Specular Strength");
		ImGui::SameLine();
		ImGui::SliderFloat("##SpecularStrength", &dirLight->specularStrength, 0.0f, 1.0f);

		ImGui::Text("Intensity");
		ImGui::SameLine();
		ImGui::SliderFloat("##Intensity", &dirLight->intensity, 0.1f, 5.0f);

		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##Color", &dirLight->lightColor[0]);
	}
}