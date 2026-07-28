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
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.IniFilename = "imgui.ini";
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::LoadIniSettingsFromDisk(io.IniFilename);
		ImGui::SaveIniSettingsToDisk(ImGui::GetIO().IniFilename);

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
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_MenuBar;

		ImGui::Begin("##EditorRoot", nullptr, flags);
		
		if (ImGui::BeginMenuBar())
		{
			MainMenuBar(scene);
			ImGui::EndMenuBar();
		}

		ToolBar();
		ImGui::Separator();

		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspace_id);

		if (m_forceResetLayout)
		{
			m_forceResetLayout = false;
			LoadDefaultLayout(true);
		}
		else
		{
			LoadDefaultLayout();
		}

		ImGui::End();

		SceneTab(scene);
		GameTab();
		ProjectTab();
		HierarchyTab(scene);
		InspectorTab(scene->GetSelectedGameObject());
		UpdateGizmoOperation();
		EditorPopup::Render();
		ConsoleTab();
	}
	void EditorUI::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorUI::SceneTab(Scene* scene)
	{
		ImGui::Begin(EditorWindowName::Scene);

		ImVec2 size = ImGui::GetContentRegionAvail();

		FrameBuffer* frameBuffer = scene->GetSceneBuffer();
		if (frameBuffer != nullptr)
		{
			frameBuffer->Resize(size.x, size.y);
		}
		ImGui::Image(
			(ImTextureID)(uintptr_t)scene->GetSceneBuffer()->GetColorTexture(),
			size,
			ImVec2(0, 1),
			ImVec2(1, 0));

		viewport.viewportFocused = ImGui::IsItemFocused();
		viewport.viewportHovered = ImGui::IsItemHovered();
		viewport.viewportMax = ImGui::GetItemRectMax();
		viewport.viewportMin = ImGui::GetItemRectMin();
		viewport.viewportSize = ImGui::GetItemRectSize();

		DrawGizmo(scene->GetSelectedGameObject(), &scene->GetCamera());

		ImGui::End();
	}

	void EditorUI::GameTab()
	{
		ImGui::Begin(EditorWindowName::Game);
		ImGui::End();
	}

	void EditorUI::ProjectTab()
	{
		ImGui::Begin(EditorWindowName::ProjectBrowser);
		ImGui::End();
	}

	void EditorUI::MainMenuBar(Scene* scene)
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
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Reset Layout"))
			{
				m_forceResetLayout = true;
			}
			ImGui::EndMenu();
		}
	}

	void EditorUI::ToolBar()
	{
		float buttonSize = 30.0f;

		float totalWidth = buttonSize * 2 + ImGui::GetStyle().ItemSpacing.x;
		float availWidth = ImGui::GetContentRegionAvail().x;

		ImGui::SetCursorPosX((availWidth - totalWidth) * 0.5f);

		if (ImGui::Button(ICON_FA_PLAY, ImVec2(buttonSize, buttonSize)))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PAUSE, ImVec2(buttonSize, buttonSize)))
		{

		}
	}

	void EditorUI::HierarchyTab(Scene* scene)
	{
		std::vector<GameObject*> objects = scene->GetGameObjects();

		ImGui::Begin(EditorWindowName::Hierarchy);
		for (int i = 0; i < scene->GetGameObjectCount(); i++)
		{
			ShowGameObjectInHierachy(scene->GetGameObjects()[i], scene);
		}
		ImGui::End();
	}

	void EditorUI::InspectorTab(GameObject* gameObject)
	{
		ImGui::Begin(EditorWindowName::Inspector);
		if (gameObject != nullptr)
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

			if (gameObject->GetMesh() != nullptr)
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

	void EditorUI::DrawGizmo(GameObject* selectedGameObject, EngineCamera* engineCamera)
	{
		if (selectedGameObject == nullptr || engineCamera == nullptr) return;

		ImGuizmo::BeginFrame();
		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(false);

		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(viewport.viewportMin.x, viewport.viewportMin.y,
			viewport.viewportMax.x - viewport.viewportMin.x, viewport.viewportMax.y - viewport.viewportMin.y);

		glm::mat4 view = engineCamera->GetViewMatrix();
		glm::mat4 projection = engineCamera->GetProjectionMatrix();

		glm::mat4 model = selectedGameObject->transform->GetWorldMatrix();

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
		if (obj == nullptr || scene == nullptr) return;

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

		bool opened = ImGui::TreeNodeEx((void*)obj, flags, "%s", obj->name.c_str());

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

	void EditorUI::ShowEngineCameraProperties(EngineCamera* cam)
	{
		if (cam == nullptr) return;

		ImGui::Begin("Engine Camera Properties");
		if (cam != nullptr)
		{
			static int selected_idx = 0;

			std::vector<std::string> options = { "Perspective", "Orthographic" };

			std::string combo_preview_value = options[selected_idx];

			if (ImGui::BeginCombo("Options", combo_preview_value.c_str()))
			{
				for (int i = 0; i < options.size(); i++)
				{
					const bool isSelected = (selected_idx == i);

					if (ImGui::Selectable(options[i].c_str(), isSelected))
					{
						selected_idx = i;
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				cam->isPerspective = (selected_idx == 0);
				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}

	void EditorUI::ConsoleTab()
	{
		ImGui::Begin(EditorWindowName::Console);
		ImGui::End();
	}

	void EditorUI::LoadDefaultLayout(bool forceReset)
	{
		static bool initialized = false;

		if ((initialized || std::filesystem::exists("imgui.ini")) && !forceReset)
			return;

		initialized = true;

		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

		// Use the available content region of EditorRoot instead of the full viewport
		ImGui::DockBuilderSetNodeSize(
			dockspace_id,
			ImGui::GetMainViewport()->WorkSize);

		ImGuiID dock_main = dockspace_id;

		ImGuiID dock_left;
		ImGuiID dock_right;
		ImGuiID dock_bottom;
		ImGuiID dock_center;
		ImGuiID dock_bottom_left;
		ImGuiID dock_bottom_right;
		ImGuiID dock_center_left;
		ImGuiID dock_center_right;


		// Left : Hierarchy (20%)
		dock_left = ImGui::DockBuilderSplitNode(
			dock_main,
			ImGuiDir_Left,
			0.20f,
			nullptr,
			&dock_main);

		// Right : Inspector (25%)
		dock_right = ImGui::DockBuilderSplitNode(
			dock_main,
			ImGuiDir_Right,
			0.25f,
			nullptr,
			&dock_main);

		// Bottom : Project + Console (25%)
		dock_bottom = ImGui::DockBuilderSplitNode(
			dock_main,
			ImGuiDir_Down,
			0.25f,
			nullptr,
			&dock_main);

		dock_bottom_left = ImGui::DockBuilderSplitNode(
			dock_bottom,
			ImGuiDir_Left,
			0.25f,
			nullptr,
			&dock_bottom_right);

		// Split the center into Scene/Game tabs
		dock_center = dock_main;

		dock_center_left = ImGui::DockBuilderSplitNode(
			dock_center,
			ImGuiDir_Left,
			0.25f,
			nullptr,
			&dock_center_right);

		ImGui::DockBuilderDockWindow(EditorWindowName::Hierarchy, dock_left);
		ImGui::DockBuilderDockWindow(EditorWindowName::Inspector, dock_right);

		ImGui::DockBuilderDockWindow(EditorWindowName::Scene, dock_center_left);
		ImGui::DockBuilderDockWindow(EditorWindowName::Game, dock_center_right);

		ImGui::DockBuilderDockWindow(EditorWindowName::ProjectBrowser, dock_bottom_left);
		ImGui::DockBuilderDockWindow(EditorWindowName::Console, dock_bottom_right);

		ImGui::DockBuilderFinish(dockspace_id);
	}
}