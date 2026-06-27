#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Scene/Scene.h"
#include "ImGuizmo.h"
#include "Editor/DirectionalLightEditorUI.h"
#include "Editor/PointLightEditorUI.h"
#include "Editor/SpotLightEditorUI.h"
#include "Editor/MeshEditorUI.h"

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
		void DrawGizmo(GameObject* selectedGameObject, EngineCamera* EngineCamera);
		void UpdateGizmoOperation();
		void ShowGameObjectInHierachy(GameObject* obj, Scene* scene);

	public:
		ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;

	private:
		DirectionalLightEditorUI dlEditorUI;
		PointLightEditorUI plEditorUI;
		SpotLightEditorUI slEditorUI;
		MeshEditorUI meshEditorUI;
	};
}
