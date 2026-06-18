#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Scene.h"
#include "ImGuizmo.h"

namespace EthernalEngine
{
	class EditorUI
	{
	public:
		bool Initialize(GLFWwindow* window);
		void BeginFrame();
		void EndFrame();
		void RenderUI(Scene* scene);
		void Shutdown();

	private:
		void MainMenuBar(Scene* scene);
		void Hierarchy(Scene* scene);
		void Inspector(GameObject* gameObject);
		void LightingSetting(DirectionalLight* dirLight);
		void DrawGizmo(GameObject* selectedGameObject, Camera* camera);
		void UpdateGizmoOperation();
		void ShowGameObjectInHierachy(GameObject* obj, Scene* scene);

	public:
		ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
	};
}
