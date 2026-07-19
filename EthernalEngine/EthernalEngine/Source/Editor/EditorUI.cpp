#include "Editor/EditorUI.h"
#include "Helper/FileHelper.h"
#include "Editor/EditorPopup.h"
#include "IconsFontAwesome7.h"

#include <windows.h>
#include <commdlg.h>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace EthernalEngine
{
	bool EditorUI::Initialize(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		io.Fonts->AddFontFromFileTTF("Fonts/roboto-regular.ttf", 18.0f);
		static const ImWchar icons_ranges[] =
		{
			ICON_MIN_FA,
			ICON_MAX_16_FA,
			0
		};

		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH = true;

		io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.otf", 18.0f, &config, icons_ranges);

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
        if (scene == nullptr) return;
		MainMenuBar(scene);
		Hierarchy(scene);
		Inspector(scene->GetSelectedGameObject());
		UpdateGizmoOperation();
        DrawGizmo(scene->GetSelectedGameObject(), &scene->GetCamera());
		EditorPopup::Render();
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
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Scene"))
				{
					const char* filter = "Ethernal Files(*.ethernal)\0 * .ethernal\0";
					std::string sceneData = scene->SerializeScene().dump();
					FileHelper::OpenFileSave(filter, sceneData);
				}
				if (ImGui::MenuItem("Load Scene"))
				{
					Popup popup = { PopupType::Confirmation, "Open Scene",
						"Opening a new scene will discard all unsaved changes.\n\nDo you want to continue?",
						[scene]() {
							const char* filter = "Ethernal Files(*.ethernal)\0 * .ethernal\0";
							std::string filepath = FileHelper::OpenFilePick(filter);
							if (!filepath.empty())
							{
								std::ifstream file(filepath);
								if (!file.is_open())
								{
									std::cout << "Failed to open file" << filepath << std::endl;
									return;
								}
								std::stringstream buffer;
								buffer << file.rdbuf();
								std::string scenedata = buffer.str();
								file.close();

								try
								{
									json sceneJson = json::parse(scenedata);
									scene->pendingSceneData = sceneJson;
									scene->pendingSceneLoad = true;
								}
								catch (const json::parse_error& e)
								{
									std::cout << "Json Parse Error: " << e.what() << std::endl;
								}
							}
						} };
					EditorPopup::ShowPopup(popup);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::BeginMenu("3D"))
				{
					if (ImGui::MenuItem("Cube"))
					{
						scene->AddGameObject(scene->CreateCubeGameObject("NewCube"));
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Light"))
				{
					if (ImGui::MenuItem("Directional"))
					{
						scene->AddDirectionalLight(scene->CreateGameObjectWithDirectionalLight());
					}
					if (ImGui::MenuItem("Point"))
					{
						scene->AddPointLight(scene->CreateGameObjectWithPointLight());
					}
					if (ImGui::MenuItem("Spot"))
					{
						scene->AddSpotLight(scene->CreateGameObjectWithSpotLight());
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Import"))
				{
					std::string filepath = FileHelper::OpenFilePick("Model Files\0*.obj;*.fbx;*.gltf;*.glb\0All Files\0*.*\0");
					if (!filepath.empty())
					{
						std::string gameobjectname = FileHelper::GetFileName(filepath);
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

            Transform* transform = gameObject->transform;
			glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(transform->rotation));
            ImGui::Text("Position");
            ImGui::SameLine();
            ImGui::DragFloat3("##Position", &transform->position.x, 0.1f, -1000.0f, 1000.0f, "%.3f");
            ImGui::Text("Rotation");
            ImGui::SameLine();
			if (ImGui::DragFloat3("##Rotation", glm::value_ptr(rotationEuler), 0.1f))
			{
				transform->rotation = glm::quat(glm::radians(rotationEuler));
			}
            ImGui::Text("Scale");
            ImGui::SameLine();
            ImGui::DragFloat3("##Scale", &transform->scale.x, 0.1f, 0.0f, 1000.0f, "%.3f");

			if (gameObject->GetMaterial() != nullptr)
			{
				materialEditorUI.ShowMaterialParameters(gameObject->GetMaterial());
			}

			if(gameObject->GetMesh() != nullptr)
			{
				meshEditorUI.ShowMeshParameters(gameObject->GetMesh());
			}

			for (int i = 0; i < gameObject->components.size(); i++)
			{
				Component* component = gameObject->components[i];

				if (dynamic_cast<DirectionalLight*>(component))
				{
					dlEditorUI.ShowDirectionalLightParameters(dynamic_cast<DirectionalLight*>(component));
				}
				else if (dynamic_cast<PointLight*>(component))
				{
					plEditorUI.ShowPointLightParameters(dynamic_cast<PointLight*>(component));
				}
				else if (dynamic_cast<SpotLight*>(component))
				{
					slEditorUI.ShowSpotLightParameters(dynamic_cast<SpotLight*>(component));
				}
			}
        }
        ImGui::End();
    }

	void EditorUI::DrawGizmo(GameObject* selectedGameObject, EngineCamera* EngineCamera)
	{
		if (!selectedGameObject) return;

		ImGuizmo::BeginFrame();
		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(false);

		ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

		ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

		glm::mat4 view = EngineCamera->GetViewMatrix();
		glm::mat4 projection = EngineCamera->GetProjectionMatrix();

		glm::mat4 model = selectedGameObject->transform->GetLocalMatrix();
		
		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), currentOperation,
			ImGuizmo::LOCAL, glm::value_ptr(model));

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 position;
			glm::vec3 rotationEuler;
			glm::vec3 scale;

			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), &position.x, &rotationEuler.x, &scale.x);

			// Update the actual components immediately
			selectedGameObject->transform->position = position;
			selectedGameObject->transform->rotation = glm::quat(glm::radians(rotationEuler));
			selectedGameObject->transform->scale = scale;
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
}