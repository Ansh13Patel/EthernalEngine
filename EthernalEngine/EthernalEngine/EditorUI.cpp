#include "EditorUI.h"

#include <windows.h>
#include <commdlg.h>

namespace EthernalEngine
{
	bool EditorUI::Initialize(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		io.Fonts->AddFontDefault();
		ImGui::GetIO().FontGlobalScale = 1.2f;
		ImGui::GetStyle().ScaleAllSizes(2.0f);
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		return true;
	}
	void EditorUI::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void EditorUI::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void EditorUI::RenderUI(Scene* scene)
	{
		MainMenuBar(scene);
		Hierarchy(scene);
		Inspector(scene->GetSelectedGameObject());
	}
	void EditorUI::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorUI::MainMenuBar(Scene* scene)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					scene->AddGameObject(scene->CreateCubeGameObject("NewCube"));
				}
				ImGui::EndMenu();
			}
		    ImGui::EndMainMenuBar();
		}
	}

	void EditorUI::Hierarchy(Scene* scene)
	{
		std::vector<GameObject*> objects = scene->GetGameObjects();

		ImGui::Begin("Hierarchy");
		for (int i = 0; i < scene->GetGameObjectCount(); i++)
		{
			ImGui::PushID(i);
			GameObject* obj = objects[i];
			bool isSelected = (scene->GetSelectedGameObject() == obj);

			if (ImGui::Selectable(obj->name.c_str(), isSelected))
			{
				scene->SetSelectedGameObject(obj);
			}
			ImGui::PopID();
		}
		ImGui::End();
	}

	void EditorUI::Inspector(GameObject* gameObject)
	{
		ImGui::Begin("Inspector");
		if (gameObject)
		{
			ImGui::Text(gameObject->name.c_str());
			ImGui::Separator();

			Transform& transform = gameObject->transform;
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##Position", &transform.position.x, 0.1f, -1000.0f, 1000.0f, "%.3f");
			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##Rotation", &transform.rotation.x, 0.1f, -360.f, 360.f, "%.3f");
			ImGui::Text("Scale");
			ImGui::SameLine();
			ImGui::DragFloat3("##Scale", &transform.scale.x, 0.1f, 0.0f, 1000.0f, "%.3f");

			ImGui::Separator();

			ImGui::Text("Color");
			ImGui::SameLine();
			ImGui::ColorEdit3("##Color", &gameObject->color[0]);

			ImGui::Separator();

			ImGui::Text("Texture");
			ImGui::SameLine();
			if (ImGui::Button("Select Texture"))
			{
				char filename[MAX_PATH] = "";
				
				OPENFILENAMEA ofn = {};
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = nullptr;
				ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg\0";
				ofn.lpstrFile = filename;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (GetOpenFileNameA(&ofn))
				{
					gameObject->GetTexture()->LoadTexture(filename);
				}
			}
			ImGui::SameLine();
			ImGui::Image((ImTextureID)gameObject->GetTexture()->GetTextureID(), ImVec2(25, 25));
		}
		ImGui::End();
	}
}