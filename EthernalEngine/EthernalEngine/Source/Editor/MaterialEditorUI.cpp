#include "Helper/FileHelper.h"
#include "Editor/MaterialEditorUI.h"

#include <iostream>

namespace EthernalEngine
{
	void MaterialEditorUI::ShowMaterialParameters(Material* material)
	{
		ImGui::Separator();

		ImGui::Text("Base Texture");
		ImGui::SameLine();
		if (ImGui::Button("Select Texture"))
		{
			std::string filepath = FileHelper::OpenFileDialog("Image Files\0*.png;*.jpg;*.jpeg\0");

			if (!filepath.empty())
			{
				material->GetBaseTexture()->LoadTextureFromPath(filepath.c_str());
			}
			else
			{
				std::cout << "Texture filepath is invalid" << std::endl;
			}
		}
		ImGui::SameLine();
		ImGui::Image((ImTextureID)material->GetBaseTexture()->GetTextureID(), ImVec2(25, 25));

		ImGui::Text("Shininess");
		ImGui::SameLine();
		ImGui::SliderFloat("##Shininess", &material->shininess, 1.0f, 128.0f);

		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit3("##Color", material->GetColor());
	}
}