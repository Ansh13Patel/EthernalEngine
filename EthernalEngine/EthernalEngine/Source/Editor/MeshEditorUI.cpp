#include "Editor/MeshEditorUI.h"
#include "Helper/FileHelper.h"

#include <iostream>

namespace EthernalEngine
{
	void MeshEditorUI::ShowMeshParameters(Mesh* mesh)
	{
		ImGui::Text("Texture");
		ImGui::SameLine();
		if (ImGui::Button("Select Texture"))
		{
			std::string filepath = FileHelper::OpenFileDialog("Image Files\0*.png;*.jpg;*.jpeg\0");

			if (!filepath.empty())
			{
				mesh->GetTexture()->LoadTextureFromPath(filepath.c_str());
			}
			else
			{
				std::cout << "Texture filepath is invalid" << std::endl;
			}
		}
		ImGui::SameLine();
		ImGui::Image((ImTextureID)mesh->GetTexture()->GetTextureID(), ImVec2(25, 25));
	}
}