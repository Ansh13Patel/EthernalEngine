#include "Editor/EditorUI.h"
#include "Helper/Helper.h"

#include <windows.h>
#include <commdlg.h>

#include <iostream>
#include <filesystem>

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
		LightingSetting(scene->GetDirectionalLight());
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
						std::string gameobjectname = Helper::GetFileName(filepath);
						if (gameobjectname == "") gameobjectname = "NewObject";
						scene->AddGameObject(scene->CreateGameObjectWithCustomModel(gameobjectname, filepath));
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
			ShowGameObjectInHierachy(scene->GetGameObjects()[i], scene);
		}
		ImGui::End();
	}

    void EditorUI::Inspector(GameObject* gameObject)
    {
        ImGui::Begin("Inspector");
        if (gameObject)
        {
            static char gameobjectname[128];
            // Copy the name from the GameObject to the buffer only if the object changes
            static const GameObject* lastGameObject = nullptr;
            if (lastGameObject != gameObject)
            {
                strncpy_s(gameobjectname, gameObject->name.c_str(), sizeof(gameobjectname) - 1);
                gameobjectname[sizeof(gameobjectname) - 1] = '\0';
                lastGameObject = gameObject;
            }
            if (ImGui::InputText("##objectname", gameobjectname, IM_ARRAYSIZE(gameobjectname)))
            {
				gameObject->name = gameobjectname;
            }
            ImGui::Separator();

            Transform& transform = gameObject->transform;
			glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(gameObject->transform.rotation));
            ImGui::Text("Position");
            ImGui::SameLine();
            ImGui::DragFloat3("##Position", &transform.position.x, 0.1f, -1000.0f, 1000.0f, "%.3f");
            ImGui::Text("Rotation");
            ImGui::SameLine();
			if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotationEuler), 0.1f))
			{
				transform.rotation = glm::quat(glm::radians(rotationEuler));
			}
            ImGui::Text("Scale");
            ImGui::SameLine();
            ImGui::DragFloat3("##Scale", &transform.scale.x, 0.1f, 0.0f, 1000.0f, "%.3f");

            ImGui::Separator();

            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::ColorEdit3("##Color", &gameObject->color[0]);

			if (gameObject->GetMesh() != nullptr)
			{
				ImGui::Separator();

				ImGui::Text("Texture");
				ImGui::SameLine();
				if (ImGui::Button("Select Texture"))
				{
					std::string filepath = Helper::OpenFileDialog("Image Files\0*.png;*.jpg;*.jpeg\0");

					if (!filepath.empty())
					{
						gameObject->GetMesh()->GetTexture()->LoadTextureFromPath(filepath.c_str());
					}
					else
					{
						std::cout << "Texture filepath is invalid" << std::endl;
					}
				}
				ImGui::SameLine();
				ImGui::Image((ImTextureID)gameObject->GetMesh()->GetTexture()->GetTextureID(), ImVec2(25, 25));
			}
        }
        ImGui::End();
    }

	void EditorUI::DrawGizmo(GameObject* selectedGameObject, Camera* camera)
	{
		if (!selectedGameObject) return;

		ImGuizmo::BeginFrame();
		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(false);

		ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

		ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		glm::mat4 model = selectedGameObject->transform.GetModelMatrix();
		
		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), currentOperation,
			ImGuizmo::LOCAL, glm::value_ptr(model));

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 position;
			glm::vec3 rotationEuler;
			glm::vec3 scale;

			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), &position.x, &rotationEuler.x, &scale.x);

			// Update the actual components immediately
			selectedGameObject->transform.position = position;
			selectedGameObject->transform.rotation = glm::quat(glm::radians(rotationEuler));
			selectedGameObject->transform.scale = scale;
		}
	}

	void EditorUI::UpdateGizmoOperation()
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsMouseDown(ImGuiMouseButton_Right) || io.WantCaptureKeyboard) return;

		if (ImGui::IsKeyPressed(ImGuiKey_W)) currentOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_E)) currentOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R)) currentOperation = ImGuizmo::SCALE;
	}

	void EditorUI::ShowGameObjectInHierachy(GameObject* obj, Scene* scene)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

		bool isSelected = scene->GetSelectedGameObject() == obj;

		if (isSelected)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (obj->GetChildObjectCount() <= 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		bool opened = ImGui::TreeNodeEx((void *)obj, flags, "%s", obj->name.c_str());

		if (ImGui::IsItemClicked())
		{
			scene->SetSelectedGameObject(obj);
		}

		if (opened)
		{
			for (GameObject* child : obj->childObjects)
			{
				ShowGameObjectInHierachy(child, scene);
			}
			ImGui::TreePop();
		}
	}

	void EditorUI::LightingSetting(DirectionalLight* dirLight)
	{
		ImGui::Begin("Directional Light Settings");

		ImGui::Text("Direction");
		ImGui::SameLine();
		ImGui::DragFloat3("##Direction", &dirLight->direction.x, 0.01f, -1.0f, 1.0f, "%.3f");

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

		ImGui::End();
	}
}