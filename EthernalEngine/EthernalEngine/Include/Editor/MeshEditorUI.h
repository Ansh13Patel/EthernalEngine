#pragma once
#include "Rendering/Mesh.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ImGuizmo.h"

namespace EthernalEngine
{
	class MeshEditorUI
	{
	public:
		MeshEditorUI() = default;
		~MeshEditorUI() = default;

		void ShowMeshParameters(Mesh* mesh);
	};
}