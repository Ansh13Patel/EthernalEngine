#include "EditorUI.h"
#include "Helper.h"

#include <windows.h>
#include <commdlg.h>

#include <iostream>

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
		UpdateGizmoOperation();
		DrawGizmo(scene->GetSelectedGameObject(), &scene->GetCamera());
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
				if (ImGui::MenuItem("Import"))
				{
					std::string filepath = Helper::OpenFileDialog("Model Files\0*.obj;*.fbx;*.gltf;*.glb\0All Files\0*.*\0");
					if (!filepath.empty())
					{
						scene->AddGameObject(scene->CreateGameObjectWithCustomModel("NewObject", filepath));
					}
					else
					{
						std::cout << "Model filepath is invalid" << std::endl;
					}
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
				std::string filepath = Helper::OpenFileDialog("Image Files\0*.png;*.jpg;*.jpeg\0");

				if (!filepath.empty())
				{
					gameObject->GetTexture()->LoadTexture(filepath.c_str());
				}
				else
				{
					std::cout << "Texture filepath is invalid" << std::endl;
				}
			}
			ImGui::SameLine();
			ImGui::Image((ImTextureID)gameObject->GetTexture()->GetTextureID(), ImVec2(25, 25));
		}
		ImGui::End();
	}

	void EditorUI::DrawGizmo(GameObject* selectedGameObject, Camera* camera)
	{
		if (!selectedGameObject) return;

		ImGuizmo::BeginFrame();
		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
		
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 model = selectedGameObject->transform.GetModelMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), currentOperation, 
			ImGuizmo::LOCAL, glm::value_ptr(model));

		if (ImGuizmo::IsOver())
		{
			std::cout << "Over Gizmo" << std::endl;
		}

		if (ImGuizmo::IsUsing())
		{
			std::cout << "Gizmo using " << std::endl;
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), &position.x, &rotation.x, &scale.x);

			selectedGameObject->transform.position = position;
			selectedGameObject->transform.rotation = rotation;
			selectedGameObject->transform.scale = scale;
		}
	}

	void EditorUI::UpdateGizmoOperation()
	{
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) return;

		if (ImGui::IsKeyPressed(ImGuiKey_W)) currentOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_E)) currentOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R)) currentOperation = ImGuizmo::SCALE;
	}
}