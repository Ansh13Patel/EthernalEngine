#include "EditorUI.h"

namespace EthernalEngine
{
	bool EditorUI::Initialize(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
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
}