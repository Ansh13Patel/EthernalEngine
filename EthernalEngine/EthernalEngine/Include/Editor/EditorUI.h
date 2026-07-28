#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "Scene/Scene.h"
#include "ImGuizmo.h"
#include "Editor/DirectionalLightEditorUI.h"
#include "Editor/PointLightEditorUI.h"
#include "Editor/SpotLightEditorUI.h"
#include "Editor/MeshEditorUI.h"
#include "Editor/MaterialEditorUI.h"

namespace EthernalEngine
{
	struct Viewport
	{
		bool viewportHovered = false;
		bool viewportFocused = false;
		ImVec2 viewportMin = ImVec2(0, 0);
		ImVec2 viewportMax = ImVec2(0, 0);
		ImVec2 viewportSize = ImVec2(0, 0);
	};

	struct EditorWindowName
	{
		static constexpr const char* Scene = "Scene";
		static constexpr const char* Game = "Game";
		static constexpr const char* Inspector = "Inspector";
		static constexpr const char* Hierarchy = "Hierarchy";
		static constexpr const char* Console = "Console";
		static constexpr const char* ProjectBrowser = "Project Browser";
	};

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
		void ToolBar();
		void HierarchyTab(Scene* scene);
		void InspectorTab(GameObject* gameObject);
		void SceneTab(Scene* scene);
		void GameTab();
		void ProjectTab();
		void ConsoleTab();
		void DrawGizmo(GameObject* selectedGameObject, EngineCamera* EngineCamera);
		void UpdateGizmoOperation();
		void ShowGameObjectInHierachy(GameObject* obj, Scene* scene);
		void ShowEngineCameraProperties(EngineCamera* scene);
		void LoadDefaultLayout(bool forceReset = false);

	public:
		ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
		Viewport viewport;

	private:
		DirectionalLightEditorUI dlEditorUI;
		PointLightEditorUI plEditorUI;
		SpotLightEditorUI slEditorUI;
		MeshEditorUI meshEditorUI;
		MaterialEditorUI materialEditorUI;
		bool m_forceResetLayout = false;
	};
}
